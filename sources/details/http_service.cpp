#include "details/http_service.h"

#include <sstream>

namespace {
std::string GetLastWord(const std::string &input) {
  std::stringstream s(input);
  std::string last_word;
  while (!s.eof()) {
    std::string t;
    s >> t;
    if (!t.empty()) {
      last_word = std::move(t);
    }
  }
  return last_word;
}
}  // namespace

void HttpService::Run(uint16_t port) {
  auto resource = std::make_shared<restbed::Resource>();
  resource->set_path("/data");
  resource->set_method_handler("GET", [this](const std::shared_ptr<restbed::Session> session) {
    Input input;
    input.input = session->get_request()->get_query_parameter("input", "");
    input.last_word = GetLastWord(input.input);
    std::cout << input.last_word << std::endl;

    InternalState new_state;
    auto answer = MakeSuggestions(input, storage_, state_, new_state);
    holder_.Change(std::move(answer));
    state_ = std::move(new_state);

    std::unique_lock<SceneHolder> lk(holder_);
    session->close(restbed::OK, holder_.data(), {{"Content-Type", "application/json"}});
  });

  auto viewer = std::make_shared<restbed::Resource>();
  viewer->set_path("/viewer/{filename: (index.html|render.js)}");
  viewer->set_method_handler("GET", [this](const std::shared_ptr<restbed::Session> session) {
    const auto request = session->get_request();
    const std::string filename = request->get_path_parameter("filename");

    std::ifstream stream(viewer_path_ + filename, std::ifstream::in);

    if (stream.is_open()) {
      const std::string body = std::string(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());

      session->close(restbed::OK, body);
    } else {
      session->close(restbed::NOT_FOUND);
    }
  });

  auto settings = std::make_shared<restbed::Settings>();
  settings->set_port(port);
  settings->set_default_header("Connection", "close");

  std::cout << "Press Ctrl+C to stop the service." << std::endl;
  const std::string full_url = "http://localhost:" + std::to_string(port) + "/viewer/index.html";
  std::cout << "Staring service: " << full_url << std::endl;

  restbed::Service service;
  service.publish(viewer);
  service.publish(resource);
  service.start(settings);
}