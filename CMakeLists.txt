cmake_minimum_required(VERSION 3.5)

project(fix-csv VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_TOOLCHAIN_FILE "/home/braquino/vcpkg/scripts/buildsystems/vcpkg.cmake")
message("$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

include(FetchContent)

FetchContent_Declare(
  spdlog
  URL "https://github.com/gabime/spdlog/archive/refs/tags/v1.11.0.tar.gz"
)
FetchContent_Declare(
  fmt
  URL "https://github.com/fmtlib/fmt/archive/refs/tags/9.1.0.tar.gz"
)
FetchContent_Declare(
  googletest
  URL "https://github.com/google/googletest/archive/refs/tags/release-1.12.1.tar.gz"
)
FetchContent_MakeAvailable(spdlog fmt googletest)

include_directories(include)

set(PROJECT_SOURCES
        src/main.cpp
        src/mainwindow.cpp
        src/mainwindow.ui
        src/csvmanager.cpp
        include/csvmanager.h
        include/mainwindow.h
)

qt_add_executable(fix-csv
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

target_link_libraries(fix-csv PRIVATE Qt${QT_VERSION_MAJOR}::Widgets spdlog::spdlog fmt)

set_target_properties(fix-csv PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS fix-csv
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

qt_finalize_executable(fix-csv)

set(TEST_FILES
  tests/test_csvmanager.cpp
)

enable_testing()

add_executable(test-fix-csv ${TEST_FILES} include/csvmanager.h src/csvmanager.cpp)
target_link_libraries(test-fix-csv spdlog::spdlog fmt GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(test-fix-csv)
