#ifndef APP_HTTP_SERVICE_H
#define APP_HTTP_SERVICE_H

#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>
#include <restbed>
#include <string>

#include "nlohmann/json.hpp"

#include "messages.h"

class SceneHolder {
  Input input_;
  Answer answer_;
  mutable std::mutex mutex_;

 public:
  void Change(Answer answer) {
    std::lock_guard<std::mutex> lk(mutex_);
    answer_ = std::move(answer);
  }

  void lock() { mutex_.lock(); }

  void unlock() { mutex_.unlock(); }

  /// Thread safe method.
  Input input() const {
    Input input;
    {
      std::lock_guard<std::mutex> lk(mutex_);
      input = input_;
    }
    return input;
  }

  /// Thread safe method.
  Answer answer() const {
    Answer answer;
    {
      std::lock_guard<std::mutex> lk(mutex_);
      answer = answer_;
    }
    return answer;
  }

  std::string data() const {
    std::vector<std::string> top_suggestions;
    for (size_t i = 0; i < std::min(answer_.suggestions.size(), 10u); ++i) {
      top_suggestions.push_back(answer_.suggestions[i]);
    }
    nlohmann::json js(top_suggestions);
    return js.dump();
  }
};

class HttpService {
 public:
  HttpService(InternalState state, Storage storage, const std::string &viewer_path)
      : state_(std::move(state)), storage_(std::move(storage)), viewer_path_(viewer_path) {}

  void Run(uint16_t port);

 private:
  InternalState state_;
  Storage storage_;
  std::string viewer_path_;
  SceneHolder holder_;
};

#endif  // APP_HTTP_SERVICE_H
