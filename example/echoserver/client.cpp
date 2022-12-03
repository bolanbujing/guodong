#include <tcp_client.h>

using namespace guodong;

class EchoClient : public TcpClient {
public:
    EchoClient() {}
    void Connect(std::string ip, int port) {
        AsyncConnect(ip, port);
    }
    void OnConnect(const boost::system::error_code& ec) override {
        TcpClient::OnConnect(ec);
        LOG(INFO) << LOG_DESC("connect success!") << LOG_KV("ecode=", ec);
    }
    void OnMessage(TcpSession::Ptr, int ecode, Message::Ptr message) override {
        auto body = message->Body();
        std::string content(body->begin(), body->end());
        LOG(DEBUG) << LOG_DESC(content) << LOG_KV("ecode=", ecode);
    }

    void CallBackMessage(int ecode, Message::Ptr message) {
        if (message) {
            auto body = message->Body();
            std::cout << "ssss = " << body->size() << std::endl;
            std::string content(body->begin(), body->end());
            LOG(DEBUG) << LOG_DESC(content) << LOG_KV("ecode=", ecode);
        }
    }
};

int main() {
    EchoClient client;
    client.Connect("127.0.0.1", 9000);
    MessageFactory mf;
    std::string line;
    while (std::getline(std::cin, line)) {
        auto new_message = mf.BuildMessage();
        new_message->SetType(2);
        new_message->SetAskId(1);
        new_message->SetMessageId(100);
        std::cout << "send message : " << line << std::endl;
        std::vector<uint8_t> vec(line.begin(), line.end());
        std::cout << "size = " << vec.size() << std::endl;
        new_message->SetBody(vec);
        client.AsyncSendMessage(new_message, std::bind(&EchoClient::CallBackMessage, &client, std::placeholders::_1, std::placeholders::_2), 1000);
    }
}