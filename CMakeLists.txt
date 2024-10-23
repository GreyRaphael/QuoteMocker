cmake_minimum_required(VERSION 3.20)
project(quote_simu VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
option(BUILD_NNG_EXAMPLES "Build nng example programs" OFF)
option(BUILD_KCP_EXAMPLES "Build kcp example programs" ON)

if(BUILD_NNG_EXAMPLES)
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

if(BUILD_KCP_EXAMPLES)
    find_package(libhv CONFIG REQUIRED)
    find_package(fmt CONFIG REQUIRED)

    add_executable(kcp_server examples/kcp/server.cpp)
    target_link_libraries(kcp_server PRIVATE hv_static)
    target_link_libraries(kcp_server PRIVATE fmt::fmt)
    target_include_directories(kcp_server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

    add_executable(kcp_client examples/kcp/client.cpp)
    target_link_libraries(kcp_client PRIVATE hv_static)
    target_link_libraries(kcp_client PRIVATE fmt::fmt)
    target_include_directories(kcp_client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
endif()
