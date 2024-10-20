#include "mocker/pair1/client.hpp"

int main(int argc, char const *argv[]) {
    Client client{"tcp://127.0.0.1:8889"};
    client.sub(0, 0, "600000");
}