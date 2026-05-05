// examples/basic/main.cpp
#include <iostream>
#include <cli11_gui.hpp>

int main(int argc, char** argv) {
    CLI::App app{"Basic Example"};
    
    int port = 8080;
    app.add_option("--port", port, "Server port")->default_val(port);
    
    std::string host = "localhost";
    app.add_option("--host", host, "Server host")->default_val(host);
    
    bool verbose = false;
    app.add_flag("--verbose", verbose, "Enable verbose output");
    
    // CLI11_GUI will auto-detect command line arguments
    // Show GUI when no args, parse normally with args
    CLI11_GUI::run(app);
    
    std::cout << "Host: " << host << std::endl;
    std::cout << "Port: " << port << std::endl;
    std::cout << "Verbose: " << (verbose ? "true" : "false") << std::endl;
    
    return 0;
}
