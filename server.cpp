#include <thread>

#include "mocker/publishers.h"

int main(int argc, char const *argv[]) {
    if (argc < 2) {
        printf("usage: %s [address]\n", argv[0]);
        return 1;
    }

    std::vector<std::string> symbols{"600000", "000001"};

    Publisher publisher{argv[1]};
    std::jthread bar_sender([&publisher, &symbols] {
        publish_bars(publisher, symbols, 3000);
    });
    std::jthread tick_sender([&publisher, &symbols] {
        publish_ticks(publisher, symbols);
    });
}
