#include <mocker/kcp/client.hpp>

int main(int argc, char const *argv[]) {
    KcpClient client{"client.json"};
    client.start();

    // realtime
    std::unordered_map<Messages::QuoteType, std::string> sub_map{
        {Messages::QuoteType::K1d, "600000|300016"},
        {Messages::QuoteType::K1min, "688538|000001"},
    };
    client.subscribe(sub_map);

    // history
    std::unordered_map<Messages::QuoteType, std::string> replay_map{
        {Messages::QuoteType::K1d, "688009"},
        {Messages::QuoteType::K1min, "000002"},
    };
    client.replay(sub_map);

    client.wait();
}