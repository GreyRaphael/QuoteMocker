cmake_minimum_required(VERSION 3.20)
project(quote_simu VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
option(BUILD_EXAMPLES "Build example programs" ON)

if(BUILD_EXAMPLES)
    find_package(nng CONFIG REQUIRED)

    # pubsub server and client
    add_executable(pub examples/pubsub/server.cpp)
    target_link_libraries(pub PRIVATE nng::nng)
    target_include_directories(pub PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    add_executable(sub examples/pubsub/client.cpp)
    target_link_libraries(sub PRIVATE nng::nng)
    target_include_directories(sub PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

    # pair1 server and clients
    add_executable(pair_server examples/pair1/server.cpp)
    target_link_libraries(pair_server PRIVATE nng::nng)
    target_include_directories(pair_server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    add_executable(pair_client examples/pair1/client.cpp)
    target_link_libraries(pair_client PRIVATE nng::nng)
    target_include_directories(pair_client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
endif()