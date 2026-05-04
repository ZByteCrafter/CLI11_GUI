# CLI11_GUI Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** 构建一个 header-only C++ 库，为 CLI11 添加 GUI 支持，当无命令行参数时自动弹出 GUI 窗口收集参数

**Architecture:** 单文件 header-only 库，基于 Dear ImGui 实现跨平台 GUI，通过 CLI11 扩展层实现无缝集成

**Tech Stack:** C++14, CMake, CLI11, Dear ImGui, GLFW, OpenGL3

---

## 文件结构

在开始实现之前，以下是项目将创建或修改的文件：

**核心文件：**
- `include/cli11_gui.hpp` - 主头文件，包含所有功能（单文件集成）
- `CMakeLists.txt` - CMake 构建配置

**示例文件：**
- `examples/basic/main.cpp` - 基本使用示例
- `examples/advanced/main.cpp` - 高级使用示例

**测试文件：**
- `tests/test_basic.cpp` - 基本功能测试
- `tests/test_controls.cpp` - 控件生成测试
- `tests/test_layout.cpp` - 布局模式测试
- `tests/test_output.cpp` - 输出显示测试

**文档文件：**
- `README.md` - 项目说明
- `docs/api.md` - API 文档

---

## Task 1: 项目初始化和 CMake 配置

**Files:**
- Create: `CMakeLists.txt`
- Create: `examples/basic/main.cpp`
- Create: `.gitignore`

- [ ] **Step 1: 创建 .gitignore 文件**

```gitignore
# Build directories
build/
cmake-build-*/

# IDE files
.vscode/
.idea/
*.swp
*.swo

# OS files
.DS_Store
Thumbs.db

# Compiled files
*.o
*.obj
*.exe
*.dll
*.so
*.dylib
```

- [ ] **Step 2: 创建基础 CMakeLists.txt**

```cmake
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
```

- [ ] **Step 3: 创建基本示例文件**

```cpp
// examples/basic/main.cpp
#include <iostream>
#include <cli11_gui.hpp>

int main(int argc, char** argv) {
    CLI::App app{"Basic Example"};
    
    int port = 8080;
    app.add_option("--port", port, "Server port")->default_val(port);
    
    std::string host = "localhost";
    app.add_option("--host", host, "Server host")->default_val(host);
    
    bool verbose = false;
    app.add_flag("--verbose", verbose, "Enable verbose output");
    
    // CLI11_GUI 会自动检测是否有命令行参数
    // 无参数时显示 GUI，有参数时正常解析
    CLI11_GUI::run(app);
    
    std::cout << "Host: " << host << std::endl;
    std::cout << "Port: " << port << std::endl;
    std::cout << "Verbose: " << (verbose ? "true" : "false") << std::endl;
    
    return 0;
}
```

- [ ] **Step 4: 创建 examples/CMakeLists.txt**

```cmake
add_executable(basic_example basic/main.cpp)
target_link_libraries(basic_example PRIVATE cli11_gui)
```

- [ ] **Step 5: 提交**

```bash
git add .gitignore CMakeLists.txt examples/
git commit -m "feat: 初始化项目结构和 CMake 配置"
```

---

## Task 2: 创建基础头文件结构

**Files:**
- Create: `include/cli11_gui.hpp`

- [ ] **Step 1: 创建头文件基础结构**

```cpp
// include/cli11_gui.hpp
#pragma once

/**
 * CLI11_GUI - GUI support for CLI11
 * 
 * Copyright (c) 2026 [Your Name]
 * 
 * This software is provided under the BSD 3-Clause License.
 * See the LICENSE file for details.
 */

// 版本信息
#define CLI11_GUI_VERSION_MAJOR 1
#define CLI11_GUI_VERSION_MINOR 0
#define CLI11_GUI_VERSION_PATCH 0
#define CLI11_GUI_VERSION "1.0.0"

// 标准库依赖
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <memory>
#include <iostream>
#include <sstream>

// CLI11 依赖
#include <CLI/CLI.hpp>

namespace CLI11_GUI {

// 前向声明
class GUI;
class Config;
class Simulation;

// 枚举类型
enum class Theme {
    Light,
    Dark,
    System
};

enum class LayoutMode {
    Flat,
    Tabs,
    Wizard,
    Collapsible,
    Auto
};

enum class ControlType {
    Auto,
    Checkbox,
    Toggle,
    Slider,
    InputInt,
    InputFloat,
    InputText,
    Multiline,
    Dropdown,
    Radio,
    List
};

enum class LogLevel {
    Debug,
    Info,
    Warning,
    Error,
    Success
};

enum class ValidationMode {
    OnInput,
    OnSubmit,
    Hybrid,
    Configurable
};

enum class TriggerMode {
    NoArgs,
    InvalidArgs,
    ExplicitFlag,
    Combined
};

// 版本 API
constexpr const char* version() {
    return CLI11_GUI_VERSION;
}

constexpr int version_major() {
    return CLI11_GUI_VERSION_MAJOR;
}

constexpr int version_minor() {
    return CLI11_GUI_VERSION_MINOR;
}

constexpr int version_patch() {
    return CLI11_GUI_VERSION_PATCH;
}

// 声明 API
void run(CLI::App& app);
void run(CLI::App& app, const Config& config);
bool should_show_gui();

// 日志 API
void log_debug(const std::string& message);
void log_info(const std::string& message);
void log_warning(const std::string& message);
void log_error(const std::string& message);
void log_success(const std::string& message);

// 状态 API
void save_state();
void load_state();
void reset_state();
bool has_state();

} // namespace CLI11_GUI
```

- [ ] **Step 2: 验证头文件可以被包含**

创建一个简单的测试文件来验证：

```cpp
// tests/test_include.cpp
#include <cli11_gui.hpp>

int main() {
    return 0;
}
```

- [ ] **Step 3: 提交**

```bash
git add include/cli11_gui.hpp tests/test_include.cpp
git commit -m "feat: 创建基础头文件结构"
```

---

## Task 3: 实现 Config 结构

**Files:**
- Modify: `include/cli11_gui.hpp`

- [ ] **Step 1: 添加 Config 结构定义**

在 `include/cli11_gui.hpp` 的 `namespace CLI11_GUI` 中添加：

```cpp
// 配置结构
struct Config {
    // 窗口配置
    std::string window_title = "";
    int window_width = 800;
    int window_height = 600;
    bool resizable = true;
    bool remember_position = true;
    
    // 布局配置
    LayoutMode layout_mode = LayoutMode::Auto;
    
    // 主题配置
    Theme theme = Theme::System;
    
    // 输出配置
    bool redirect_cout = true;
    bool show_timestamps = true;
    bool auto_scroll = true;
    
    // 验证配置
    ValidationMode validation_mode = ValidationMode::OnSubmit;
    
    // 触发配置
    TriggerMode trigger_mode = TriggerMode::NoArgs;
    
    // 测试配置
    bool simulation_mode = false;
};
```

