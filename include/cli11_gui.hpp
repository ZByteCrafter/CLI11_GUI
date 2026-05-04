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
