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
