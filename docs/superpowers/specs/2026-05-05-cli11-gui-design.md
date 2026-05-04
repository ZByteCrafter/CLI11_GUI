# CLI11_GUI 设计文档

**日期**：2026-05-05
**版本**：1.0.0
**作者**：CLI11_GUI 项目组

## 1. 项目概述

### 1.1 核心理念

CLI11_GUI 是一个 C++ 库，扩展 CLI11 以提供 GUI 支持：

- **零配置**：用户只需包含一个头文件，无需额外配置
- **自动检测**：自动检测是否有命令行参数，决定是否显示 GUI
- **透明替换**：GUI 收集的参数与命令行参数完全等效

### 1.2 工作流程

```
程序启动
    ↓
检测是否有命令行参数
    ↓
有参数 → 正常 CLI11 解析 → 执行程序逻辑
    ↓
无参数（或特定条件）→ 显示 GUI 窗口
    ↓
GUI 收集参数 → 转换为 argc/argv → 执行程序逻辑
```

## 2. 技术规格

### 2.1 技术栈

- **语言**：C++14 及以上
- **构建系统**：CMake
- **GUI 框架**：Dear ImGui
- **依赖管理**：FetchContent
- **许可证**：BSD 3-Clause

### 2.2 集成方式

**纯 header-only**：所有代码都在单个头文件 `cli11_gui.hpp` 中，用户只需：

```cpp
#include "cli11_gui.hpp"
```

### 2.3 跨平台支持

- Windows (Win32)
- macOS (Cocoa)
- Linux (X11/Wayland)

## 3. 架构设计

### 3.1 主要组件

1. **CLI11 扩展层**：扩展现有 CLI11 App 类，添加 GUI 支持
2. **控件生成器**：根据 CLI11 选项类型自动生成对应 GUI 控件
3. **布局管理器**：支持多种布局模式
4. **输出显示框**：重定向 cout，支持日志级别
5. **主题系统**：提供浅色、深色、系统主题
6. **状态管理**：保存和恢复窗口位置、大小

### 3.2 单文件内部组织

```cpp
// cli11_gui.hpp

// 第一部分：前向声明和类型定义
// 第二部分：内部实现细节
// 第三部分：核心类实现
// 第四部分：公共 API 实现
// 第五部分：CLI11 扩展
```

## 4. API 设计

### 4.1 核心 API

```cpp
// 1. 最简单的使用方式
CLI::App app{"My Application"};
// ... 添加选项 ...
CLI11_GUI::run(app);  // 自动检测并处理 GUI

// 2. 自定义配置
CLI11_GUI::Config config;
config.window_title = "My App";
config.layout_mode = CLI11_GUI::LayoutMode::Tabs;
config.theme = CLI11_GUI::Theme::Dark;
CLI11_GUI::run(app, config);

// 3. 手动控制
if (CLI11_GUI::should_show_gui()) {
    CLI11_GUI::GUI gui{app, config};
    if (gui.show()) {
        auto args = gui.get_args();
        app.parse(args);
        // 执行程序逻辑
    }
} else {
    app.parse(argc, argv);
}
```

### 4.2 选项覆盖 API

```cpp
// 为特定选项指定控件类型
app.add_option("--port", port)
   ->gui_control(CLI11_GUI::ControlType::Slider, 1024, 65535);

app.add_option("--level", level)
   ->gui_control(CLI11_GUI::ControlType::Dropdown, {"Low", "Medium", "High"});

app.add_flag("--verbose")
   ->gui_control(CLI11_GUI::ControlType::Toggle);
```

### 4.3 输出 API

```cpp
// 使用专用输出函数
CLI11_GUI::log_info("Processing...");
CLI11_GUI::log_warning("Low memory");
CLI11_GUI::log_error("File not found");

// 或者直接使用 cout（自动重定向）
std::cout << "Normal output" << std::endl;
std::cerr << "Error output" << std::endl;
```

## 5. 控件映射规则

### 5.1 自动推断规则

| CLI11 类型 | 默认控件 | 备选控件 |
|------------|----------|----------|
| `add_flag()` | 复选框 (Checkbox) | 开关 (Toggle) |
| `add_option<bool>` | 复选框 (Checkbox) | 开关 (Toggle) |
| `add_option<int/float/double>` | 数字输入框 | 滑块 (Slider) |
| `add_option<string>` | 文本输入框 | 多行文本 |
| `add_option<vector<T>>` | 动态列表 | 标签输入 |
| 位置参数 | 文本输入框 | - |

### 5.2 控件类型枚举

