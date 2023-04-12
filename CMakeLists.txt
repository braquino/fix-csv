cmake_minimum_required(VERSION 3.5)

project(fix-csv VERSION 0.1.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)
find_package(GTest)
find_package(tinyfiledialogs CONFIG REQUIRED)

include_directories(include imgui)

FILE(GLOB IMGUI_SOURCES
  imgui/*.cpp
  imgui/*.h
  imgui/backend/*.cpp
  imgui/backend/*.h
)

set(PROJECT_SOURCES
  src/main.cpp
  src/csvmanager.cpp
  src/csvstatistics.cpp
  src/opengl_glfw_ui.cpp
  include/csvmanager.h
  include/csvstatistics.h
  include/baseui.h
  include/opengl_glfw_ui.h
)

add_executable(fix-csv
  ${PROJECT_SOURCES}
  ${IMGUI_SOURCES}
)

target_link_libraries(fix-csv PRIVATE OpenGL::GL glfw spdlog::spdlog fmt::fmt tinyfiledialogs::tinyfiledialogs)

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
