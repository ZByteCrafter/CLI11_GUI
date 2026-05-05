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
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <atomic>

// Platform-specific includes
#ifdef _WIN32
#include <direct.h>  // for _mkdir
#else
#include <sys/stat.h>  // for mkdir
#endif

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

// Window state struct
struct WindowState {
    int x = 0;
    int y = 0;
    int width = 800;
    int height = 600;
    int monitor_index = 0;
    bool maximized = false;
    bool minimized = false;
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

// 输出缓冲区
class OutputBuffer {
public:
    OutputBuffer() = default;
    ~OutputBuffer() = default;

    void add_log(LogLevel level, const std::string& message);
    void clear();
    void render();
    const std::vector<std::pair<LogLevel, std::string>>& get_logs() const;

private:
    std::vector<std::pair<LogLevel, std::string>> logs_;
    bool auto_scroll_ = true;
    bool show_timestamps_ = true;
};

// 重定向 cout
class CoutRedirect {
public:
    explicit CoutRedirect(OutputBuffer& buffer);
    ~CoutRedirect();

    CoutRedirect(const CoutRedirect&) = delete;
    CoutRedirect& operator=(const CoutRedirect&) = delete;

private:
    OutputBuffer& buffer_;
    std::streambuf* original_cout_;
    std::streambuf* original_cerr_;
    std::ostringstream captured_;
};

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
    
    // 检查状态是否已加载
    bool is_loaded() const;
    
    // 检查状态文件是否存在
    bool state_file_exists() const;
    
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
    std::unique_ptr<detail::OutputBuffer> output_buffer_;
    std::unique_ptr<detail::CoutRedirect> cout_redirect_;
    std::unique_ptr<detail::ThemeManager> theme_manager_;
    std::unique_ptr<detail::LayoutManager> layout_manager_;
};

// Run API (implemented at end of file after GUI class)

inline bool should_show_gui() {
    return true;
}

// 全局输出缓冲区（使用原子指针提高线程安全性）
static std::atomic<detail::OutputBuffer*> g_output_buffer{nullptr};

// 全局状态管理器
static std::unique_ptr<detail::StateManager> g_state_manager;

// Log API
inline void log_debug(const std::string& message) {
    auto* buffer = g_output_buffer.load();
    if (buffer) {
        buffer->add_log(LogLevel::Debug, message);
    }
}

inline void log_info(const std::string& message) {
    auto* buffer = g_output_buffer.load();
    if (buffer) {
        buffer->add_log(LogLevel::Info, message);
    }
}

inline void log_warning(const std::string& message) {
    auto* buffer = g_output_buffer.load();
    if (buffer) {
        buffer->add_log(LogLevel::Warning, message);
    }
}

inline void log_error(const std::string& message) {
    auto* buffer = g_output_buffer.load();
    if (buffer) {
        buffer->add_log(LogLevel::Error, message);
    }
}

inline void log_success(const std::string& message) {
    auto* buffer = g_output_buffer.load();
    if (buffer) {
        buffer->add_log(LogLevel::Success, message);
    }
}

// State API
inline void save_state() {
    if (g_state_manager) {
        g_state_manager->save();
    }
}

inline void load_state() {
    if (g_state_manager) {
        g_state_manager->load();
    }
}

inline void reset_state() {
    if (g_state_manager) {
        g_state_manager->reset();
    }
}

inline bool has_state() {
    if (g_state_manager) {
        return g_state_manager->is_loaded();
    }
    return false;
}

// ControlGenerator implementation
inline detail::ControlGenerator::ControlGenerator(const CLI::App& app, Config& config)
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

inline void detail::ControlGenerator::render() {
    for (const auto& option : app_.get_options()) {
        render_option(option);
    }
}

inline void detail::ControlGenerator::render_option(const CLI::Option* option) {
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
            // 默认使用滑块控件
            render_slider(option);
        } else if (type_name == "bool") {
            render_checkbox(option);
        } else {
            render_text_input(option);
        }
    }
}

inline void detail::ControlGenerator::render_flag(const CLI::Option* option) {
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

inline void detail::ControlGenerator::render_text_input(const CLI::Option* option) {
    auto name = option->get_name();
    auto& value = values_[name];

    char buffer[256] = {0};  // 初始化为全零
    snprintf(buffer, sizeof(buffer), "%s", value.c_str());

    if (ImGui::InputText(name.c_str(), buffer, sizeof(buffer))) {
        value = buffer;
    }

    // 显示帮助
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("%s", option->get_description().c_str());
    }
}

