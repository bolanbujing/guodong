#include "log.h"
#include "tcp_client.h"

namespace guodong {
TcpClient::TcpClient(boost::asio::io_context& io_context)
    : io_context_(io_context) {}

void TcpClient::AsyncConnect(std::string ip, int port) {
    session_ = std::make_shared<TcpSession>(io_context_);
    session_->InitResolver();
    boost::function<void(const boost::system::error_code&)> handler =
        boost::bind(&TcpClient::OnConnect, this, boost::asio::placeholders::error);
    session_->AsyncConnect(ip, port, handler);
}

// void TcpClient::onConnect(const boost::system::error_code& ec) {
//     if (ec) {
//         LOG(ERROR) << LOG_DESC("TCP Connection refused by node") << LOG_KV("message", ec.message());

//         // m_threadPool->enqueue([callback, _nodeIPEndpoint]() {
//         //     callback(NetworkException(ConnectError, "Connect failed"), NodeInfo(), std::shared_ptr<SessionFace>());
//         // });
//         return;
//     } else {
//         LOG(INFO) << LOG_DESC("onconnect, success");
//         // insertPendingConns(_nodeIPEndpoint);
//         // /// get the public key of the server during handshake
//         // std::shared_ptr<std::string> endpointPublicKey = std::make_shared<std::string>();
//         // m_asioInterface->setVerifyCallback(socket, newVerifyCallback(endpointPublicKey));
//         // /// call handshakeClient after handshake succeed
//         // m_asioInterface->asyncHandshake(
//         //     socket, ba::ssl::stream_base::client,
//         //     boost::bind(&Host::handshakeClient, shared_from_this(), ba::placeholders::error, socket,
//         //     endpointPublicKey,
//         //                 callback, _nodeIPEndpoint, connect_timer));
//     }
// }

void TcpClient::AsyncSendMessage(Message::Ptr message) {
    session_->AsyncSendMessage(message, TcpSession::CallbackType(), 0);
}

}  // namespace guodong
