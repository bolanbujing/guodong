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
 private:
    void onConnect(boost::system::error_code const& ec);
    boost::asio::io_context& io_context_;
    std::shared_ptr<guodong::TcpSession> session_;
};
}