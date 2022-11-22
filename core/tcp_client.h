#pragma once
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include "tcp_session.h"

namespace guodong {
using boost::asio::ip::tcp;
class TcpClient {
 public:
    TcpClient(boost::asio::io_context& io_context);
    void AsyncConnect(std::string ip, int port);
    Message::Ptr BuildMessage() {
       return session_->BuildMessage();
    }
    virtual void OnConnect(const boost::system::error_code& ec) = 0;
    virtual void OnMessage(TcpSession::Ptr, int, Message::Ptr) = 0;
 protected:
    void AsyncSendMessage(Message::Ptr message);
 private:
    boost::asio::io_context& io_context_;
    std::shared_ptr<guodong::TcpSession> session_;
};
}