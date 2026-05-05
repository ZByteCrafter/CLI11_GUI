// examples/advanced/main.cpp
#include <iostream>
#include <cli11_gui.hpp>

int main(int argc, char** argv) {
    CLI::App app{"Advanced Example"};
    
    // 全局选项
    std::string config_file = "config.ini";
    app.add_option("--config,-c", config_file, "Configuration file")
       ->default_val(config_file);
    
    bool debug = false;
    app.add_flag("--debug,-d", debug, "Enable debug mode");
    
    // 子命令：serve
    auto serve_cmd = app.add_subcommand("serve", "Start the server");
    
    int port = 8080;
    serve_cmd->add_option("--port,-p", port, "Server port")
              ->default_val(port)
              ->check(CLI::Range(1024, 65535));
    
    std::string host = "0.0.0.0";
    serve_cmd->add_option("--host,-h", host, "Server host")
              ->default_val(host);
    
    int threads = 4;
    serve_cmd->add_option("--threads,-t", threads, "Number of threads")
              ->default_val(threads)
              ->check(CLI::Range(1, 16));
    
    // 子命令：db
    auto db_cmd = app.add_subcommand("db", "Database operations");
    
    std::string db_host = "localhost";
    db_cmd->add_option("--host", db_host, "Database host")
          ->default_val(db_host);
    
    int db_port = 5432;
    db_cmd->add_option("--port", db_port, "Database port")
          ->default_val(db_port)
          ->check(CLI::Range(1024, 65535));
    
    std::string db_name = "mydb";
    db_cmd->add_option("--name,-n", db_name, "Database name")
          ->default_val(db_name);
    
    // 配置 GUI
    CLI11_GUI::Config config;
    config.window_title = "Advanced Example";
    config.theme = CLI11_GUI::Theme::Dark;
    config.layout_mode = CLI11_GUI::LayoutMode::Tabs;
    
    // 运行
    CLI11_GUI::run(app, config);
    
    // 输出结果
    CLI11_GUI::log_info("Configuration:");
    CLI11_GUI::log_info("  Config file: " + config_file);
    CLI11_GUI::log_info("  Debug mode: " + std::string(debug ? "true" : "false"));
    
    if (serve_cmd->parsed()) {
        CLI11_GUI::log_info("Server configuration:");
        CLI11_GUI::log_info("  Host: " + host);
        CLI11_GUI::log_info("  Port: " + std::to_string(port));
        CLI11_GUI::log_info("  Threads: " + std::to_string(threads));
    }
    
    if (db_cmd->parsed()) {
        CLI11_GUI::log_info("Database configuration:");
        CLI11_GUI::log_info("  Host: " + db_host);
        CLI11_GUI::log_info("  Port: " + std::to_string(db_port));
        CLI11_GUI::log_info("  Name: " + db_name);
    }
    
    return 0;
}
