#pragma once
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include "tcp_session.h"
#include "thread_pool.h"

namespace guodong {

class TcpServer {
public:
    TcpServer(boost::asio::io_context& io_context);

private:
    void startAccept();
    void onConnect(TcpSession::Ptr session);
    void onClientRequest(TcpSession::Ptr, int, Message::Ptr);

    void handleAccept(TcpSession::Ptr session, const boost::system::error_code& error);
    boost::asio::io_context& io_context_;
    boost::asio::ip::tcp::acceptor acceptor_;
    ThreadPool::Ptr request_thread_pool_;
    ThreadPool::Ptr response_thread_pool_;
};

}  // namespace guodong