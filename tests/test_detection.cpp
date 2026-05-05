#include <cli11_gui.hpp>
#include <cassert>

int main() {
    // 测试无参数情况
    assert(CLI11_GUI::should_show_gui() == true);

    return 0;
}
