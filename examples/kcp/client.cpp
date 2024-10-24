#include <mocker/kcp/client.hpp>

int main(int argc, char const *argv[]) {
    KcpClient client{"client.json"};
    client.start();
    client.subscribe({"BTCUSDT", "ETHUSDT"});
    client.replay({"BTCUSDT", "ETHUSDT"});
    client.wait();
}