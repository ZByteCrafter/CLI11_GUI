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

} // namespace detail

// Run API
inline void run(CLI::App& app, const Config& config) {
    // TODO: Get argc/argv from global state or other mechanism
    // For now, use hardcoded values as placeholder
    int argc = 1;
    char* argv[] = {(char*)"program", nullptr};

    bool show_gui = detail::should_show_gui_impl(config);

    if (show_gui) {
        // TODO: Implement GUI display
    } else {
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

} // namespace CLI11_GUI
