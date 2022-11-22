#include "tcp_client.h"

using namespace guodong;

class EchoClient : public TcpClient {
public:
    EchoClient(boost::asio::io_context& context) : TcpClient(context) {}
    void Connect(std::string ip, int port) {
        AsyncConnect(ip, port);
    }
    void OnConnect(const boost::system::error_code& ec) override {
        LOG(INFO) << LOG_DESC("connect success!") << LOG_KV("ecode=", ec);
    }
    void OnMessage(TcpSession::Ptr, int ecode, Message::Ptr message) override {
        std::string content(message->Body().begin(), message->Body().end());
        LOG(DEBUG) << LOG_DESC(content) << LOG_KV("ecode=", ecode);
    }
    void Write() {
        std::string line;
        while (std::getline(std::cin, line)) {
            auto new_message = BuildMessage();
            new_message->SetType(2);
            new_message->SetAskId(1);
            new_message->SetMessageId(100);
            std::vector<uint8_t> vec(line.begin(), line.end());
            new_message->SetBody(vec);
            AsyncSendMessage(new_message);
        }
        
    }
};

int main() {
    boost::asio::io_context io_context;
    EchoClient client(io_context);
    client.Connect("127.0.0.1", 9000);
    std::thread t([&](){
        io_context.run();
    });
    client.Write();
}