- [ ] **Step 2: 验证 Config 可以使用**

```cpp
// tests/test_config.cpp
#include <cli11_gui.hpp>
#include <cassert>

int main() {
    CLI11_GUI::Config config;
    assert(config.window_width == 800);
    assert(config.window_height == 600);
    assert(config.theme == CLI11_GUI::Theme::System);
    return 0;
}
```

- [ ] **Step 3: 提交**

```bash
git add include/cli11_gui.hpp tests/test_config.cpp
git commit -m "feat: 添加 Config 配置结构"
```

---

## Task 4: 实现参数检测逻辑

**Files:**
- Modify: `include/cli11_gui.hpp`

- [ ] **Step 1: 添加参数检测函数**

在 `include/cli11_gui.hpp` 的实现部分添加：

```cpp
namespace CLI11_GUI {
namespace detail {

// 检测是否应该显示 GUI
bool should_show_gui_impl(const Config& config) {
    switch (config.trigger_mode) {
        case TriggerMode::NoArgs:
            // 严格无参数
            return false; // 将在 run() 中实现
        case TriggerMode::ExplicitFlag:
            // 需要显式标志
            return false;
        case TriggerMode::Combined:
            // 组合模式
            return false;
        default:
            return false;
    }
}

// 检测是否有命令行参数
bool has_command_line_args(int argc, char** argv) {
    return argc > 1;
}

} // namespace detail
} // namespace CLI11_GUI
```

- [ ] **Step 2: 实现基本的 run 函数**

```cpp
namespace CLI11_GUI {

void run(CLI::App& app) {
    Config config;
    run(app, config);
}

void run(CLI::App& app, const Config& config) {
    // 获取命令行参数
    int argc = 0;
    char** argv = nullptr;
    
    // 检测是否应该显示 GUI
    bool show_gui = false;
    
    if (config.trigger_mode == TriggerMode::NoArgs) {
        // 无参数时显示 GUI
        show_gui = !detail::has_command_line_args(argc, argv);
    }
    
    if (show_gui) {
        // 显示 GUI
        // TODO: 实现 GUI 显示
    } else {
        // 正常解析命令行
        try {
            app.parse(argc, argv);
        } catch (const CLI::ParseError& e) {
            exit(app.exit(e));
        }
    }
}

bool should_show_gui() {
    Config config;
    return detail::should_show_gui_impl(config);
}

} // namespace CLI11_GUI
```

- [ ] **Step 3: 测试参数检测**

```cpp
// tests/test_detection.cpp
#include <cli11_gui.hpp>
#include <cassert>

int main() {
    // 测试无参数情况
    assert(CLI11_GUI::should_show_gui() == true);
    
    return 0;
}
```

- [ ] **Step 4: 提交**

```bash
git add include/cli11_gui.hpp tests/test_detection.cpp
git commit -m "feat: 实现参数检测逻辑"
```

---

## Task 5: 集成 Dear ImGui 依赖

**Files:**
- Modify: `CMakeLists.txt`
- Create: `src/imgui_impl.cpp`

- [ ] **Step 1: 更新 CMakeLists.txt 添加 ImGui 后端**

```cmake
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

# 链接到 cli11_gui
target_link_libraries(cli11_gui INTERFACE imgui_lib)
```

- [ ] **Step 2: 创建 ImGui 初始化封装**

```cpp
// src/imgui_impl.cpp (这个文件不会直接使用，代码会合并到头文件)
// 但我们需要在头文件中包含 ImGui 头文件

// 在 include/cli11_gui.hpp 中添加
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
```

- [ ] **Step 3: 测试编译**

```bash
mkdir -p build && cd build
cmake ..
make
```

- [ ] **Step 4: 提交**

```bash
git add CMakeLists.txt include/cli11_gui.hpp
git commit -m "feat: 集成 Dear ImGui 依赖"
```

---

## Task 6: 实现 GUI 窗口基础框架

**Files:**
- Modify: `include/cli11_gui.hpp`

- [ ] **Step 1: 添加 GUI 类声明**

在 `include/cli11_gui.hpp` 的 `namespace CLI11_GUI` 中添加：

```cpp
// GUI 窗口类
class GUI {
public:
    GUI(const CLI::App& app, const Config& config);
    ~GUI();
    
    // 显示 GUI 并返回是否成功
    bool show();
    
    // 获取收集的参数
    std::vector<std::string> get_args() const;
    
private:
    // 初始化 ImGui
    bool initialize();
    
    // 清理资源
    void cleanup();
    
    // 渲染 GUI
    void render();
    
    // 渲染控件
    void render_controls();
    
    // 渲染输出框
    void render_output();
    
    // 渲染按钮
    void render_buttons();
    
    const CLI::App& app_;
    Config config_;
    GLFWwindow* window_ = nullptr;
    bool initialized_ = false;
    bool should_run_ = false;
    std::vector<std::string> args_;
};
```

- [ ] **Step 2: 实现 GUI 构造函数和析构函数**

```cpp
namespace CLI11_GUI {

GUI::GUI(const CLI::App& app, const Config& config)
    : app_(app), config_(config) {
    // 延迟初始化
}

GUI::~GUI() {
    cleanup();
}

bool GUI::initialize() {
    // 初始化 GLFW
    if (!glfwInit()) {
        log_error("Failed to initialize GLFW");
        return false;
    }
    
    // 创建窗口
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, config_.resizable ? GLFW_TRUE : GLFW_FALSE);
    
    std::string title = config_.window_title.empty() ? "CLI11_GUI" : config_.window_title;
    window_ = glfwCreateWindow(
        config_.window_width,
        config_.window_height,
        title.c_str(),
        nullptr,
        nullptr
    );
    
    if (!window_) {
        log_error("Failed to create GLFW window");
        glfwTerminate();
        return false;
    }
    
    glfwMakeContextCurrent(window_);
    glfwSwapInterval(1);
    
    // 初始化 ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    
    // 设置样式
    ImGui::StyleColorsDark();
    
    // 初始化后端
    ImGui_ImplGlfw_InitForOpenGL(window_, true);
    ImGui_ImplOpenGL3_Init("#version 130");
    
    initialized_ = true;
    return true;
}

void GUI::cleanup() {
    if (initialized_) {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        
        if (window_) {
            glfwDestroyWindow(window_);
        }
        
        glfwTerminate();
        initialized_ = false;
    }
}

bool GUI::show() {
    if (!initialize()) {
        return false;
    }
    
    // 主循环
    while (!glfwWindowShouldClose(window_)) {
        glfwPollEvents();
        
        // 开始新帧
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        // 渲染 GUI
        render();
        
        // 渲染
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window_, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        glfwSwapBuffers(window_);
        
        // 检查是否应该运行
        if (should_run_) {
            break;
        }
    }
    
    return should_run_;
}

std::vector<std::string> GUI::get_args() const {
    return args_;
}

void GUI::render() {
    // 渲染主窗口
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin("CLI11_GUI", nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse
    );
    
    // 渲染控件
    render_controls();
    
    // 渲染输出框
    render_output();
    
    // 渲染按钮
    render_buttons();
    
    ImGui::End();
}

void GUI::render_controls() {
    // TODO: 实现控件渲染
    ImGui::Text("Controls will be here");
}

void GUI::render_output() {
    // TODO: 实现输出框渲染
    ImGui::Text("Output will be here");
}

void GUI::render_buttons() {
    ImGui::Separator();
    
    if (ImGui::Button("Run", ImVec2(120, 0))) {
        should_run_ = true;
    }
    
    ImGui::SameLine();
    
    if (ImGui::Button("Cancel", ImVec2(120, 0))) {
        glfwSetWindowShouldClose(window_, true);
    }
}

} // namespace CLI11_GUI
```

