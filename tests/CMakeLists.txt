add_executable(test_include test_include.cpp)
target_link_libraries(test_include PRIVATE cli11_gui)
add_test(NAME test_include COMMAND test_include)
