#pragma once

/**
 * CLI11_GUI - GUI support for CLI11
 *
 * Copyright (c) 2026 [Your Name]
 *
 * This software is provided under the BSD 3-Clause License.
 * See the LICENSE file for details.
 */

// Version info
#define CLI11_GUI_VERSION_MAJOR 1
#define CLI11_GUI_VERSION_MINOR 0
#define CLI11_GUI_VERSION_PATCH 0
#define CLI11_GUI_VERSION "1.0.0"

// Standard library dependencies
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <memory>
#include <iostream>
#include <sstream>

// CLI11 dependency
#include <CLI/CLI.hpp>

// ImGui dependency
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

namespace CLI11_GUI {

// Forward declarations
class GUI;
struct Config;
class Simulation;

// Enum types
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

// Config struct
struct Config {
    // Window config
    std::string window_title = "";
    int window_width = 800;
    int window_height = 600;
    bool resizable = true;
    bool remember_position = true;

    // Layout config
    LayoutMode layout_mode = LayoutMode::Auto;

    // Theme config
    Theme theme = Theme::System;

    // Output config
    bool redirect_cout = true;
    bool show_timestamps = true;
    bool auto_scroll = true;

    // Validation config
    ValidationMode validation_mode = ValidationMode::OnSubmit;

    // Trigger config
    TriggerMode trigger_mode = TriggerMode::NoArgs;

    // Test config
    bool simulation_mode = false;
};

// Version API
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

// Internal implementation
namespace detail {

// Check if GUI should be shown based on config
inline bool should_show_gui_impl(const Config& config) {
    // Always return false in implementation
    // The public should_show_gui() without args returns true
    return false;
}

// Check if command line has arguments
inline bool has_command_line_args(int argc, char** argv) {
    return argc > 1;
}

// 控件生成器
class ControlGenerator {
public:
    explicit ControlGenerator(const CLI::App& app, Config& config);

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

// Forward declarations for run API
inline void run(CLI::App& app, const Config& config);
inline void run(CLI::App& app);

// GUI window class
class GUI {
public:
    GUI(const CLI::App& app, const Config& config);
    ~GUI();

    // Show GUI and return whether to run
    bool show();

    // Get collected arguments
    std::vector<std::string> get_args() const;

private:
    // Initialize ImGui
    bool initialize();

    // Cleanup resources
    void cleanup();

    // Render GUI
    void render();

    // Render controls
    void render_controls();

    // Render output box
    void render_output();

    // Render buttons
    void render_buttons();

    const CLI::App& app_;
    Config config_;
    GLFWwindow* window_ = nullptr;
    bool initialized_ = false;
    bool should_run_ = false;
    std::vector<std::string> args_;
    std::unique_ptr<detail::ControlGenerator> control_generator_;
};

// Run API (implemented at end of file after GUI class)

inline bool should_show_gui() {
    return true;
}

// Log API
inline void log_debug(const std::string& message) {
    // TODO: Implement debug log
}

inline void log_info(const std::string& message) {
    // TODO: Implement info log
}

inline void log_warning(const std::string& message) {
    // TODO: Implement warning log
}

inline void log_error(const std::string& message) {
    // TODO: Implement error log
}

inline void log_success(const std::string& message) {
    // TODO: Implement success log
}

// State API
inline void save_state() {
    // TODO: Implement state save
}

inline void load_state() {
    // TODO: Implement state load
}

inline void reset_state() {
    // TODO: Implement state reset
}

inline bool has_state() {
    // TODO: Implement state check
    return false;
}

// ControlGenerator implementation
detail::ControlGenerator::ControlGenerator(const CLI::App& app, Config& config)
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

void detail::ControlGenerator::render() {
    for (const auto& option : app_.get_options()) {
        render_option(option);
    }
}

void detail::ControlGenerator::render_option(const CLI::Option* option) {
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

void detail::ControlGenerator::render_flag(const CLI::Option* option) {
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

void detail::ControlGenerator::render_text_input(const CLI::Option* option) {
    auto name = option->get_name();
    auto& value = values_[name];

    char buffer[256];
    strncpy_s(buffer, value.c_str(), sizeof(buffer) - 1);

    if (ImGui::InputText(name.c_str(), buffer, sizeof(buffer))) {
        value = buffer;
    }

    // 显示帮助
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("%s", option->get_description().c_str());
    }
}

void detail::ControlGenerator::render_number_input(const CLI::Option* option) {
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

void detail::ControlGenerator::render_checkbox(const CLI::Option* option) {
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

void detail::ControlGenerator::render_slider(const CLI::Option* option) {
    // TODO: 实现滑块控件
}

void detail::ControlGenerator::render_dropdown(const CLI::Option* option) {
    // TODO: 实现下拉框控件
}

std::vector<std::string> detail::ControlGenerator::get_args() const {
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

// GUI implementation
GUI::GUI(const CLI::App& app, const Config& config)
    : app_(app), config_(config) {
    // Delayed initialization
}

GUI::~GUI() {
    cleanup();
}

bool GUI::initialize() {
    // Initialize GLFW
    if (!glfwInit()) {
        log_error("Failed to initialize GLFW");
        return false;
    }

    // Create window
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

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    // Setup style
    ImGui::StyleColorsDark();

    // Initialize backends
    ImGui_ImplGlfw_InitForOpenGL(window_, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    // 创建控件生成器
    control_generator_ = std::make_unique<detail::ControlGenerator>(app_, config_);

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

    // Main loop
    while (!glfwWindowShouldClose(window_)) {
        glfwPollEvents();

        // Start new frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Render GUI
        render();

        // Render
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window_, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window_);

        // Check if should run
        if (should_run_) {
            break;
        }
    }

    return should_run_;
}

std::vector<std::string> GUI::get_args() const {
    if (control_generator_) {
        return control_generator_->get_args();
    }
    return {};
}

void GUI::render() {
    // Render main window
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin("CLI11_GUI", nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse
    );

    // Render controls
    render_controls();

    // Render output box
    render_output();

    // Render buttons
    render_buttons();

    ImGui::End();
}

void GUI::render_controls() {
    if (control_generator_) {
        control_generator_->render();
    }
}

void GUI::render_output() {
    // TODO: Implement output box rendering
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

// Run API with GUI support
inline void run(CLI::App& app, const Config& config) {
    // Get command line arguments
    int argc = 0;
    char** argv = nullptr;

    // Detect if GUI should be shown
    bool show_gui = false;

    if (config.trigger_mode == TriggerMode::NoArgs) {
        // Show GUI when no arguments
        show_gui = !detail::has_command_line_args(argc, argv);
    }

    if (show_gui) {
        // Show GUI
        GUI gui(app, config);
        if (gui.show()) {
            auto args = gui.get_args();
            try {
                app.parse(args);
            } catch (const CLI::ParseError& e) {
                exit(app.exit(e));
            }
        } else {
            // User cancelled
            exit(0);
        }
    } else {
        // Normal command line parsing
        try {
            app.parse(argc, argv);
        } catch (const CLI::ParseError& e) {
            exit(app.exit(e));
        }
    }
}

inline void run(CLI::App& app) {
    Config config;
    run(app, config);
}

} // namespace CLI11_GUI