- [ ] **Step 3: 更新 run 函数使用 GUI**

```cpp
void run(CLI::App& app, const Config& config) {
    // 获取命令行参数
    int argc = 0;
    char** argv = nullptr;
    
    // 检测是否应该显示 GUI
    bool show_gui = false;
    
    if (config.trigger_mode == TriggerMode::NoArgs) {
        // 无参数时显示 GUI
        show_gui = !detail::has_command_line_args(argc, argv);
    }
    
    if (show_gui) {
        // 显示 GUI
        GUI gui(app, config);
        if (gui.show()) {
            auto args = gui.get_args();
            try {
                app.parse(args);
            } catch (const CLI::ParseError& e) {
                exit(app.exit(e));
            }
        } else {
            // 用户取消
            exit(0);
        }
    } else {
        // 正常解析命令行
        try {
            app.parse(argc, argv);
        } catch (const CLI::ParseError& e) {
            exit(app.exit(e));
        }
    }
}
```

- [ ] **Step 4: 测试 GUI 窗口**

```bash
cd build
cmake ..
make
./examples/basic_example
```

- [ ] **Step 5: 提交**

```bash
git add include/cli11_gui.hpp
git commit -m "feat: 实现 GUI 窗口基础框架"
```

---

## Task 7: 实现控件生成器

**Files:**
- Modify: `include/cli11_gui.hpp`

- [ ] **Step 1: 添加控件生成器类**

```cpp
namespace CLI11_GUI {
namespace detail {

// 控件生成器
class ControlGenerator {
public:
    ControlGenerator(const CLI::App& app, Config& config);
    
    // 渲染所有控件
    void render();
    
    // 获取收集的参数
    std::vector<std::string> get_args() const;
    
private:
    // 渲染单个选项
    void render_option(const CLI::Option* option);
    
    // 渲染标志
    void render_flag(const CLI::Option* option);
    
    // 渲染文本输入
    void render_text_input(const CLI::Option* option);
    
    // 渲染数字输入
    void render_number_input(const CLI::Option* option);
    
    // 渲染复选框
    void render_checkbox(const CLI::Option* option);
    
    // 渲染滑块
    void render_slider(const CLI::Option* option);
    
    // 渲染下拉框
    void render_dropdown(const CLI::Option* option);
    
    const CLI::App& app_;
    Config& config_;
    std::map<std::string, std::string> values_;
    std::map<std::string, bool> flags_;
};

} // namespace detail
} // namespace CLI11_GUI
```

- [ ] **Step 2: 实现控件生成器**

```cpp
namespace CLI11_GUI {
namespace detail {

ControlGenerator::ControlGenerator(const CLI::App& app, Config& config)
    : app_(app), config_(config) {
    // 初始化默认值
    for (const auto& option : app.get_options()) {
        if (option->get_expected() == 0) {
            // 标志
            flags_[option->get_name()] = false;
        } else {
            // 选项
            auto default_val = option->get_default_str();
            if (!default_val.empty()) {
                values_[option->get_name()] = default_val;
            }
        }
    }
}

void ControlGenerator::render() {
    for (const auto& option : app_.get_options()) {
        render_option(option.get());
    }
}

void ControlGenerator::render_option(const CLI::Option* option) {
    // 跳过帮助选项
    if (option->get_name() == "--help" || option->get_name() == "-h") {
        return;
    }
    
    // 根据类型渲染
    if (option->get_expected() == 0) {
        render_flag(option);
    } else {
        // 根据值类型选择控件
        auto type_name = option->get_type_name();
        
        if (type_name == "int" || type_name == "float" || type_name == "double") {
            render_number_input(option);
        } else if (type_name == "bool") {
            render_checkbox(option);
        } else {
            render_text_input(option);
        }
    }
}

void ControlGenerator::render_flag(const CLI::Option* option) {
    auto name = option->get_name();
    bool value = flags_[name];
    
    if (ImGui::Checkbox(name.c_str(), &value)) {
        flags_[name] = value;
    }
    
    // 显示帮助
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("%s", option->get_description().c_str());
    }
}

void ControlGenerator::render_text_input(const CLI::Option* option) {
    auto name = option->get_name();
    auto& value = values_[name];
    
    char buffer[256];
    strncpy(buffer, value.c_str(), sizeof(buffer));
    
    if (ImGui::InputText(name.c_str(), buffer, sizeof(buffer))) {
        value = buffer;
    }
    
    // 显示帮助
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("%s", option->get_description().c_str());
    }
}

void ControlGenerator::render_number_input(const CLI::Option* option) {
    auto name = option->get_name();
    auto& value = values_[name];
    
    // 尝试解析为数字
    try {
        if (option->get_type_name() == "int") {
            int int_val = std::stoi(value);
            if (ImGui::InputInt(name.c_str(), &int_val)) {
                value = std::to_string(int_val);
            }
        } else {
            float float_val = std::stof(value);
            if (ImGui::InputFloat(name.c_str(), &float_val)) {
                value = std::to_string(float_val);
            }
        }
    } catch (...) {
        // 回退到文本输入
        render_text_input(option);
    }
    
    // 显示帮助
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("%s", option->get_description().c_str());
    }
}

void ControlGenerator::render_checkbox(const CLI::Option* option) {
    auto name = option->get_name();
    auto& value = values_[name];
    
    bool bool_val = (value == "true" || value == "1");
    if (ImGui::Checkbox(name.c_str(), &bool_val)) {
        value = bool_val ? "true" : "false";
    }
    
    // 显示帮助
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("%s", option->get_description().c_str());
    }
}

void ControlGenerator::render_slider(const CLI::Option* option) {
    // TODO: 实现滑块控件
}

void ControlGenerator::render_dropdown(const CLI::Option* option) {
    // TODO: 实现下拉框控件
}

std::vector<std::string> ControlGenerator::get_args() const {
    std::vector<std::string> args;
    
    for (const auto& pair : values_) {
        if (!pair.second.empty()) {
            args.push_back(pair.first);
            args.push_back(pair.second);
        }
    }
    
    for (const auto& pair : flags_) {
        if (pair.second) {
            args.push_back(pair.first);
        }
    }
    
    return args;
}

} // namespace detail
} // namespace CLI11_GUI
```

