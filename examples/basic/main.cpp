// examples/basic/main.cpp
#include <iostream>
#include <cli11_gui.hpp>

int main(int argc, char** argv) {
    CLI::App app{"Basic Example"};
    
    int port = 8080;
    app.add_option("--port", port, "Server port")
       ->default_val(port)
       ->check(CLI::Range(1024, 65535));
    
    std::string host = "localhost";
    app.add_option("--host", host, "Server host")
       ->default_val(host);
    
    bool verbose = false;
    app.add_flag("--verbose,-v", verbose, "Enable verbose output");
    
    // 配置 GUI
    CLI11_GUI::Config config;
    config.window_title = "Basic Example";
    config.theme = CLI11_GUI::Theme::Dark;
    
    // 运行
    CLI11_GUI::run(app, config);
    
    // 输出结果
    std::cout << "Host: " << host << std::endl;
    std::cout << "Port: " << port << std::endl;
    std::cout << "Verbose: " << (verbose ? "true" : "false") << std::endl;
    
    // 模拟一些输出
    CLI11_GUI::log_info("Starting server...");
    CLI11_GUI::log_success("Server started successfully");
    
    return 0;
}
