#include <catch2/catch_test_macros.hpp>
#include <cli11_gui.hpp>

TEST_CASE("GUI detection", "[detection]") {
    REQUIRE(CLI11_GUI::should_show_gui() == true);
}