- [ ] **Step 3: 在 GUI 类中使用控件生成器**

```cpp
class GUI {
public:
    // ...
    
private:
    // ...
    std::unique_ptr<detail::ControlGenerator> control_generator_;
};

// 在 initialize() 中
bool GUI::initialize() {
    // ...
    control_generator_ = std::make_unique<detail::ControlGenerator>(app_, config_);
    // ...
}

// 在 render_controls() 中
void GUI::render_controls() {
    control_generator_->render();
}

// 在 get_args() 中
std::vector<std::string> GUI::get_args() const {
    return control_generator_->get_args();
}
```

- [ ] **Step 4: 测试控件生成**

```bash
cd build
cmake ..
make
./examples/basic_example
```

- [ ] **Step 5: 提交**

```bash
git add include/cli11_gui.hpp
git commit -m "feat: 实现控件生成器"
```

---

## Task 8: 实现输出显示框

**Files:**
- Modify: `include/cli11_gui.hpp`

- [ ] **Step 1: 添加输出缓冲区类**

```cpp
namespace CLI11_GUI {
namespace detail {

// 输出缓冲区
class OutputBuffer {
public:
    OutputBuffer();
    ~OutputBuffer();
    
    // 添加日志
    void add_log(LogLevel level, const std::string& message);
    
    // 清空日志
    void clear();
    
    // 渲染输出框
    void render();
    
    // 获取日志
    const std::vector<std::pair<LogLevel, std::string>>& get_logs() const;
    
private:
    std::vector<std::pair<LogLevel, std::string>> logs_;
    bool auto_scroll_ = true;
    bool show_timestamps_ = true;
};

} // namespace detail
} // namespace CLI11_GUI
```

- [ ] **Step 2: 实现输出缓冲区**

```cpp
namespace CLI11_GUI {
namespace detail {

OutputBuffer::OutputBuffer() = default;
OutputBuffer::~OutputBuffer() = default;

void OutputBuffer::add_log(LogLevel level, const std::string& message) {
    logs_.push_back({level, message});
    
    // 自动滚动
    if (auto_scroll_) {
        // 将在渲染时处理
    }
}

void OutputBuffer::clear() {
    logs_.clear();
}

void OutputBuffer::render() {
    ImGui::Separator();
    ImGui::Text("Output:");
    
    // 控制按钮
    if (ImGui::Button("Clear")) {
        clear();
    }
    ImGui::SameLine();
    ImGui::Checkbox("Auto-scroll", &auto_scroll_);
    ImGui::SameLine();
    ImGui::Checkbox("Timestamps", &show_timestamps_);
    
    // 输出区域
    ImGui::BeginChild("OutputRegion", ImVec2(0, 200), true);
    
    for (const auto& log : logs_) {
        // 设置颜色
        ImVec4 color;
        switch (log.first) {
            case LogLevel::Debug:
                color = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
                break;
            case LogLevel::Info:
                color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
                break;
            case LogLevel::Warning:
                color = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
                break;
            case LogLevel::Error:
                color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
                break;
            case LogLevel::Success:
                color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
                break;
        }
        
        ImGui::PushStyleColor(ImGuiCol_Text, color);
        ImGui::TextUnformatted(log.second.c_str());
        ImGui::PopStyleColor();
    }
    
    // 自动滚动
    if (auto_scroll_ && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
        ImGui::SetScrollHereY(1.0f);
    }
    
    ImGui::EndChild();
}

const std::vector<std::pair<LogLevel, std::string>>& OutputBuffer::get_logs() const {
    return logs_;
}

} // namespace detail
} // namespace CLI11_GUI
```

- [ ] **Step 3: 实现 cout 重定向**

```cpp
namespace CLI11_GUI {
namespace detail {

// 重定向 cout
class CoutRedirect {
public:
    CoutRedirect(OutputBuffer& buffer);
    ~CoutRedirect();
    
private:
    OutputBuffer& buffer_;
    std::streambuf* original_cout_;
    std::streambuf* original_cerr_;
    std::ostringstream captured_;
};

} // namespace detail
} // namespace CLI11_GUI

// 实现
namespace CLI11_GUI {
namespace detail {

CoutRedirect::CoutRedirect(OutputBuffer& buffer)
    : buffer_(buffer), original_cout_(std::cout.rdbuf()), original_cerr_(std::cerr.rdbuf()) {
    // 重定向 cout
    std::cout.rdbuf(captured_.rdbuf());
    std::cerr.rdbuf(captured_.rdbuf());
}

CoutRedirect::~CoutRedirect() {
    // 恢复原始缓冲区
    std::cout.rdbuf(original_cout_);
    std::cerr.rdbuf(original_cerr_);
    
    // 输出捕获的内容
    auto content = captured_.str();
    if (!content.empty()) {
        buffer_.add_log(LogLevel::Info, content);
    }
}

} // namespace detail
} // namespace CLI11_GUI
```

- [ ] **Step 4: 在 GUI 类中使用输出缓冲区**

```cpp
class GUI {
public:
    // ...
    
private:
    // ...
    std::unique_ptr<detail::OutputBuffer> output_buffer_;
    std::unique_ptr<detail::CoutRedirect> cout_redirect_;
};

// 在 initialize() 中
bool GUI::initialize() {
    // ...
    output_buffer_ = std::make_unique<detail::OutputBuffer>();
    
    if (config_.redirect_cout) {
        cout_redirect_ = std::make_unique<detail::CoutRedirect>(*output_buffer_);
    }
    // ...
}

// 在 render_output() 中
void GUI::render_output() {
    output_buffer_->render();
}
```

- [ ] **Step 5: 实现日志 API**

```cpp
namespace CLI11_GUI {

// 全局输出缓冲区
static std::unique_ptr<detail::OutputBuffer> g_output_buffer;

void log_debug(const std::string& message) {
    if (g_output_buffer) {
        g_output_buffer->add_log(LogLevel::Debug, message);
    }
}

void log_info(const std::string& message) {
    if (g_output_buffer) {
        g_output_buffer->add_log(LogLevel::Info, message);
    }
}

void log_warning(const std::string& message) {
    if (g_output_buffer) {
        g_output_buffer->add_log(LogLevel::Warning, message);
    }
}

void log_error(const std::string& message) {
    if (g_output_buffer) {
        g_output_buffer->add_log(LogLevel::Error, message);
    }
}

void log_success(const std::string& message) {
    if (g_output_buffer) {
        g_output_buffer->add_log(LogLevel::Success, message);
    }
}

} // namespace CLI11_GUI
```

- [ ] **Step 6: 测试输出显示**

```bash
cd build
cmake ..
make
./examples/basic_example
```

- [ ] **Step 7: 提交**

```bash
git add include/cli11_gui.hpp
git commit -m "feat: 实现输出显示框"
```

---

## Task 9: 实现主题系统

**Files:**
- Modify: `include/cli11_gui.hpp`

- [ ] **Step 1: 添加主题管理器类**

