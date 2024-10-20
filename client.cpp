#include "mocker/subscribers.h"

int main(int argc, char const *argv[]) {
    if (argc < 2) {
        printf("usage: %s [address]\n", argv[0]);
        return 1;
    }
    std::vector<std::string> symbols{"600000"};
    Subscriber subscriber{argv[1], symbols};
    subscribe_bars(subscriber);
}