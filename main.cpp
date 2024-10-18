#include "simulator/publishers.h"

int main(int argc, char const *argv[]) {
    if (argc < 3) {
        printf("usage: %s [address] [tick|bar]\n", argv[0]);
        return 1;
    }
    std::string_view cmd{argv[2]};

    std::vector<std::string> symbols{"600000", "000001"};
    if (cmd == "tick") {
        publish_ticks(argv[1], symbols);
    } else if (cmd == "bar") {
        publish_bars(argv[1], symbols);
    } else {
        printf("Unknown command:%s\n", argv[2]);
        return 1;
    }
}
