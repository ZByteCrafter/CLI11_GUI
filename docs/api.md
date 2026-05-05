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
