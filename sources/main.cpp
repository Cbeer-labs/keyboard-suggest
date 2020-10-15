#include <chrono>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

#include <cxxopts.hpp>

#include "details/bug.h"
#include "details/http_service.h"
#include "details/world.h"

struct RandomBug : public Bug {
  ActionType Action(const Map& map) override {
    int i = rand() % 5;
    return static_cast<ActionType>(i);
  }

  std::shared_ptr<Cell> Clone() const override final {
    return std::make_shared<RandomBug>(*this);
  }
};


int main(int argc, char* argv[]) {
  size_t port = 0;
  std::string resources_path;
  long long timeout = 0;

  {
    cxxopts::Options options("bugs-world", "Cpp contest");
    options.add_options()("t,timeout", "Sleep for between epochs",
                          cxxopts::value<long long>()->default_value("300"))(
        "r,resources", "Path to viewer's resources",
        cxxopts::value<std::string>())("p,port", "Port number",
                                       cxxopts::value<size_t>()->default_value(
                                           "1984"))("h,help", "Print usage");
    auto result = options.parse(argc, argv);

    timeout = result["timeout"].as<long long>();
    resources_path = result["resources"].as<std::string>();
    port = result["port"].as<size_t>();

    std::cout << "Timeout: " << timeout << "ms" << std::endl;
    std::cout << "Resources: '" << resources_path << "'" << std::endl;
    std::cout << "Service port: " << port << std::endl;
  }

  SceneHolder<std::string> state;
  World world(50, 50, 0.2, 0.05, 0.000005, 0);
  world.AddBugs<RandomBug>(30, 'r');

  std::thread service_runner(HttpServiceRun<std::string>, std::ref(state),
                             port, resources_path);

  bool is_alive = true;
  using namespace std::chrono;
  for (size_t i = 0; i < 1000 && is_alive; ++i) {
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    world.Step();
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double, std::milli> time_span = t2 - t1;

    std::cout << "It took me " << time_span.count() << " milliseconds."
              << std::endl;
    state.Change(world.Serialize());
    is_alive = world.IsValid();
    std::this_thread::sleep_for(std::chrono::milliseconds(timeout));
    std::cout << i << "  ========" << std::endl;
  }

  service_runner.join();
}
