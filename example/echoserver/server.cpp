#include <tcp_server.h>

using namespace guodong;

class EchoServer : public TcpServer {
public:
    EchoServer() : TcpServer() {}
    void OnConnect(TcpSession::Ptr session) override {
        LOG(TRACE) << LOG_DESC("connect success !") << LOG_KV("remote ip = ", session->GetRemoteIp())
                   << LOG_KV("remote port = ", session->GetRemotePort());
    }
    void OnMessageRequest(TcpSession::Ptr session, int error, Message::Ptr message) override {
        if (!message || error != 0) {
            LOG(TRACE) << LOG_DESC("connect disable");
            return;
        }
        auto type = message->Type();
        auto askid = message->AskId();
        auto body = message->Body();
        std::cout << "rrrrr = " << body->size() << std::endl;
        std::string content(body->begin(), body->end());
        LOG(TRACE) << LOG_KV("error", error) << LOG_KV("body", content) << LOG_KV("type", type)
                   << LOG_KV("askid", askid);
        auto new_message = session->BuildMessage();
        new_message->SetType(type);
        new_message->SetAskId(askid);
        new_message->SetMessageId(message->MessageId());
        new_message->SetBody(*(message->Body()));
        session->AsyncSendMessage(new_message, TcpSession::CallbackType(), 0);
    }
};

int main() {
    EchoServer server;
    if (!server.Init(9000)) {
        return 0;
    }
    server.Run();
}