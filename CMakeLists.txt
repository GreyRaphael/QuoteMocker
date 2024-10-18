cmake_minimum_required(VERSION 3.20)
project(quote_simu VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
add_executable(server server.cpp)
add_executable(client client.cpp)

find_package(nng CONFIG REQUIRED)
target_link_libraries(server PRIVATE nng::nng)
target_link_libraries(client PRIVATE nng::nng)