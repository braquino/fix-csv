cmake_minimum_required(VERSION 3.5)

project(fix-csv VERSION 0.1.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (WIN32)
  find_library(D3DX11_LIBRARY d3dx11.lib     PATHS     "$ENV{DXSDK_DIR}/Lib/x86" "C:/Program Files (x86)/Microsoft DirectX SDK/Lib/x86")
else()
  find_package(OpenGL REQUIRED)
  find_package(glfw3 REQUIRED)
endif()
find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)
find_package(GTest)
find_package(tinyfiledialogs CONFIG REQUIRED)

include_directories(include imgui)

FILE(GLOB IMGUI_SOURCES
  imgui/*.cpp
  imgui/*.h
  imgui/misc/cpp/*.cpp
  imgui/misc/cpp/*.h
)

set(PROJECT_SOURCES
  src/main.cpp
  src/csvmanager.cpp
  src/csvstatistics.cpp
  src/uimanager.cpp
  src/helperui.cpp
  include/csvmanager.h
  include/csvstatistics.h
  include/baseui.h
  include/uimanager.h
  include/helperui.h
  include/logsinkui.h
)

set(WIN_SOURCES
  src/win32_dx11_ui.cpp
  include/win32_dx11_ui.h
  imgui/backend/imgui_impl_dx11.h
  imgui/backend/imgui_impl_dx11.cpp
  imgui/backend/imgui_impl_win32.h
  imgui/backend/imgui_impl_win32.cpp
)

set(LINUX_SOURCES
  src/opengl_glfw_ui.cpp
  include/opengl_glfw_ui.h
  imgui/backend/imgui_impl_glfw.h
  imgui/backend/imgui_impl_glfw.cpp
  imgui/backend/imgui_impl_opengl3.h
  imgui/backend/imgui_impl_opengl3.cpp
  imgui/backend/imgui_impl_opengl3_loader.h
)

if (WIN32)
  add_executable(fix-csv
    ${PROJECT_SOURCES}
    ${WIN_SOURCES}
    ${IMGUI_SOURCES}
  )
  target_link_libraries(fix-csv PRIVATE ${D3DX11_LIBRARY} spdlog::spdlog fmt::fmt tinyfiledialogs::tinyfiledialogs)
else()
  add_executable(fix-csv
    ${PROJECT_SOURCES}
    ${LINUX_SOURCES}
    ${IMGUI_SOURCES}
  )
  target_link_libraries(fix-csv PRIVATE OpenGL::GL glfw spdlog::spdlog fmt::fmt tinyfiledialogs::tinyfiledialogs)
endif()

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
