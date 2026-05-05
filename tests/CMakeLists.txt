add_executable(test_include test_include.cpp)
target_link_libraries(test_include PRIVATE cli11_gui)
add_test(NAME test_include COMMAND test_include)

add_executable(test_config test_config.cpp)
target_link_libraries(test_config PRIVATE cli11_gui)
add_test(NAME test_config COMMAND test_config)

add_executable(test_detection test_detection.cpp)
target_link_libraries(test_detection PRIVATE cli11_gui)
add_test(NAME test_detection COMMAND test_detection)
