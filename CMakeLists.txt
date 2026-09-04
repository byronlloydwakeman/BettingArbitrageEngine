cmake_minimum_required(VERSION 4.3)
project(BettingArbitrageEngine)

set(CMAKE_CXX_STANDARD 20)

add_executable(BettingArbitrageEngine src/main.cpp
        src/mockFeed.cpp
        src/ingestion.cpp
        src/orderBook.cpp
        src/orderBook.hpp
        src/footballComputeQueue.cpp
        src/mockFeed.hpp
        src/metricsRecorder.hpp
        src/prometheusMetricRecorder.cpp
        src/prometheusMetricRecorder.hpp
        src/mockMetricsRecorder.hpp
)

include(FetchContent)

set(ENABLE_TESTING OFF CACHE BOOL "" FORCE)
set(ENABLE_PUSH OFF CACHE BOOL "" FORCE)
set(ENABLE_COMPRESSION OFF CACHE BOOL "" FORCE)
set(OVERRIDE_CXX_STANDARD_FLAGS OFF CACHE BOOL "" FORCE)
set(USE_THIRDPARTY_LIBRARIES ON CACHE BOOL "" FORCE)

FetchContent_Declare(
        prometheus-cpp
        GIT_REPOSITORY https://github.com/jupp0r/prometheus-cpp.git
        GIT_TAG v1.3.0
        GIT_SUBMODULES_RECURSE ON
)

FetchContent_MakeAvailable(prometheus-cpp)

target_link_libraries(BettingArbitrageEngine PRIVATE prometheus-cpp::pull)

if (WIN32)
    target_link_libraries(BettingArbitrageEngine PRIVATE ws2_32)
endif ()

set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
        google_benchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG v1.9.5
)

FetchContent_MakeAvailable(google_benchmark)

enable_testing()

set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.15.2
)

FetchContent_MakeAvailable(googletest)

add_executable(BettingArbitrageEngineTests
        tests/footballComputeQueueTests.cpp
        src/footballComputeQueue.cpp
        src/orderBook.cpp
        src/mockMetricsRecorder.hpp
)
target_include_directories(BettingArbitrageEngineTests PRIVATE src)
target_link_libraries(BettingArbitrageEngineTests PRIVATE gtest gtest_main)

include(GoogleTest)
gtest_discover_tests(BettingArbitrageEngineTests)
