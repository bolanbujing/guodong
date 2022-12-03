#pragma once
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include "tcp_session.h"

namespace guodong {
using boost::asio::ip::tcp;
class TcpClient : public noncopyable {
public:
    TcpClient();
    void AsyncConnect(std::string ip, int port);
    virtual void OnConnect(const boost::system::error_code& ec);
    virtual void OnMessage(TcpSession::Ptr, int, Message::Ptr) = 0;
    void AsyncSendMessage(Message::Ptr message) noexcept;
    void AsyncSendMessage(Message::Ptr message, TcpSession::CallbackType, uint32_t mills) noexcept;

private:
    boost::asio::io_context io_context_;
    boost::asio::io_context::work io_work_;
    std::shared_ptr<guodong::TcpSession> session_;
};

}  // namespace guodong