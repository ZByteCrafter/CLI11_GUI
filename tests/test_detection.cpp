#include <cli11_gui.hpp>
#include <cassert>

int main() {
    // Test should_show_gui() - no args, always returns true
    assert(CLI11_GUI::should_show_gui() == true);

    return 0;
}