```cpp
enum class ControlType {
    Auto,           // 自动推断（默认）
    Checkbox,       // 复选框
    Toggle,         // 开关
    Slider,         // 滑块
    InputInt,       // 整数输入
    InputFloat,     // 浮点数输入
    InputText,      // 文本输入
    Multiline,      // 多行文本
    Dropdown,       // 下拉框
    Radio,          // 单选按钮
    List,           // 列表
    ColorPicker,    // 颜色选择器
    FileBrowser,    // 文件浏览器
    DirectoryBrowser // 目录浏览器
};
```

### 5.3 验证器映射

```cpp
app.add_option("--port", port)
   ->check(CLI::Range(1024, 65535));  // 自动创建滑块

app.add_option("--email", email)
   ->check(CLI::ValidEmail);  // 自动添加邮箱格式验证

app.add_option("--file", file)
   ->check(CLI::ExistingFile);  // 自动添加文件存在验证
```

## 6. 布局模式

### 6.1 布局模式枚举

```cpp
enum class LayoutMode {
    Flat,           // 单窗口平铺
    Tabs,           // 标签页
    Wizard,         // 向导式
    Collapsible     // 可折叠面板
};
```

### 6.2 布局策略

1. **Flat 模式**：所有选项在一个窗口中平铺显示，子命令用分组框区分
2. **Tabs 模式**：每个子命令对应一个标签页，全局选项显示在第一个标签页
3. **Wizard 模式**：按子命令顺序逐步引导用户填写
4. **Collapsible 模式**：每个子命令是一个可折叠的面板

### 6.3 自动布局选择

```cpp
if (subcommands.empty()) {
    layout_mode = LayoutMode::Flat;
} else if (subcommands.size() <= 3) {
    layout_mode = LayoutMode::Tabs;
} else {
    layout_mode = LayoutMode::Collapsible;
}
```

## 7. 输出显示框

### 7.1 日志级别支持

```cpp
enum class LogLevel {
    Debug,      // 灰色
    Info,       // 白色
    Warning,    // 黄色
    Error,      // 红色
    Success     // 绿色
};
```

### 7.2 自动重定向

- 捕获 `std::cout` 输出，显示为 Info 级别
- 捕获 `std::cerr` 输出，显示为 Error 级别
- 捕获 `std::clog` 输出，显示为 Warning 级别

### 7.3 显示功能

- 自动滚动到底部
- 清空日志
- 复制选中文本
- 保存到文件
- 按级别过滤
- 搜索内容
- 时间戳显示
- 语法高亮

## 8. 主题系统

### 8.1 预设主题

```cpp
enum class Theme {
    Light,      // 浅色主题
    Dark,       // 深色主题
    System      // 跟随系统
};
```

### 8.2 主题配置结构

```cpp
struct ThemeConfig {
    ImVec4 background;
    ImVec4 background_child;
    ImVec4 background_popup;
    ImVec4 text;
    ImVec4 text_disabled;
    ImVec4 primary;
    ImVec4 secondary;
    ImVec4 accent;
    ImVec4 border;
    ImVec4 border_light;
    ImVec4 success;
    ImVec4 warning;
    ImVec4 error;
    ImVec4 info;
};
```

## 9. 状态管理

### 9.1 保存的状态

```cpp
struct WindowState {
    int x;
    int y;
    int width;
    int height;
    int monitor_index;
    bool maximized;
    bool minimized;
};
```

### 9.2 状态文件位置

- Windows: `%APPDATA%/CLI11_GUI/state.ini`
- macOS: `~/Library/Application Support/CLI11_GUI/state.ini`
- Linux: `~/.config/CLI11_GUI/state.ini`

### 9.3 状态管理 API

```cpp
CLI11_GUI::save_state();
CLI11_GUI::load_state();
CLI11_GUI::reset_state();
bool exists = CLI11_GUI::has_state();
```

## 10. 错误处理

### 10.1 错误类型

```cpp
enum class ErrorCode {
    Success = 0,
    GUIInitFailed,
    WindowCreationFailed,
    RenderingFailed,
    InvalidOption,
    ValidationFailed,
    StateLoadFailed,
    StateSaveFailed,
    OutputRedirectFailed
};
```

### 10.2 错误处理策略

1. **GUI 初始化失败**：弹出错误对话框，回退到命令行模式
2. **渲染失败**：记录错误日志，尝试重新初始化
3. **验证失败**：高亮显示错误字段，显示错误提示
4. **状态文件错误**：静默忽略，使用默认状态

## 11. 线程安全和资源管理

