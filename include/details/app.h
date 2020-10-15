#ifndef APP_APP_H
#define APP_APP_H

#include <string>

class World;

struct Config {
  size_t port = 0;
  std::string resources_path;
  long long timeout = 0;
};

void Run(World&, const Config&);

#endif  // APP_APP_H
