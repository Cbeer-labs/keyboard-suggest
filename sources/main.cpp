#include <iostream>
#include <memory>
//#include <string>

#include "cxxopts.hpp"
#include "details/app.h"

int main(int argc, char* argv[]) {
  Config config;

  {
    cxxopts::Options options("bugs-world", "Cpp contest");
    options.add_options()("t,timeout", "Sleep for between epochs", cxxopts::value<long long>()->default_value("300"))(
        "r,resources", "Path to viewer's resources", cxxopts::value<std::string>())(
        "p,port", "Port number", cxxopts::value<size_t>()->default_value("1984"))("h,help", "Print usage");
    auto result = options.parse(argc, argv);

    config.timeout = result["timeout"].as<long long>();
    config.resources_path = result["resources"].as<std::string>();
    config.port = result["port"].as<size_t>();

    std::cout << "Timeout: " << config.timeout << "ms" << std::endl;
    std::cout << "Resources: '" << config.resources_path << "'" << std::endl;
    std::cout << "Service port: " << config.port << std::endl;
  }

  Run(config);
}
