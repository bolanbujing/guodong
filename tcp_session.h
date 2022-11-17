#pragma once

#include <memory>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include "message.h"
#include "thread_pool.h"
#include <queue>

using boost::asio::ip::tcp;
namespace guodong {
class TcpSession : public std::enable_shared_from_this<TcpSession> {
 public:
    typedef std::shared_ptr<TcpSession> Ptr;
    using ReadGuard = boost::shared_lock<boost::shared_mutex>;
    using WriteGuard = boost::unique_lock<boost::shared_mutex>;
    explicit TcpSession(boost::asio::io_context& io_context);
    virtual ~TcpSession();
    tcp::socket& Socket();
    void StartRead();
    void SetMessageFactory(MessageFactory message_factory) {
        message_factory_ = message_factory;
    }

    void SetMessageHandler(std::function<void(TcpSession::Ptr, int, Message::Ptr)> message_handler) {
        message_handler_ = message_handler;
    }

    void SetBusinessThread(ThreadPool::Ptr request_thread, ThreadPool::Ptr response_thread) {
        request_thread_ = request_thread;
        response_thread_ = response_thread;
    }

    void SetRemoteIp(std::string ip) {
        remote_ip_ = ip;
    }

    std::string GetRemoteIp() {
        return remote_ip_;
    }

    void SetRemotePort(int port) {
        port_ = port;
    }

    int GetRemotePort() {
        return port_;
    }
    
    void SetActive() {
        actived_ = true;
    }

    bool GetActive() {
        return actived_;
    }
    void AsyncSendMessage(Message::Ptr request, std::function<void(int, Message::Ptr)> callback, uint32_t timeout);
    const int32_t buffer_length_ = 4096;
 private:
    void onRead(const boost::system::error_code& error, size_t bytes_transferred);
    void onMessage(int, Message::Ptr);
    void onTimeout(const boost::system::error_code& error, int askid_);
    void writeBuffer(std::shared_ptr<std::vector<uint8_t>> buffer);
    void startWrite();
    void onWrite(const boost::system::error_code &error, std::shared_ptr<std::vector<uint8_t>>, size_t);
    void disconnect(int ecode);

    struct ResponseCallback : public std::enable_shared_from_this<ResponseCallback> {
        typedef std::shared_ptr<ResponseCallback> Ptr;
        int32_t askid_ = 0;
        std::function<void(int ecode, Message::Ptr)> callback;
        std::shared_ptr<boost::asio::deadline_timer> timeoutHandler;
    };

    void insertResponseCallback(int32_t askid, ResponseCallback::Ptr callback_ptr) {
        WriteGuard l(callback_lock_);
        response_callbacks_.insert(std::make_pair(askid, callback_ptr));
    }

    ResponseCallback::Ptr findResponseCallbackBySeq(int32_t askid) {
        ReadGuard l(callback_lock_);
        auto it = response_callbacks_.find(askid);
        if (it != response_callbacks_.end()) {
            return it->second;
        }
        return nullptr;
    }

    void eraseResponseCallbackBySeq(int32_t askid) {
        WriteGuard l(callback_lock_);
        response_callbacks_.erase(askid);
    }

    void clearResponseCallbacks() {
        WriteGuard l(callback_lock_);
        response_callbacks_.clear();
    }

 private:
    boost::asio::io_context& context_;
    std::function<void(TcpSession::Ptr, int, Message::Ptr)> message_handler_;
    tcp::socket socket_;
    uint8_t recv_buffer_[4096];
    std::vector<uint8_t> recv_protocol_buffer_;
    std::queue<std::shared_ptr<std::vector<uint8_t>>> send_buffer_;
    MessageFactory message_factory_;
    std::string remote_ip_;
    int port_;
    ThreadPool::Ptr request_thread_;
    ThreadPool::Ptr response_thread_;
    bool actived_;
    std::map<int32_t, ResponseCallback::Ptr> response_callbacks_;
    boost::shared_mutex callback_lock_;
    std::recursive_mutex mutex_;
    bool writing_;
};

}