```cpp
namespace CLI11_GUI {
namespace detail {

// 主题管理器
class ThemeManager {
public:
    ThemeManager();
    
    // 应用主题
    void apply_theme(Theme theme);
    
    // 获取当前主题
    Theme get_current_theme() const;
    
private:
    // 应用浅色主题
    void apply_light_theme();
    
    // 应用深色主题
    void apply_dark_theme();
    
    // 应用系统主题
    void apply_system_theme();
    
    // 检测系统主题
    bool is_system_dark_mode() const;
    
    Theme current_theme_ = Theme::System;
};

} // namespace detail
} // namespace CLI11_GUI
```

- [ ] **Step 2: 实现主题管理器**

```cpp
namespace CLI11_GUI {
namespace detail {

ThemeManager::ThemeManager() = default;

void ThemeManager::apply_theme(Theme theme) {
    current_theme_ = theme;
    
    switch (theme) {
        case Theme::Light:
            apply_light_theme();
            break;
        case Theme::Dark:
            apply_dark_theme();
            break;
        case Theme::System:
            apply_system_theme();
            break;
    }
}

Theme ThemeManager::get_current_theme() const {
    return current_theme_;
}

void ThemeManager::apply_light_theme() {
    ImGui::StyleColorsLight();
    
    // 自定义样式
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 4.0f;
    style.FrameRounding = 2.0f;
    style.GrabRounding = 2.0f;
}

void ThemeManager::apply_dark_theme() {
    ImGui::StyleColorsDark();
    
    // 自定义样式
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 4.0f;
    style.FrameRounding = 2.0f;
    style.GrabRounding = 2.0f;
}

void ThemeManager::apply_system_theme() {
    if (is_system_dark_mode()) {
        apply_dark_theme();
    } else {
        apply_light_theme();
    }
}

bool ThemeManager::is_system_dark_mode() const {
#ifdef _WIN32
    // Windows: 检查注册表
    // TODO: 实现 Windows 深色模式检测
    return false;
#elif __APPLE__
    // macOS: 检查系统偏好设置
    // TODO: 实现 macOS 深色模式检测
    return false;
#else
    // Linux: 检查 GTK 主题
    // TODO: 实现 Linux 深色模式检测
    return false;
#endif
}

} // namespace detail
} // namespace CLI11_GUI
```

- [ ] **Step 3: 在 GUI 类中使用主题管理器**

```cpp
class GUI {
public:
    // ...
    
private:
    // ...
    std::unique_ptr<detail::ThemeManager> theme_manager_;
};

// 在 initialize() 中
bool GUI::initialize() {
    // ...
    theme_manager_ = std::make_unique<detail::ThemeManager>();
    theme_manager_->apply_theme(config_.theme);
    // ...
}
```

- [ ] **Step 4: 测试主题切换**

```bash
cd build
cmake ..
make
./examples/basic_example
```

- [ ] **Step 5: 提交**

```bash
git add include/cli11_gui.hpp
git commit -m "feat: 实现主题系统"
```

---

## Task 10: 实现状态管理

**Files:**
- Modify: `include/cli11_gui.hpp`

- [ ] **Step 1: 添加状态管理器类**

```cpp
namespace CLI11_GUI {
namespace detail {

// 状态管理器
class StateManager {
public:
    StateManager();
    
    // 保存状态
    void save();
    
    // 加载状态
    void load();
    
    // 重置状态
    void reset();
    
    // 检查状态是否存在
    bool exists() const;
    
    // 获取状态
    const WindowState& get_state() const;
    
    // 设置状态
    void set_state(const WindowState& state);
    
private:
    // 获取状态文件路径
    std::string get_state_file_path() const;
    
    // 保存到文件
    void save_to_file(const std::string& path);
    
    // 从文件加载
    void load_from_file(const std::string& path);
    
    WindowState state_;
    bool has_state_ = false;
};

} // namespace detail
} // namespace CLI11_GUI
```

- [ ] **Step 2: 实现状态管理器**

```cpp
namespace CLI11_GUI {
namespace detail {

StateManager::StateManager() = default;

void StateManager::save() {
    auto path = get_state_file_path();
    save_to_file(path);
}

void StateManager::load() {
    auto path = get_state_file_path();
    if (std::ifstream(path).good()) {
        load_from_file(path);
        has_state_ = true;
    }
}

void StateManager::reset() {
    state_ = WindowState{};
    has_state_ = false;
    
    // 删除状态文件
    auto path = get_state_file_path();
    std::remove(path.c_str());
}

bool StateManager::exists() const {
    return has_state_;
}

const WindowState& StateManager::get_state() const {
    return state_;
}

void StateManager::set_state(const WindowState& state) {
    state_ = state;
}

std::string StateManager::get_state_file_path() const {
#ifdef _WIN32
    char* appdata = nullptr;
    size_t len = 0;
    _dupenv_s(&appdata, &len, "APPDATA");
    std::string path = appdata ? appdata : "";
    free(appdata);
    return path + "\\CLI11_GUI\\state.ini";
#elif __APPLE__
    char* home = getenv("HOME");
    std::string path = home ? home : "";
    return path + "/Library/Application Support/CLI11_GUI/state.ini";
#else
    char* home = getenv("HOME");
    std::string path = home ? home : "";
    return path + "/.config/CLI11_GUI/state.ini";
#endif
}

void StateManager::save_to_file(const std::string& path) {
    // 创建目录
    auto dir = path.substr(0, path.find_last_of("/\\"));
    // TODO: 创建目录
    
    // 保存到文件
    std::ofstream file(path);
    if (file.is_open()) {
        file << "[window]\n";
        file << "x=" << state_.x << "\n";
        file << "y=" << state_.y << "\n";
        file << "width=" << state_.width << "\n";
        file << "height=" << state_.height << "\n";
        file << "monitor=" << state_.monitor_index << "\n";
        file << "maximized=" << (state_.maximized ? "true" : "false") << "\n";
    }
}

void StateManager::load_from_file(const std::string& path) {
    std::ifstream file(path);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            // 简单的 INI 解析
            if (line.find("x=") == 0) {
                state_.x = std::stoi(line.substr(2));
            } else if (line.find("y=") == 0) {
                state_.y = std::stoi(line.substr(2));
            } else if (line.find("width=") == 0) {
                state_.width = std::stoi(line.substr(6));
            } else if (line.find("height=") == 0) {
                state_.height = std::stoi(line.substr(7));
            }
        }
    }
}

} // namespace detail
} // namespace CLI11_GUI
```

- [ ] **Step 3: 实现状态 API**

```cpp
namespace CLI11_GUI {

// 全局状态管理器
static std::unique_ptr<detail::StateManager> g_state_manager;

void save_state() {
    if (g_state_manager) {
        g_state_manager->save();
    }
}

void load_state() {
    if (g_state_manager) {
        g_state_manager->load();
    }
}

void reset_state() {
    if (g_state_manager) {
        g_state_manager->reset();
    }
}

bool has_state() {
    if (g_state_manager) {
        return g_state_manager->exists();
    }
    return false;
}

} // namespace CLI11_GUI
```

