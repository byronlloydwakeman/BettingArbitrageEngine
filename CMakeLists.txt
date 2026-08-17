cmake_minimum_required(VERSION 4.3)
project(BettingArbitrageEngine)

set(CMAKE_CXX_STANDARD 20)

add_executable(BettingArbitrageEngine main.cpp
        event.hpp
        feed.hpp
        mockFeed.cpp
        mockFeed.hpp
        ingestion.cpp
        ingestion.hpp
        orderBook.cpp
        orderBook.hpp
)
