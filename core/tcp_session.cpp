#include "tcp_session.h"

namespace guodong {

TcpSession::TcpSession(boost::asio::io_context &io_context)
    : context_(io_context)
    , socket_(std::make_shared<tcp::socket>(io_context)) {}

TcpSession::~TcpSession() { LOG(INFO) << " ~TcpSession !!!!!!"; }

std::shared_ptr<tcp::socket> TcpSession::Socket() { return socket_; }

void TcpSession::StartRead() {
    auto session = std::weak_ptr<TcpSession>(shared_from_this());
    socket_->async_read_some(boost::asio::buffer(recv_buffer_, buffer_length_),
                             [session](const boost::system::error_code &error, size_t bytes_transferred) {
                                 auto s = session.lock();
                                 s->onRead(error, bytes_transferred);
                             });
}

void TcpSession::onRead(const boost::system::error_code &error, size_t bytes_transferred) {
    if (!error) {
        LOG(INFO) << LOG_KV("bytes_transferred", bytes_transferred) << LOG_KV("content", recv_buffer_);
        recv_protocol_buffer_.insert(recv_protocol_buffer_.end(), recv_buffer_, recv_buffer_ + bytes_transferred);
        while (true) {
            auto message = message_factory_.BuildMessage();
            ssize_t result = message->Decode(recv_protocol_buffer_.data(), recv_protocol_buffer_.size());
            if (result > 0) {
                onMessage(0, message);
                recv_protocol_buffer_.erase(recv_protocol_buffer_.begin(), recv_protocol_buffer_.begin() + result);
            } else if (result == 0) {
                StartRead();
                break;
            } else if (result < 0) {
                LOG(ERROR) << LOG_KV("onRead Protocol parser error", result);
                disconnect(-1);
                break;
            }
        }
    } else {
        LOG(ERROR) << LOG_KV("onRead fail, error", error.message());
        if (actived_) {
            disconnect(-1);
        }
    }
}

void TcpSession::onMessage(int, Message::Ptr message) {
    try {
        if (!actived_) {
            LOG(ERROR) << LOG_DESC("onMessage ChannelSession inactived");
            return;
        }
        auto response_callback = findResponseCallbackBySeq(message->AskId());
        if (response_callback != nullptr) {
            if (response_callback->timeoutHandler) {
                response_callback->timeoutHandler->cancel();
            }

            if (response_callback->callback) {
                response_thread_->enqueue([=]() {
                    response_callback->callback(0, message);
                    eraseResponseCallbackBySeq(message->AskId());
                });
            } else {
                LOG(ERROR) << LOG_DESC("onMessage Callback empty");
                eraseResponseCallbackBySeq(message->AskId());
            }
        } else {
            if (message_handler_) {
                auto session = std::weak_ptr<TcpSession>(shared_from_this());
                request_thread_->enqueue([session, message]() {
                    auto s = session.lock();
                    if (s && s->message_handler_) {
                        s->message_handler_(s, 0, message);
                    }
                });
            } else {
                LOG(ERROR) << LOG_DESC("MessageHandler empty");
            }
        }
        // // stat AMOP incoming-network-traffic
        // if (m_networkStat && isAMOPMessage(message)) {
        //   m_networkStat->updateAMOPInTraffic(message->length());
        // }
    } catch (std::exception &e) {
        LOG(ERROR) << LOG_DESC("onMessage error") << LOG_KV("what", boost::diagnostic_information(e));
    }
}

void TcpSession::AsyncSendMessage(Message::Ptr request, CallbackType callback, uint32_t timeout) {
    try {
        if (!actived_) {
            LOG(INFO) << "RRRRRRRRRRRRReee";
            if (callback) {
                callback(-3, Message::Ptr());
            }
            return;
        }

        ResponseCallback::Ptr responseCallback = std::make_shared<ResponseCallback>();
        if (callback) {
            responseCallback->askid_ = request->AskId();
            responseCallback->callback = callback;
            if (timeout > 0) {
                std::shared_ptr<boost::asio::deadline_timer> timeoutHandler =
                    std::make_shared<boost::asio::deadline_timer>(context_, boost::posix_time::milliseconds(timeout));
                timeoutHandler->async_wait(boost::bind(&TcpSession::onTimeout, shared_from_this(),
                                                       boost::asio::placeholders::error, responseCallback->askid_));
                responseCallback->timeoutHandler = timeoutHandler;
            }
            insertResponseCallback(responseCallback->askid_, responseCallback);
        }
        std::shared_ptr<std::vector<uint8_t>> buffer = std::make_shared<std::vector<uint8_t>>();
        request->Encode(*buffer);
        writeBuffer(buffer);
        // // update the group outgoing traffic
        // if (m_networkStat && request->groupID() != -1) {
        //   m_networkStat->updateGroupResponseTraffic(
        //       request->groupID(), request->type(), request->length());
        // }
        // // stat AMOP outgoing-network-traffic
        // if (m_networkStat && isAMOPMessage(request)) {
        //   m_networkStat->updateAMOPOutTraffic(request->length());
        // }
    } catch (std::exception &e) {
        LOG(ERROR) << LOG_DESC("asyncSendMessage error") << LOG_KV("what", boost::diagnostic_information(e));
    }
}

void TcpSession::onTimeout(const boost::system::error_code &error, int askid) {
    try {
        if (error) {
            if (error != boost::asio::error::operation_aborted) {
                LOG(ERROR) << "Timer error" << LOG_KV("message", error.message());
            }
            return;
        }

        if (!actived_) {
            LOG(ERROR) << "ChannelSession inactived";
            return;
        }

        auto response_callback = findResponseCallbackBySeq(askid);
        if (response_callback) {
            if (response_callback->callback) {
                response_callback->callback(-2, Message::Ptr());
            } else {
                LOG(ERROR) << "Callback empty";
            }
            eraseResponseCallbackBySeq(askid);
        } else {
            LOG(WARNING) << "onTimeout Seq timeout" << LOG_KV("askid", askid);
        }
    } catch (std::exception &e) {
        LOG(ERROR) << "onTimeout error" << LOG_KV("what", boost::diagnostic_information(e));
    }
}

void TcpSession::startWrite() {
    if (!actived_) {
        LOG(ERROR) << "startWrite ChannelSession inactived";
        return;
    }
    if (writing_) {
        return;
    }
    if (!send_buffer_.empty()) {
        writing_ = true;
        auto buffer = send_buffer_.front();
        send_buffer_.pop();
        auto session = std::weak_ptr<TcpSession>(shared_from_this());
        context_.post([session, buffer] {
            auto s = session.lock();
            if (s) {
                boost::asio::async_write(*(s->Socket()), boost::asio::buffer(buffer->data(), buffer->size()),
                                         [=](const boost::system::error_code &error, size_t bytes_transferred) {
                                             auto s = session.lock();
                                             if (s) {
                                                 s->onWrite(error, buffer, bytes_transferred);
                                             }
                                         });
            }
        });
    } else {
        writing_ = false;
    }
}

void TcpSession::writeBuffer(std::shared_ptr<std::vector<uint8_t>> buffer) {
    try {
        std::lock_guard<std::recursive_mutex> lock(mutex_);
        send_buffer_.push(buffer);
        startWrite();
    } catch (std::exception &e) {
        LOG(ERROR) << "writeBuffer error" << LOG_KV("what", boost::diagnostic_information(e));
    }
}

void TcpSession::onWrite(const boost::system::error_code &error, std::shared_ptr<std::vector<uint8_t>>, size_t) {
    try {
        if (!actived_) {
            LOG(ERROR) << "ChannelSession inactived";
            return;
        }
        std::lock_guard<std::recursive_mutex> lock(mutex_);
        // updateIdleTimer();
        if (error) {
            LOG(ERROR) << LOG_DESC("Write error") << LOG_KV("message", error.message());
            disconnect(-1);
            return;
        }
        writing_ = false;
        startWrite();
    } catch (std::exception &e) {
        LOG(ERROR) << LOG_DESC("onWrite error") << LOG_KV("what", boost::diagnostic_information(e));
    }
}

void TcpSession::disconnect(int ecode) {
    LOG(INFO) << LOG_KV("disconnect, ecode = ", ecode);
    try {
        std::lock_guard<std::recursive_mutex> lock(mutex_);
        if (actived_) {
            //_idleTimer->cancel();
            actived_ = false;

            {
                ReadGuard l(callback_lock_);
                for (auto &it : response_callbacks_) {
                    try {
                        if (it.second->timeoutHandler.get() != NULL) {
                            it.second->timeoutHandler->cancel();
                        }

                        if (it.second->callback) {
                            response_thread_->enqueue([=]() { it.second->callback(0, Message::Ptr()); });
                        } else {
                            LOG(ERROR) << "Callback empty";
                        }
                    } catch (std::exception &e) {
                        LOG(ERROR) << "Disconnect responseCallback" << LOG_KV("responseCallback", it.first)
                                   << LOG_KV("what", boost::diagnostic_information(e));
                    }
                }
            }
            clearResponseCallbacks();
            if (message_handler_) {
                try {
                    message_handler_(shared_from_this(), -1, Message::Ptr());
                } catch (std::exception &e) {
                    LOG(ERROR) << "disconnect messageHandler error" << LOG_KV("what", boost::diagnostic_information(e));
                }
                message_handler_ = std::function<void(TcpSession::Ptr, int, Message::Ptr)>();
            }

            // auto socket = socket_;
            // force close socket after 30 seconds
            // auto socket = socket_;
            // auto shutdownTimer =
            //     std::make_shared<boost::asio::deadline_timer>(context_, boost::posix_time::milliseconds(30000));
            // shutdownTimer->async_wait([socket](const boost::system::error_code &error) {
            //     if (error && error != boost::asio::error::operation_aborted) {
            //         LOG(WARNING) << "channel shutdown timer error" << LOG_KV("message", error.message());
            //         return;
            //     }

            //     if (socket->is_open()) {
            //         LOG(WARNING) << "channel shutdown timeout force close";
            //         socket->close();
            //     }
            // });
            socket_->shutdown(boost::asio::ip::tcp::socket::shutdown_both);
            // socket_->shutdown([socket, shutdownTimer](const boost::system::error_code &error) {
            //     if (error) {
            //         LOG(WARNING) << "async_shutdown" << LOG_KV("message", error.message());
            //     }
            //     shutdownTimer->cancel();

            //     if (socket->is_open()) {
            //         socket->close();
            //     }
            // });
            LOG(DEBUG) << "Disconnected";
        }
    } catch (std::exception &e) {
        LOG(WARNING) << "Close error" << LOG_KV("what", boost::diagnostic_information(e));
    }
}

void TcpSession::InitResolver() { resolver_ = std::make_shared<tcp::resolver>(context_); }

bool TcpSession::AsyncConnect(std::string ip, int port,
                              boost::function<void(const boost::system::error_code &)> handler) {
    if (!resolver_) {
        LOG(ERROR) << "resolver not init!";
        return false;
    }
    auto connect_timer = std::make_shared<boost::asio::deadline_timer>(context_, boost::posix_time::milliseconds(1000));
    auto socket = socket_;
    connect_timer->async_wait([=](const boost::system::error_code &error) {
        /// return when cancel has been called
        if (error == boost::asio::error::operation_aborted) {
            LOG(DEBUG) << LOG_DESC("AsyncConnect handshake handler revoke this operation");
            return;
        }
        /// connection timer error
        if (error && error != boost::asio::error::operation_aborted) {
            LOG(ERROR) << LOG_DESC("AsyncConnect timer failed") << LOG_KV("errorValue", error.value())
                       << LOG_KV("message", error.message());
        }
        socket->close();
        LOG(WARNING) << LOG_DESC("AsyncConnect timeout erase");
    });
    auto protocol = tcp::tcp::v4();
    resolver_->async_resolve(protocol, ip, std::to_string(port),
                             [=](const boost::system::error_code &ec, tcp::resolver::results_type results) {
                                 if (!ec) {
                                     // results is a iterator, but only use first endpoint.
                                     socket->async_connect(results->endpoint(), handler);
                                     LOG(INFO)
                                         << LOG_DESC("asyncResolveConnect") << LOG_KV("endpoint", results->endpoint());
                                 } else {
                                     LOG(WARNING) << LOG_DESC("asyncResolve failed");
                                 }
                             });
    return true;
}

Message::Ptr TcpSession::BuildMessage() { return message_factory_.BuildMessage(); }

// void TcpSession::updateIdleTimer() {
//   if (!actived_) {
//     LOG(ERROR) << "ChannelSession inactived";
//     return;
//   }

//   if (_idleTimer) {
//     _idleTimer->expires_from_now(boost::posix_time::seconds(_idleTime));

//     auto session = std::weak_ptr<ChannelSession>(shared_from_this());
//     _idleTimer->async_wait([session](const boost::system::error_code &error)
//     {
//       auto s = session.lock();
//       if (s) {
//         s->onIdle(error);
//       }
//     });
//   }
// }

}  // namespace guodong