- [ ] **Step 4: 在 GUI 类中使用状态管理器**

```cpp
class GUI {
public:
    // ...
    
private:
    // ...
    std::unique_ptr<detail::StateManager> state_manager_;
};

// 在 initialize() 中
bool GUI::initialize() {
    // ...
    state_manager_ = std::make_unique<detail::StateManager>();
    state_manager_->load();
    
    // 恢复窗口位置
    if (state_manager_->exists() && config_.remember_position) {
        auto state = state_manager_->get_state();
        glfwSetWindowPos(window_, state.x, state.y);
        glfwSetWindowSize(window_, state.width, state.height);
    }
    // ...
}

// 在 cleanup() 中
void GUI::cleanup() {
    // 保存窗口状态
    if (state_manager_ && config_.remember_position) {
        WindowState state;
        glfwGetWindowPos(window_, &state.x, &state.y);
        glfwGetWindowSize(window_, &state.width, &state.height);
        state_manager_->set_state(state);
        state_manager_->save();
    }
    
    // ... 其他清理
}
```

- [ ] **Step 5: 测试状态管理**

```bash
cd build
cmake ..
make
./examples/basic_example
# 调整窗口大小和位置
# 关闭程序
# 重新运行，检查窗口位置是否恢复
```

- [ ] **Step 6: 提交**

```bash
git add include/cli11_gui.hpp
git commit -m "feat: 实现状态管理"
```

---

## Task 11: 实现布局管理器

**Files:**
- Modify: `include/cli11_gui.hpp`

- [ ] **Step 1: 添加布局管理器类**

```cpp
namespace CLI11_GUI {
namespace detail {

// 布局管理器
class LayoutManager {
public:
    LayoutManager(const CLI::App& app, Config& config);
    
    // 选择布局模式
    LayoutMode select_layout_mode();
    
    // 渲染布局
    void render();
    
private:
    // 渲染平铺布局
    void render_flat();
    
    // 渲染标签页布局
    void render_tabs();
    
    // 渲染向导布局
    void render_wizard();
    
    // 渲染可折叠布局
    void render_collapsible();
    
    const CLI::App& app_;
    Config& config_;
    int wizard_step_ = 0;
};

} // namespace detail
} // namespace CLI11_GUI
```

- [ ] **Step 2: 实现布局管理器**

```cpp
namespace CLI11_GUI {
namespace detail {

LayoutManager::LayoutManager(const CLI::App& app, Config& config)
    : app_(app), config_(config) {}

LayoutMode LayoutManager::select_layout_mode() {
    if (config_.layout_mode != LayoutMode::Auto) {
        return config_.layout_mode;
    }
    
    // 自动选择布局模式
    auto subcommands = app_.get_subcommands({});
    
    if (subcommands.empty()) {
        return LayoutMode::Flat;
    } else if (subcommands.size() <= 3) {
        return LayoutMode::Tabs;
    } else {
        return LayoutMode::Collapsible;
    }
}

void LayoutManager::render() {
    auto mode = select_layout_mode();
    
    switch (mode) {
        case LayoutMode::Flat:
            render_flat();
            break;
        case LayoutMode::Tabs:
            render_tabs();
            break;
        case LayoutMode::Wizard:
            render_wizard();
            break;
        case LayoutMode::Collapsible:
            render_collapsible();
            break;
        default:
            render_flat();
            break;
    }
}

void LayoutManager::render_flat() {
    // 平铺布局：所有选项在一个窗口中
    // 由 ControlGenerator 处理
}

void LayoutManager::render_tabs() {
    // 标签页布局
    auto subcommands = app_.get_subcommands({});
    
    if (ImGui::BeginTabBar("Subcommands")) {
        // 全局选项标签页
        if (ImGui::BeginTabItem("Global")) {
            // 渲染全局选项
            ImGui::EndTabItem();
        }
        
        // 子命令标签页
        for (size_t i = 0; i < subcommands.size(); ++i) {
            auto subcmd = subcommands[i];
            if (ImGui::BeginTabItem(subcmd->get_name().c_str())) {
                // 渲染子命令选项
                ImGui::EndTabItem();
            }
        }
        
        ImGui::EndTabBar();
    }
}

void LayoutManager::render_wizard() {
    // 向导布局
    auto subcommands = app_.get_subcommands({});
    
    // 步骤指示
    ImGui::Text("Step %d of %zu", wizard_step_ + 1, subcommands.size() + 1);
    ImGui::Separator();
    
    // 渲染当前步骤
    if (wizard_step_ == 0) {
        // 全局选项
        ImGui::Text("Global Options");
    } else if (wizard_step_ <= subcommands.size()) {
        // 子命令选项
        auto subcmd = subcommands[wizard_step_ - 1];
        ImGui::Text("Subcommand: %s", subcmd->get_name().c_str());
    }
    
    ImGui::Separator();
    
    // 导航按钮
    if (wizard_step_ > 0) {
        if (ImGui::Button("Previous")) {
            wizard_step_--;
        }
        ImGui::SameLine();
    }
    
    if (wizard_step_ < subcommands.size()) {
        if (ImGui::Button("Next")) {
            wizard_step_++;
        }
    }
}

void LayoutManager::render_collapsible() {
    // 可折叠布局
    auto subcommands = app_.get_subcommands({});
    
    // 全局选项
    if (ImGui::CollapsingHeader("Global Options", ImGuiTreeNodeFlags_DefaultOpen)) {
        // 渲染全局选项
    }
    
    // 子命令选项
    for (auto subcmd : subcommands) {
        if (ImGui::CollapsingHeader(subcmd->get_name().c_str())) {
            // 渲染子命令选项
        }
    }
}

} // namespace detail
} // namespace CLI11_GUI
```

- [ ] **Step 3: 在 GUI 类中使用布局管理器**

```cpp
class GUI {
public:
    // ...
    
private:
    // ...
    std::unique_ptr<detail::LayoutManager> layout_manager_;
};

// 在 initialize() 中
bool GUI::initialize() {
    // ...
    layout_manager_ = std::make_unique<detail::LayoutManager>(app_, config_);
    // ...
}

// 在 render_controls() 中
void GUI::render_controls() {
    layout_manager_->render();
    control_generator_->render();
}
```

- [ ] **Step 4: 测试布局模式**

```bash
cd build
cmake ..
make
./examples/basic_example
```

- [ ] **Step 5: 提交**

```bash
git add include/cli11_gui.hpp
git commit -m "feat: 实现布局管理器"
```

---

## Task 12: 创建完整示例

**Files:**
- Modify: `examples/basic/main.cpp`
- Create: `examples/advanced/main.cpp`

- [ ] **Step 1: 更新基本示例**

