#include <cstdio>
#include <mocker/pair1/client.hpp>

int main(int argc, char const *argv[]) {
    Client client1{"tcp://127.0.0.1:7772"};
    client1.sub(0, 0, "600000");

    Client client2{"tcp://127.0.0.1:7772"};
    client2.sub(0, 0, "000001");

    getchar();
}