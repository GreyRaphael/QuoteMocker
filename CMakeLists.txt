cmake_minimum_required(VERSION 3.20)
project(quote_simu VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
add_executable(quote_simu main.cpp)

find_package(nng CONFIG REQUIRED)
target_link_libraries(quote_simu PRIVATE nng::nng)