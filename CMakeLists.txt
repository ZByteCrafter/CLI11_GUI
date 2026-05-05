cmake_minimum_required(VERSION 3.14)
project(CLI11_GUI VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 选项
option(CLI11_GUI_BUILD_EXAMPLES "Build examples" ON)
option(CLI11_GUI_BUILD_TESTS "Build tests" ON)

# 查找 OpenGL
find_package(OpenGL REQUIRED)

# 依赖管理
include(FetchContent)

FetchContent_Declare(
    cli11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    GIT_TAG v2.4.2
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

# ImGui 后端文件
set(IMGUI_SOURCES
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

# 创建 ImGui 库
add_library(imgui_lib STATIC ${IMGUI_SOURCES})
target_include_directories(imgui_lib PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)
target_link_libraries(imgui_lib PUBLIC glfw)

# 头文件库（INTERFACE 库只能链接其他 INTERFACE 库）
add_library(cli11_gui INTERFACE)
target_include_directories(cli11_gui INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(cli11_gui INTERFACE CLI11::CLI11 OpenGL::GL)

# 创建一个辅助函数来链接所有依赖
function(cli11_gui_link_libraries target)
    target_link_libraries(${target} PRIVATE cli11_gui imgui_lib)
endfunction()

# 示例
if(CLI11_GUI_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# 测试
if(CLI11_GUI_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
