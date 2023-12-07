#ifndef HTTPD_H_
#define HTTPD_H_

#include <memory>
#include <mutex>
#include "crow_all.h"
#include "state.h"
#include "messages.h"

class HTTPServer : public std::enable_shared_from_this<HTTPServer> {
    crow::SimpleApp app;
    std::mutex state_mutex;
    Storage store;
    InternalState state;
    void RenewSuggestions(const crow::request&, crow::response&);

public:
    HTTPServer() = delete;
    HTTPServer(const Config&);
    void Serve(const Config&);
    void SetupRoutes();
};

#endif // HTTPD_H_
