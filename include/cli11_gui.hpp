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
    return args_;
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
    // TODO: Implement control rendering
    ImGui::Text("Controls will be here");
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
