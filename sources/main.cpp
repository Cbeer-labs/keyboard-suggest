#include <memory>
#include <filesystem>
#include "messages.h"
#include "server/httpd.h"
#include "server/config.h"

int main() {
  Config cfg;
  cfg.addr = "127.0.0.1";
  cfg.port = 8080;
  auto serv = std::make_shared<HTTPServer>(cfg);
  serv->SetupRoutes();
  std::filesystem::current_path(PREFIX);
  serv->Serve(cfg);
}
