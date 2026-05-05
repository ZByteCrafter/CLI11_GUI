add_executable(basic_example basic/main.cpp)
target_link_libraries(basic_example PRIVATE cli11_gui)

add_executable(advanced_example advanced/main.cpp)
target_link_libraries(advanced_example PRIVATE cli11_gui)
