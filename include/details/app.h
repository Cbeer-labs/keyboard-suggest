#ifndef APP_APP_H
#define APP_APP_H

#include <string>

#include "messages.h"

struct Config {
  size_t port = 0;
  std::string resources_path;
  long long timeout = 0;
};

void Run(const Config&);

std::string Serialize(const Answer&);

#endif  // APP_APP_H
