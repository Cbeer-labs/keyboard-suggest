#include "details/app.h"

#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include "details/http_service.h"

#include "messages.h"

void Run(const Config &config) {
  InternalState internal_state;
  InitState(config, internal_state);

  Storage storage;
  InitStorage(config, storage);

  HttpService service(std::move(internal_state), std::move(storage), config.resources_path);

  std::thread service_runner(&HttpService::Run, &service, config.port);
  service_runner.join();
}