```cpp
// examples/basic/main.cpp
#include <iostream>
#include <cli11_gui.hpp>

int main(int argc, char** argv) {
    CLI::App app{"Basic Example"};
    
    int port = 8080;
    app.add_option("--port", port, "Server port")
       ->default_val(port)
       ->check(CLI::Range(1024, 65535));
    
    std::string host = "localhost";
    app.add_option("--host", host, "Server host")
       ->default_val(host);
    
    bool verbose = false;
    app.add_flag("--verbose,-v", verbose, "Enable verbose output");
    
    // 配置 GUI
    CLI11_GUI::Config config;
    config.window_title = "Basic Example";
    config.theme = CLI11_GUI::Theme::Dark;
    
    // 运行
    CLI11_GUI::run(app, config);
    
    // 输出结果
    std::cout << "Host: " << host << std::endl;
    std::cout << "Port: " << port << std::endl;
    std::cout << "Verbose: " << (verbose ? "true" : "false") << std::endl;
    
    // 模拟一些输出
    CLI11_GUI::log_info("Starting server...");
    CLI11_GUI::log_success("Server started successfully");
    
    return 0;
}
```

- [ ] **Step 2: 创建高级示例**

```cpp
// examples/advanced/main.cpp
#include <iostream>
#include <cli11_gui.hpp>

int main(int argc, char** argv) {
    CLI::App app{"Advanced Example"};
    
    // 全局选项
    std::string config_file = "config.ini";
    app.add_option("--config,-c", config_file, "Configuration file")
       ->default_val(config_file);
    
    bool debug = false;
    app.add_flag("--debug,-d", debug, "Enable debug mode");
    
    // 子命令：serve
    auto serve_cmd = app.add_subcommand("serve", "Start the server");
    
    int port = 8080;
    serve_cmd->add_option("--port,-p", port, "Server port")
              ->default_val(port)
              ->check(CLI::Range(1024, 65535));
    
    std::string host = "0.0.0.0";
    serve_cmd->add_option("--host,-h", host, "Server host")
              ->default_val(host);
    
    int threads = 4;
    serve_cmd->add_option("--threads,-t", threads, "Number of threads")
              ->default_val(threads)
              ->check(CLI::Range(1, 16));
    
    // 子命令：db
    auto db_cmd = app.add_subcommand("db", "Database operations");
    
    std::string db_host = "localhost";
    db_cmd->add_option("--host", db_host, "Database host")
          ->default_val(db_host);
    
    int db_port = 5432;
    db_cmd->add_option("--port", db_port, "Database port")
          ->default_val(db_port)
          ->check(CLI::Range(1024, 65535));
    
    std::string db_name = "mydb";
    db_cmd->add_option("--name,-n", db_name, "Database name")
          ->default_val(db_name);
    
    // 配置 GUI
    CLI11_GUI::Config config;
    config.window_title = "Advanced Example";
    config.theme = CLI11_GUI::Theme::Dark;
    config.layout_mode = CLI11_GUI::LayoutMode::Tabs;
    
    // 运行
    CLI11_GUI::run(app, config);
    
    // 输出结果
    CLI11_GUI::log_info("Configuration:");
    CLI11_GUI::log_info("  Config file: " + config_file);
    CLI11_GUI::log_info("  Debug mode: " + std::string(debug ? "true" : "false"));
    
    if (serve_cmd->parsed()) {
        CLI11_GUI::log_info("Server configuration:");
        CLI11_GUI::log_info("  Host: " + host);
        CLI11_GUI::log_info("  Port: " + std::to_string(port));
        CLI11_GUI::log_info("  Threads: " + std::to_string(threads));
    }
    
    if (db_cmd->parsed()) {
        CLI11_GUI::log_info("Database configuration:");
        CLI11_GUI::log_info("  Host: " + db_host);
        CLI11_GUI::log_info("  Port: " + std::to_string(db_port));
        CLI11_GUI::log_info("  Name: " + db_name);
    }
    
    return 0;
}
```

- [ ] **Step 3: 更新 examples/CMakeLists.txt**

```cmake
add_executable(basic_example basic/main.cpp)
target_link_libraries(basic_example PRIVATE cli11_gui)

add_executable(advanced_example advanced/main.cpp)
target_link_libraries(advanced_example PRIVATE cli11_gui)
```

- [ ] **Step 4: 测试示例**

```bash
cd build
cmake ..
make
./examples/basic_example
./examples/advanced_example
```

- [ ] **Step 5: 提交**

```bash
git add examples/
git commit -m "feat: 创建完整示例"
```

---

## Task 13: 编写测试

**Files:**
- Create: `tests/CMakeLists.txt`
- Create: `tests/test_basic.cpp`
- Create: `tests/test_config.cpp`
- Create: `tests/test_detection.cpp`

- [ ] **Step 1: 创建测试 CMakeLists.txt**

```cmake
# tests/CMakeLists.txt

# 测试框架
FetchContent_Declare(
    catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.4.0
)
FetchContent_MakeAvailable(catch2)

# 测试可执行文件
add_executable(cli11_gui_tests
    test_basic.cpp
    test_config.cpp
    test_detection.cpp
)

target_link_libraries(cli11_gui_tests PRIVATE
    cli11_gui
    Catch2::Catch2WithMain
)

# 添加测试
include(Catch)
catch_discover_tests(cli11_gui_tests)
```

- [ ] **Step 2: 创建基本测试**

```cpp
// tests/test_basic.cpp
#include <catch2/catch_test_macros.hpp>
#include <cli11_gui.hpp>

TEST_CASE("Version API", "[version]") {
    REQUIRE(CLI11_GUI::version_major() == 1);
    REQUIRE(CLI11_GUI::version_minor() == 0);
    REQUIRE(CLI11_GUI::version_patch() == 0);
    REQUIRE(std::string(CLI11_GUI::version()) == "1.0.0");
}
```

- [ ] **Step 3: 创建配置测试**

```cpp
// tests/test_config.cpp
#include <catch2/catch_test_macros.hpp>
#include <cli11_gui.hpp>

TEST_CASE("Config defaults", "[config]") {
    CLI11_GUI::Config config;
    
    REQUIRE(config.window_width == 800);
    REQUIRE(config.window_height == 600);
    REQUIRE(config.resizable == true);
    REQUIRE(config.remember_position == true);
    REQUIRE(config.theme == CLI11_GUI::Theme::System);
    REQUIRE(config.layout_mode == CLI11_GUI::LayoutMode::Auto);
    REQUIRE(config.redirect_cout == true);
    REQUIRE(config.show_timestamps == true);
    REQUIRE(config.auto_scroll == true);
    REQUIRE(config.validation_mode == CLI11_GUI::ValidationMode::OnSubmit);
    REQUIRE(config.trigger_mode == CLI11_GUI::TriggerMode::NoArgs);
    REQUIRE(config.simulation_mode == false);
}

TEST_CASE("Config customization", "[config]") {
    CLI11_GUI::Config config;
    
    config.window_title = "Test";
    config.window_width = 1024;
    config.window_height = 768;
    config.theme = CLI11_GUI::Theme::Dark;
    
    REQUIRE(config.window_title == "Test");
    REQUIRE(config.window_width == 1024);
    REQUIRE(config.window_height == 768);
    REQUIRE(config.theme == CLI11_GUI::Theme::Dark);
}
```

