#include <cli11_gui.hpp>
#include <cassert>

int main() {
    CLI11_GUI::Config config;
    assert(config.window_width == 800);
    assert(config.window_height == 600);
    assert(config.theme == CLI11_GUI::Theme::System);
    return 0;
}
