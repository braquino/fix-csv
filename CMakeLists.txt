cmake_minimum_required(VERSION 3.0)

project(fix-csv 
  VERSION 0.1
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)

find_package(ftxui)
find_package(fmt)
find_package(spdlog)
set(SPDLOG_FMT_EXTERNAL ON)
find_package(GTest)

include_directories(include)

set(SOURCES
  src/main.cpp
  src/csvmanager.cpp
  src/csvstatistics.cpp
  src/uixevent.cpp
  src/uixscreen.cpp
  src/customftxui.cpp
)

add_executable(fix-csv src/main.cpp ${SOURCES})
target_link_libraries(fix-csv PRIVATE
  ftxui::ftxui fmt::fmt spdlog::spdlog
)

set(TEST_FILES
  tests/test_csvmanager.cpp
  tests/test_csvstatistics.cpp
  src/csvmanager.cpp
  src/csvstatistics.cpp
)

enable_testing()

add_executable(test-fix-csv ${TEST_FILES})
target_link_libraries(test-fix-csv spdlog::spdlog fmt::fmt GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(test-fix-csv)