- [ ] **Step 4: 创建参数检测测试**

```cpp
// tests/test_detection.cpp
#include <catch2/catch_test_macros.hpp>
#include <cli11_gui.hpp>

TEST_CASE("GUI detection", "[detection]") {
    // 在无参数情况下应该显示 GUI
    // 注意：这个测试在实际运行时可能会失败
    // 因为测试框架本身会传递参数
    REQUIRE(CLI11_GUI::should_show_gui() == true);
}
```

- [ ] **Step 5: 更新主 CMakeLists.txt**

```cmake
# 在主 CMakeLists.txt 中添加
if(CLI11_GUI_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
```

- [ ] **Step 6: 运行测试**

```bash
cd build
cmake ..
make
ctest
```

- [ ] **Step 7: 提交**

```bash
git add tests/ CMakeLists.txt
git commit -m "test: 添加单元测试"
```

---

## Task 14: 创建文档

**Files:**
- Create: `README.md`
- Create: `docs/api.md`

- [ ] **Step 1: 创建 README.md**

```markdown
# CLI11_GUI

为 CLI11 添加 GUI 支持的 C++ 库。

## 特性

- **零配置**：只需包含一个头文件
- **自动检测**：无命令行参数时自动显示 GUI
- **透明替换**：GUI 收集的参数与命令行参数完全等效
- **跨平台**：支持 Windows、macOS、Linux
- **Header-only**：无需编译，直接包含

## 快速开始

### 安装

1. 复制 `include/cli11_gui.hpp` 到你的项目
2. 确保已安装 CLI11 和 Dear ImGui

### 基本使用

```cpp
#include <cli11_gui.hpp>

int main(int argc, char** argv) {
    CLI::App app{"My Application"};
    
    int port = 8080;
    app.add_option("--port", port, "Server port");
    
    std::string host = "localhost";
    app.add_option("--host", host, "Server host");
    
    bool verbose = false;
    app.add_flag("--verbose", verbose, "Enable verbose output");
    
    CLI11_GUI::run(app);
    
    // 你的程序逻辑
    return 0;
}
```

### 自定义配置

```cpp
CLI11_GUI::Config config;
config.window_title = "My App";
config.theme = CLI11_GUI::Theme::Dark;
config.layout_mode = CLI11_GUI::LayoutMode::Tabs;

CLI11_GUI::run(app, config);
```

## 构建

```bash
mkdir build && cd build
cmake ..
make
```

## 示例

- `examples/basic/` - 基本使用示例
- `examples/advanced/` - 高级使用示例

## 测试

```bash
cd build
ctest
```

## 许可证

BSD 3-Clause License
```

- [ ] **Step 2: 创建 API 文档**

```markdown
# CLI11_GUI API 文档

## 命名空间

所有 API 都在 `CLI11_GUI` 命名空间中。

## 版本 API

```cpp
constexpr const char* version();
constexpr int version_major();
constexpr int version_minor();
constexpr int version_patch();
```

## 核心 API

### run

```cpp
void run(CLI::App& app);
void run(CLI::App& app, const Config& config);
```

运行应用程序，自动检测是否显示 GUI。

### should_show_gui

```cpp
bool should_show_gui();
```

检查是否应该显示 GUI。

## 日志 API

```cpp
void log_debug(const std::string& message);
void log_info(const std::string& message);
void log_warning(const std::string& message);
void log_error(const std::string& message);
void log_success(const std::string& message);
```

## 状态 API

```cpp
void save_state();
void load_state();
void reset_state();
bool has_state();
```

## 配置

### Config 结构

```cpp
struct Config {
    // 窗口配置
    std::string window_title = "";
    int window_width = 800;
    int window_height = 600;
    bool resizable = true;
    bool remember_position = true;
    
    // 布局配置
    LayoutMode layout_mode = LayoutMode::Auto;
    
    // 主题配置
    Theme theme = Theme::System;
    
    // 输出配置
    bool redirect_cout = true;
    bool show_timestamps = true;
    bool auto_scroll = true;
    
    // 验证配置
    ValidationMode validation_mode = ValidationMode::OnSubmit;
    
    // 触发配置
    TriggerMode trigger_mode = TriggerMode::NoArgs;
    
    // 测试配置
    bool simulation_mode = false;
};
```

### 枚举类型

#### Theme

```cpp
enum class Theme {
    Light,
    Dark,
    System
};
```

#### LayoutMode

```cpp
enum class LayoutMode {
    Flat,
    Tabs,
    Wizard,
    Collapsible,
    Auto
};
```

#### ControlType

```cpp
enum class ControlType {
    Auto,
    Checkbox,
    Toggle,
    Slider,
    InputInt,
    InputFloat,
    InputText,
    Multiline,
    Dropdown,
    Radio,
    List
};
```

#### LogLevel

```cpp
enum class LogLevel {
    Debug,
    Info,
    Warning,
    Error,
    Success
};
```

#### ValidationMode

```cpp
enum class ValidationMode {
    OnInput,
    OnSubmit,
    Hybrid,
    Configurable
};
```

#### TriggerMode

```cpp
enum class TriggerMode {
    NoArgs,
    InvalidArgs,
    ExplicitFlag,
    Combined
};
```

## GUI 类

```cpp
class GUI {
public:
    GUI(const CLI::App& app, const Config& config);
    ~GUI();
    
    bool show();
    std::vector<std::string> get_args() const;
};
```

## 选项覆盖

```cpp
// 为特定选项指定控件类型
app.add_option("--port", port)
   ->gui_control(ControlType::Slider, 1024, 65535);
```
```

- [ ] **Step 3: 提交**

```bash
git add README.md docs/
git commit -m "docs: 创建文档"
```

---

## Task 15: 最终集成和测试

**Files:**
- Modify: `include/cli11_gui.hpp`

- [ ] **Step 1: 整合所有组件到头文件**

确保所有组件都正确集成到 `include/cli11_gui.hpp` 中。

- [ ] **Step 2: 完整编译测试**

```bash
cd build
cmake ..
make
```

- [ ] **Step 3: 运行所有测试**

```bash
ctest
```

- [ ] **Step 4: 运行示例**

```bash
./examples/basic_example
./examples/advanced_example
```

- [ ] **Step 5: 最终提交**

```bash
git add .
git commit -m "feat: 完成 CLI11_GUI 库实现"
```

---

## 执行选项

**计划完成并保存到 `docs/superpowers/plans/2026-05-05-cli11-gui-implementation.md`。两种执行选项：**

**1. 子代理驱动（推荐）** - 我为每个任务分派一个新的子代理，任务之间进行审查，快速迭代

**2. 内联执行** - 在当前会话中执行任务，批量执行并设置检查点

**选择哪种方式？**
