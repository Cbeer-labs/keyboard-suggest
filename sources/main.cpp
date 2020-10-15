#include <chrono>
#include <iostream>
#include <memory>
#include <string>

#include "cxxopts.hpp"
#include "details/app.h"
#include "details/bug.h"
#include "details/world.h"

struct RandomBug : public Bug {
  ActionType Action(const WorldMap& map) override {
    int i = rand() % 5;
    return static_cast<ActionType>(i);
  }

  Cell::Ptr Clone() const override final { return std::make_unique<RandomBug>(*this); }
};

struct FoolBug : public Bug {
  ActionType Action(const WorldMap& map) override { return ActionType::Bottom; }

  Cell::Ptr Clone() const override final { return std::make_unique<FoolBug>(*this); }
};

struct KnightBug : public Bug {
  ActionType Action(const WorldMap& map) override {
    state_ = (state_ + 1) % 3;
    if (state_ == 0) {
      return ActionType::Left;
    }
    return ActionType::Top;
  }

  Cell::Ptr Clone() const override final { return std::make_unique<KnightBug>(*this); }

 private:
  size_t state_ = 0;
};

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

  World world(World::Params::DefaultWorldParams());
  world.AddBugs<RandomBug>(5);
  world.AddBugs<FoolBug>(5);
  world.AddBugs<KnightBug>(5);

  Run(world, config);
}
