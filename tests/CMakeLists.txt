FetchContent_Declare(
    catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.4.0
)
FetchContent_MakeAvailable(catch2)

add_executable(cli11_gui_tests
    test_basic.cpp
    test_config.cpp
    test_detection.cpp
)

target_link_libraries(cli11_gui_tests PRIVATE
    cli11_gui
    Catch2::Catch2WithMain
)

include(Catch)
catch_discover_tests(cli11_gui_tests)
