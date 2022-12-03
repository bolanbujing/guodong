#include <functional>
#include "log.h"
#include "tcp_server.h"
#include <tcp_session.h>

using namespace guodong;

std::atomic<uint64_t> seq_{0};

TcpServer::TcpServer()
    : io_work_(io_context_)
    , init_(false) {}

TcpServer::~TcpServer() { LOG(DEBUG) << "tcp server destructor !"; }

bool TcpServer::Init(int port, int thread_for_request, int thread_for_response) {
    if (init_) {
        LOG(ERROR) << "tcpserver already initialized";
        return false;
    }
    if (port <= 0) {
        LOG(ERROR) << LOG_KV("port : ", port);
        return false;
    }
    request_thread_pool_ = std::make_shared<ThreadPool>("request", thread_for_request);
    response_thread_pool_ = std::make_shared<ThreadPool>("response", thread_for_response);
    acceptor_ = std::make_shared<boost::asio::ip::tcp::acceptor>(io_context_, tcp::endpoint(tcp::v4(), port));
    startAccept();
    init_ = true;
    return true;
}

void TcpServer::startAccept() {
    auto session = std::make_shared<TcpSession>(io_context_);
    std::cout << "start_accept" << std::endl;
    acceptor_->async_accept(*(session->Socket()),
                            boost::bind(&TcpServer::handleAccept, this, session, boost::asio::placeholders::error));
    std::cout << "end_accept" << std::endl;
}

void TcpServer::handleAccept(TcpSession::Ptr session, const boost::system::error_code& error) {
    if (!error) {
        boost::system::error_code ec;
        auto remoteEndpoint = session->Socket()->remote_endpoint(ec);
        session->SetRemoteIp(remoteEndpoint.address().to_string());
        session->SetRemotePort(remoteEndpoint.port());
        std::function<void(TcpSession::Ptr, int, Message::Ptr)> fp = std::bind(
            &TcpServer::OnMessageRequest, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
        session->SetMessageHandler(fp);
        session->SetBusinessThread(request_thread_pool_, response_thread_pool_);
        session->SetActive();
        session_map_[seq_++] = session;
        OnConnect(session);
        session->StartRead();
        std::cout << "handle_accept" << std::endl;
    } else {
        std::cout << "handleAccept fail, error = " << error << std::endl;
    }
    startAccept();
}

void TcpServer::Run() {
    if (!init_) {
        LOG(ERROR) << LOG_DESC("init_ == false");
        return;
    }
    io_context_.run();
}
