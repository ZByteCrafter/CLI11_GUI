cmake_minimum_required(VERSION 3.14)
project(CLI11_GUI VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 选项
option(CLI11_GUI_BUILD_EXAMPLES "Build examples" ON)
option(CLI11_GUI_BUILD_TESTS "Build tests" ON)

# 头文件库
add_library(cli11_gui INTERFACE)
target_include_directories(cli11_gui INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# 依赖管理
include(FetchContent)

FetchContent_Declare(
    cli11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    GIT_TAG v2.3.2
)

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.89.9
)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.8
)

FetchContent_MakeAvailable(cli11 imgui glfw)

# 链接依赖
target_link_libraries(cli11_gui INTERFACE CLI11::CLI11)

# 示例
if(CLI11_GUI_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# 测试
if(CLI11_GUI_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
