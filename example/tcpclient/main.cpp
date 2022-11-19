#include "tcp_client.h"

using namespace guodong;

int main() {
    boost::asio::io_context io_context;
    TcpClient client(io_context);
    for (;;) {
        client.AsyncConnect("127.0.0.1", 9000);
        sleep(3);
    }
    
}