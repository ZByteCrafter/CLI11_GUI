#include <catch2/catch_test_macros.hpp>
#include <cli11_gui.hpp>

TEST_CASE("Version API", "[version]") {
    REQUIRE(CLI11_GUI::version_major() == 1);
    REQUIRE(CLI11_GUI::version_minor() == 0);
    REQUIRE(CLI11_GUI::version_patch() == 0);
    REQUIRE(std::string(CLI11_GUI::version()) == "1.0.0");
}
