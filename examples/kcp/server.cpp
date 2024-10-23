#include <mocker/kcp/server.hpp>

int main(int argc, char const *argv[]) {
    KcpServer server{"server.json"};
    server.start();
    server.wait();
}