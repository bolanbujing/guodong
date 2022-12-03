#pragma once
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include "tcp_session.h"
#include "thread_pool.h"
#include "noncopyable.h"

namespace guodong {

class TcpServer : public noncopyable {
public:
    TcpServer();
    virtual ~TcpServer();
    virtual void OnConnect(TcpSession::Ptr session) = 0;
    virtual void OnMessageRequest(TcpSession::Ptr, int, Message::Ptr) = 0;
    bool Init(int port, int thread_for_request = 8, int thread_for_response = 4);
    void Run();

private:
    void startAccept();

    void handleAccept(TcpSession::Ptr session, const boost::system::error_code& error);
    boost::asio::io_context io_context_;
    std::shared_ptr<boost::asio::ip::tcp::acceptor> acceptor_;
    boost::asio::io_context::work io_work_;
    ThreadPool::Ptr request_thread_pool_;
    ThreadPool::Ptr response_thread_pool_;
    std::map<uint64_t, TcpSession::Ptr> session_map_;
    bool init_;
};

}  // namespace guodong