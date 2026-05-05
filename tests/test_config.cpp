#include <catch2/catch_test_macros.hpp>
#include <cli11_gui.hpp>

TEST_CASE("Config defaults", "[config]") {
    CLI11_GUI::Config config;

    REQUIRE(config.window_width == 800);
    REQUIRE(config.window_height == 600);
    REQUIRE(config.resizable == true);
    REQUIRE(config.remember_position == true);
    REQUIRE(config.theme == CLI11_GUI::Theme::System);
    REQUIRE(config.layout_mode == CLI11_GUI::LayoutMode::Auto);
    REQUIRE(config.redirect_cout == true);
    REQUIRE(config.show_timestamps == true);
    REQUIRE(config.auto_scroll == true);
    REQUIRE(config.validation_mode == CLI11_GUI::ValidationMode::OnSubmit);
    REQUIRE(config.trigger_mode == CLI11_GUI::TriggerMode::NoArgs);
    REQUIRE(config.simulation_mode == false);
}

TEST_CASE("Config customization", "[config]") {
    CLI11_GUI::Config config;

    config.window_title = "Test";
    config.window_width = 1024;
    config.window_height = 768;
    config.theme = CLI11_GUI::Theme::Dark;

    REQUIRE(config.window_title == "Test");
    REQUIRE(config.window_width == 1024);
    REQUIRE(config.window_height == 768);
    REQUIRE(config.theme == CLI11_GUI::Theme::Dark);
}
