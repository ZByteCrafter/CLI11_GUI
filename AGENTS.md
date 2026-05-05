# AGENTS.md - CLI11_GUI

## Project Overview

Header-only C++ library adding GUI support to CLI11. When launched without command-line arguments, automatically shows a GUI window to collect options; with arguments, behaves like normal CLI11.

## Build & Test Commands

```bash
# Configure and build (Windows: use Visual Studio 2022 generator)
cmake -B build
cmake --build build

# Run tests
ctest --test-dir build

# Run examples
./build/examples/basic_example
./build/examples/advanced_example
```

## Architecture

- **Single header**: `include/cli11_gui.hpp` (1000+ lines) - contains ALL code
- **Dependencies**: CLI11 v2.4.2, Dear ImGui v1.89.9, GLFW 3.3.8 (auto-fetched via CMake FetchContent)
- **Test framework**: Catch2 v3.4.0
- **C++ standard**: C++14 minimum

## Key Components (all in `cli11_gui.hpp`)

| Component | Purpose |
|-----------|---------|
| `CLI11_GUI::Config` | Window/theme/layout configuration |
| `CLI11_GUI::GUI` | Main window class |
| `detail::ControlGenerator` | Auto-generates ImGui controls from CLI11 options |
| `detail::OutputBuffer` | Captures and displays stdout/stderr |
| `detail::ThemeManager` | Light/Dark/System theme support |
| `detail::LayoutManager` | Flat/Tabs/Wizard/Collapsible layouts |
| `detail::StateManager` | Saves/restores window position |

## API Usage Pattern

```cpp
#include <cli11_gui.hpp>

int main(int argc, char** argv) {
    CLI::App app{"My App"};
    int port = 8080;
    app.add_option("--port", port);
    
    CLI11_GUI::run(app);  // Auto-detects: GUI or CLI mode
    return 0;
}
```

## Testing

Tests in `tests/` use Catch2. Each test file covers one aspect:
- `test_basic.cpp` - Version API
- `test_config.cpp` - Config struct defaults/customization
- `test_detection.cpp` - GUI trigger detection logic

## Common Pitfalls

1. **Network required**: First build downloads dependencies via FetchContent
2. **OpenGL required**: System must have OpenGL drivers (most desktops do)
3. **Single translation unit**: Since it's header-only, include in only one .cpp per executable to avoid ODR violations
4. **MSVC encoding warnings**: Chinese comments may trigger C4819; use UTF-8 with BOM or ASCII comments

## File Structure

```
CLI11_GUI/
├── include/cli11_gui.hpp    # The library (header-only)
├── CMakeLists.txt           # Build config
├── examples/                # Usage examples
├── tests/                   # Catch2 tests
└── docs/                    # Design specs and plans
```
