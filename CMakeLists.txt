cmake_minimum_required(VERSION 4.3)
project(BettingArbitrageEngine)

set(CMAKE_CXX_STANDARD 20)

add_executable(BettingArbitrageEngine main.cpp
        mockFeed.cpp
        ingestion.cpp
        orderBook.cpp
        orderBook.hpp
        footballComputeQueue.cpp
)
