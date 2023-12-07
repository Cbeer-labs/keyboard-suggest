#include <iostream>
#include <memory>
#include "messages.h"
#include "server/httpd.h"

int main() {
  Config cfg;
  cfg.addr = "127.0.0.1";
  cfg.port = 8080;
  auto serv = std::make_shared<HTTPServer>(cfg);
  serv->SetupRoutes();
  serv->Serve(cfg);
}
