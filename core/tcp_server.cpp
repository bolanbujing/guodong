#include <functional>
#include "log.h"
#include "tcp_server.h"
#include "tcp_session.h"

using namespace guodong;

TcpServer::TcpServer(boost::asio::io_context& io_context)
    : io_context_(io_context)
    , acceptor_(io_context, tcp::endpoint(tcp::v4(), 9000)) {
    request_thread_pool_ = std::make_shared<ThreadPool>("request", 8);
    response_thread_pool_ = std::make_shared<ThreadPool>("response", 4);
    startAccept();
}

void TcpServer::startAccept() {
    auto session = std::make_shared<TcpSession>(io_context_);
    std::cout << "start_accept" << std::endl;
    acceptor_.async_accept(*(session->Socket()),
                           boost::bind(&TcpServer::handleAccept, this, session, boost::asio::placeholders::error));
    std::cout << "end_accept" << std::endl;
}

void TcpServer::onConnect(TcpSession::Ptr session) {
    LOG(TRACE) << LOG_DESC("connect success !") << LOG_KV("remote ip = ", session->GetRemoteIp())
               << LOG_KV("remote port = ", session->GetRemotePort());
}

void TcpServer::handleAccept(TcpSession::Ptr session, const boost::system::error_code& error) {
    if (!error) {
        boost::system::error_code ec;
        auto remoteEndpoint = session->Socket()->remote_endpoint(ec);
        session->SetRemoteIp(remoteEndpoint.address().to_string());
        session->SetRemotePort(remoteEndpoint.port());
        std::function<void(TcpSession::Ptr, int, Message::Ptr)> fp = std::bind(
            &TcpServer::onClientRequest, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
        session->SetMessageHandler(fp);
        session->SetBusinessThread(request_thread_pool_, response_thread_pool_);
        session->SetActive();
        onConnect(session);
        session->StartRead();
        std::cout << "handle_accept" << std::endl;
    } else {
        std::cout << "handleAccept fail, error = " << error << std::endl;
    }
    startAccept();
}

void TcpServer::onClientRequest(TcpSession::Ptr, int error, Message::Ptr message) {
    int16_t type = message->Header().type_;
    int32_t askid = message->Header().askid_;
    std::string content(message->Body().begin(), message->Body().end());
    LOG(TRACE) << LOG_KV("error", error) << LOG_KV("body", content) << LOG_KV("type", type) << LOG_KV("askid", askid);
}