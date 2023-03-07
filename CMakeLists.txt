cmake_minimum_required(VERSION 3.5)

project(fix-csv VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)
find_package(GTest)

include_directories(include)

set(PROJECT_SOURCES
  src/main.cpp
  src/mainwindow.cpp
  src/mainwindow.ui
  src/csvmanager.cpp
  src/csvstatistics.cpp
  include/csvmanager.h
  include/mainwindow.h
  include/csvstatistics.h
)

qt_add_executable(fix-csv
  MANUAL_FINALIZATION
  ${PROJECT_SOURCES}
)

target_link_libraries(fix-csv PRIVATE Qt6::Widgets spdlog::spdlog fmt::fmt)

set_target_properties(fix-csv PROPERTIES
  MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
  MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
  MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
  MACOSX_BUNDLE TRUE
  WIN32_EXECUTABLE TRUE
)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
  message("-> Release mode")
  add_compile_options(-O3)
  target_compile_definitions(fix-csv PRIVATE DEBUG=0)
else()
  message("-> Debug mode")
  target_compile_definitions(fix-csv PRIVATE DEBUG=1)
endif()

install(TARGETS fix-csv
  BUNDLE DESTINATION .
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_finalize_executable(fix-csv)

set(TEST_FILES
  tests/test_csvmanager.cpp
  tests/test_csvstatistics.cpp
  include/csvmanager.h
  include/csvstatistics.h
  src/csvmanager.cpp
  src/csvstatistics.cpp
)

enable_testing()

add_executable(test-fix-csv ${TEST_FILES})
target_link_libraries(test-fix-csv spdlog::spdlog fmt::fmt GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(test-fix-csv)
