#include <cstdio>
#include <mocker/pair1/server.hpp>

int main(int argc, char const *argv[]) {
    Server server{"tcp://127.0.0.1:7772"};
    server.run();
    getchar();
}