### 11.1 线程安全策略

- GUI 操作必须在主线程执行
- 提供线程安全的输出接口

```cpp
class ThreadSafeOutput {
public:
    void log(LogLevel level, const std::string& message) {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push({level, message, std::this_thread::get_id());
    }
    
private:
    std::mutex mutex_;
    std::queue<LogEntry> queue_;
};
```

### 11.2 资源管理策略

- 使用 RAII 模式管理资源
- 使用智能指针管理动态资源
- 退出时自动清理所有资源

## 12. 测试支持

### 12.1 模拟模式

```cpp
CLI11_GUI::Config config;
config.simulation_mode = true;

CLI11_GUI::Simulation simulation;
simulation.set_value("--port", "8080");
simulation.set_value("--host", "localhost");

CLI11_GUI::run(app, config, simulation);
```

### 12.2 模拟 API

```cpp
class Simulation {
public:
    void set_value(const std::string& option, const std::string& value);
    void set_values(const std::map<std::string, std::string>& values);
    void simulate_click(const std::string& button);
    void simulate_input(const std::string& field, const std::string& value);
    bool verify_value(const std::string& option, const std::string& expected);
    std::vector<std::string> get_args() const;
};
```

## 13. 性能优化

### 13.1 启动优化

- 延迟初始化：只在第一次显示时初始化 GUI
- 按需加载：只为必需或已使用的选项创建控件

### 13.2 渲染优化

- 脏区域检测：只在内容变化时重绘
- 控件缓存：缓存控件状态，避免重复计算

### 13.3 内存优化

- 对象池：重用控件对象
- 字符串优化：使用 string_view 避免拷贝

## 14. 版本管理

### 14.1 版本信息

```cpp
#define CLI11_GUI_VERSION_MAJOR 1
#define CLI11_GUI_VERSION_MINOR 0
#define CLI11_GUI_VERSION_PATCH 0
#define CLI11_GUI_VERSION "1.0.0"
```

### 14.2 版本 API

```cpp
namespace CLI11_GUI {
    constexpr const char* version();
    constexpr int version_major();
    constexpr int version_minor();
    constexpr int version_patch();
    constexpr bool is_compatible(int major, int minor, int patch);
}
```

## 15. 配置选项

### 15.1 Config 结构

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

### 15.2 枚举类型

```cpp
enum class ValidationMode {
    OnInput,        // 输入时验证
    OnSubmit,       // 提交时验证
    Hybrid,         // 混合模式
    Configurable    // 可配置
};

enum class TriggerMode {
    NoArgs,         // 无参数时触发
    InvalidArgs,    // 无效参数时触发
    ExplicitFlag,   // 显式标志触发
    Combined        // 组合模式
};
```

## 16. 依赖项

### 16.1 核心依赖

- **CLI11**：命令行解析库
- **Dear ImGui**：即时模式 GUI 库
- **GLFW**：窗口管理库
- **OpenGL3**：图形 API

### 16.2 可选依赖

- **stb_image**：图像加载（用于图标）
- **inih**：INI 文件解析（用于状态文件）

## 17. 文件结构

```
CLI11_GUI/
├── CMakeLists.txt
├── LICENSE
├── README.md
├── include/
│   └── cli11_gui.hpp          # 单个头文件
├── examples/
│   ├── basic/
│   ├── advanced/
│   └── custom_theme/
├── tests/
│   ├── test_basic.cpp
│   ├── test_layout.cpp
│   ├── test_controls.cpp
│   └── test_output.cpp
└── docs/
    ├── api.md
    ├── tutorial.md
    └── changelog.md
```

## 18. 未来扩展

### 18.1 可能的扩展方向

1. **更多控件类型**：滑块、旋钮、图表等
2. **自定义主题**：允许用户创建和分享主题
3. **插件系统**：支持第三方插件扩展功能
4. **国际化**：支持多语言界面
5. **可访问性**：增强屏幕阅读器支持

### 18.2 渐进式发展

- 第一版：核心功能（选项、标志、位置参数）
- 第二版：高级功能（子命令、选项组、验证器）
- 第三版：完整功能（回调、互斥组、条件依赖）

## 19. 总结

CLI11_GUI 提供了一个简洁而强大的方式来为 CLI11 应用程序添加 GUI 支持。通过 header-only 设计、自动控件生成和灵活的配置选项，它既易于使用又高度可定制。

**核心优势**：
- 零配置集成
- 自动控件生成
- 多种布局模式
- 完整的输出显示
- 跨平台支持
- 高性能优化
