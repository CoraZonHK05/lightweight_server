cmake_minimum_required(VERSION 3.10)
project(lightweight_server VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

set(SOURCES
    src/mem_pool.cpp
    src/thread_pool.cpp
    src/buffer.cpp
    src/tcp_connection.cpp
    src/event_loop.cpp
    src/acceptor.cpp
    main.cpp
)

add_executable(http_server ${SOURCES})

if(WIN32)
    target_link_libraries(http_server ws2_32)
else()
    find_package(Threads REQUIRED)
    target_link_libraries(http_server Threads::Threads)
endif()