inline void detail::ControlGenerator::render_number_input(const CLI::Option* option) {
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

inline void detail::ControlGenerator::render_checkbox(const CLI::Option* option) {
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

inline void detail::ControlGenerator::render_slider(const CLI::Option* option) {
    auto name = option->get_name();
    auto& value = values_[name];

    // 尝试解析为数字并显示滑块
    try {
        if (option->get_type_name() == "int") {
            int int_val = std::stoi(value);
            // 默认范围 0-100，可通过 check(CLI::Range) 设置
            if (ImGui::SliderInt(name.c_str(), &int_val, 0, 100)) {
                value = std::to_string(int_val);
            }
        } else {
            float float_val = std::stof(value);
            if (ImGui::SliderFloat(name.c_str(), &float_val, 0.0f, 100.0f)) {
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

inline void detail::ControlGenerator::render_dropdown(const CLI::Option* option) {
    // CLI11 不直接支持下拉框选项
    // 回退到文本输入
    render_text_input(option);
}

inline std::vector<std::string> detail::ControlGenerator::get_args() const {
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

// OutputBuffer implementation
inline void detail::OutputBuffer::add_log(LogLevel level, const std::string& message) {
    logs_.push_back({level, message});
}

inline void detail::OutputBuffer::clear() {
    logs_.clear();
}

inline void detail::OutputBuffer::render() {
    ImGui::Separator();
    ImGui::Text("Output:");

    if (ImGui::Button("Clear")) {
        clear();
    }
    ImGui::SameLine();
    ImGui::Checkbox("Auto-scroll", &auto_scroll_);
    ImGui::SameLine();
    ImGui::Checkbox("Timestamps", &show_timestamps_);

    ImGui::BeginChild("OutputRegion", ImVec2(0, 200), true);

    for (const auto& log : logs_) {
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

    if (auto_scroll_ && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
        ImGui::SetScrollHereY(1.0f);
    }

    ImGui::EndChild();
}

inline const std::vector<std::pair<LogLevel, std::string>>& detail::OutputBuffer::get_logs() const {
    return logs_;
}

// CoutRedirect implementation
inline detail::CoutRedirect::CoutRedirect(OutputBuffer& buffer)
    : buffer_(buffer), original_cout_(std::cout.rdbuf()), original_cerr_(std::cerr.rdbuf()) {
    std::cout.rdbuf(captured_.rdbuf());
    std::cerr.rdbuf(captured_.rdbuf());
}

inline detail::CoutRedirect::~CoutRedirect() {
    std::cout.rdbuf(original_cout_);
    std::cerr.rdbuf(original_cerr_);

    auto content = captured_.str();
    if (!content.empty()) {
        buffer_.add_log(LogLevel::Info, content);
    }
}

// ThemeManager implementation
inline detail::ThemeManager::ThemeManager() = default;

inline void detail::ThemeManager::apply_theme(Theme theme) {
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

inline Theme detail::ThemeManager::get_current_theme() const {
    return current_theme_;
}

inline void detail::ThemeManager::apply_light_theme() {
    ImGui::StyleColorsLight();
    
    // 自定义样式
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 4.0f;
    style.FrameRounding = 2.0f;
    style.GrabRounding = 2.0f;
}

inline void detail::ThemeManager::apply_dark_theme() {
    ImGui::StyleColorsDark();
    
    // 自定义样式
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 4.0f;
    style.FrameRounding = 2.0f;
    style.GrabRounding = 2.0f;
}

inline void detail::ThemeManager::apply_system_theme() {
    if (is_system_dark_mode()) {
        apply_dark_theme();
    } else {
        apply_light_theme();
    }
}

inline bool detail::ThemeManager::is_system_dark_mode() const {
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

// LayoutManager implementation
inline detail::LayoutManager::LayoutManager(const CLI::App& app, Config& config)
    : app_(app), config_(config) {}

inline LayoutMode detail::LayoutManager::select_layout_mode() {
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

inline void detail::LayoutManager::render() {
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

inline void detail::LayoutManager::render_flat() {
    // 平铺布局：所有选项在一个窗口中
    // 由 ControlGenerator 处理
}

inline void detail::LayoutManager::render_tabs() {
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

inline void detail::LayoutManager::render_wizard() {
    // 向导布局
    auto subcommands = app_.get_subcommands({});

    // 步骤指示
    ImGui::Text("Step %d of %zu", wizard_step_ + 1, subcommands.size() + 1);
    ImGui::Separator();

    // 渲染当前步骤
    if (wizard_step_ == 0) {
        // 全局选项
        ImGui::Text("Global Options");
    } else if (wizard_step_ <= static_cast<int>(subcommands.size())) {
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

    if (wizard_step_ < static_cast<int>(subcommands.size())) {
        if (ImGui::Button("Next")) {
            wizard_step_++;
        }
    }
}

inline void detail::LayoutManager::render_collapsible() {
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

// StateManager implementation
inline detail::StateManager::StateManager() = default;

inline void detail::StateManager::save() {
    auto path = get_state_file_path();
    save_to_file(path);
}

inline void detail::StateManager::load() {
    auto path = get_state_file_path();
    if (std::ifstream(path).good()) {
        load_from_file(path);
        has_state_ = true;
    }
}

inline void detail::StateManager::reset() {
    state_ = WindowState{};
    has_state_ = false;
    
    // 删除状态文件
    auto path = get_state_file_path();
    std::remove(path.c_str());
}

inline bool detail::StateManager::is_loaded() const {
    return has_state_;
}

inline bool detail::StateManager::state_file_exists() const {
    auto path = get_state_file_path();
    std::ifstream file(path);
    return file.good();
}

inline const WindowState& detail::StateManager::get_state() const {
    return state_;
}

inline void detail::StateManager::set_state(const WindowState& state) {
    state_ = state;
}

inline std::string detail::StateManager::get_state_file_path() const {
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

inline void detail::StateManager::save_to_file(const std::string& path) {
    // 创建目录
    auto dir = path.substr(0, path.find_last_of("/\\"));
    if (!dir.empty()) {
#ifdef _WIN32
        _mkdir(dir.c_str());
#else
        mkdir(dir.c_str(), 0755);
#endif
    }
    
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

inline void detail::StateManager::load_from_file(const std::string& path) {
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
            } else if (line.find("monitor=") == 0) {
                state_.monitor_index = std::stoi(line.substr(8));
            } else if (line.find("maximized=") == 0) {
                state_.maximized = (line.substr(10) == "true");
            }
        }
    }
}

// GUI implementation
inline GUI::GUI(const CLI::App& app, const Config& config)
    : app_(app), config_(config) {
    // Delayed initialization
}

inline GUI::~GUI() {
    cleanup();
}

inline bool GUI::initialize() {
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
    // Theme manager will apply the appropriate theme

    // Initialize backends
    ImGui_ImplGlfw_InitForOpenGL(window_, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    // 初始化主题管理器
    theme_manager_ = std::make_unique<detail::ThemeManager>();
    theme_manager_->apply_theme(config_.theme);

    // 创建控件生成器
    control_generator_ = std::make_unique<detail::ControlGenerator>(app_, config_);

    // 创建布局管理器
    layout_manager_ = std::make_unique<detail::LayoutManager>(app_, config_);

    // 创建输出缓冲区
    output_buffer_ = std::make_unique<detail::OutputBuffer>();
    g_output_buffer.store(output_buffer_.get());

    // 重定向 cout
    if (config_.redirect_cout) {
        cout_redirect_ = std::make_unique<detail::CoutRedirect>(*output_buffer_);
    }

    // 初始化全局状态管理器（只创建一次）
    if (!g_state_manager) {
        g_state_manager = std::make_unique<detail::StateManager>();
    }
    g_state_manager->load();
    
    // 恢复窗口位置
    if (g_state_manager->is_loaded() && config_.remember_position) {
        auto state = g_state_manager->get_state();
        glfwSetWindowPos(window_, state.x, state.y);
        glfwSetWindowSize(window_, state.width, state.height);
    }

    initialized_ = true;
    return true;
}

inline void GUI::cleanup() {
    if (initialized_) {
        // 保存窗口状态
        if (g_state_manager && config_.remember_position) {
            WindowState state;
            glfwGetWindowPos(window_, &state.x, &state.y);
            glfwGetWindowSize(window_, &state.width, &state.height);
            g_state_manager->set_state(state);
            g_state_manager->save();
        }

        cout_redirect_.reset();
        g_output_buffer.store(nullptr);
        output_buffer_.reset();

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

inline bool GUI::show() {
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

inline std::vector<std::string> GUI::get_args() const {
    if (control_generator_) {
        return control_generator_->get_args();
    }
    return {};
}

inline void GUI::render() {
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

inline void GUI::render_controls() {
    if (layout_manager_) {
        layout_manager_->render();
    }
    if (control_generator_) {
        control_generator_->render();
    }
}

inline void GUI::render_output() {
    if (output_buffer_) {
        output_buffer_->render();
    }
}

inline void GUI::render_buttons() {
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
inline void run(CLI::App& app, int argc, char** argv, const Config& config) {
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

inline void run(CLI::App& app, const Config& config) {
    // Use global argc/argv if available, otherwise show GUI
    // This is a fallback - prefer using run(app, argc, argv, config)
    Config cfg = config;
    cfg.trigger_mode = TriggerMode::NoArgs;
    GUI gui(app, cfg);
    if (gui.show()) {
        auto args = gui.get_args();
        try {
            app.parse(args);
        } catch (const CLI::ParseError& e) {
            exit(app.exit(e));
        }
    } else {
        exit(0);
    }
}

inline void run(CLI::App& app, int argc, char** argv) {
    Config config;
    run(app, argc, argv, config);
}

inline void run(CLI::App& app) {
    Config config;
    run(app, config);
}

} // namespace CLI11_GUI
