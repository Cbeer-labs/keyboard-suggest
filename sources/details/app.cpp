#include "details/app.h"

#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include "details/http_service.h"
#include "details/world.h"

void Run(World& world, const Config& config) {

  SceneHolder<std::string> state;

  std::thread service_runner(HttpServiceRun<std::string>, std::ref(state), config.port, config.resources_path);

  bool is_alive = true;
  for (size_t i = 0; i < 1000 && is_alive; ++i) {
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    world.Step();
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> time_span = t2 - t1;

//    std::cout << "It took me " << time_span.count() << " milliseconds." << std::endl;
    state.Change(world.Serialize());
    is_alive = world.IsValid();
    std::this_thread::sleep_for(std::chrono::milliseconds(config.timeout));
    std::cout << "Epoch: " << i << std::endl;
  }

  service_runner.join();
}