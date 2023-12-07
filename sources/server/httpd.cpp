#include "server/httpd.h"
#include <mutex>
#include <sstream>
#include <stdexcept>
#include "crow_all.h"
#include "messages.h"
#include "state.h"

void HTTPServer::RenewSuggestions(const crow::request& req, crow::response& res) {
    crow::json::rvalue req_obj = crow::json::load(req.body);
    if (req_obj.error() || !req_obj.has("text")) {
        res.code = 502;
        res.end();
        return;
    }
    std::string data = std::string(req_obj["text"]);
    std::string last_word;
    try {
        last_word = data.substr(data.find_last_of(' ') + 1);
    } catch (std::out_of_range& ex) {
        last_word = "";
    }
    state_mutex.lock();
    auto old_state = state;
    Input in { data, last_word };
    Answer answ = MakeSuggestions(in, store, old_state, state);
    state_mutex.unlock();
    crow::json::wvalue resp;
    resp["suggestions"] = answ.suggestions;
    res.set_header("Content-Type", "application/json");
    res.code = 200;
    res.body = resp.dump();
    res.end();
}

void HTTPServer::SetupRoutes() {
    auto this_shared = shared_from_this();
    CROW_ROUTE(app, "/ping")([]() { return "Pong!"; });
    CROW_ROUTE(app, "/")
        ([](crow::response& res) {
            res.set_static_file_info("static/index.html");
            res.end();
        });
    CROW_ROUTE(app, "/suggestion").methods(crow::HTTPMethod::POST)
        ([this_shared](const crow::request& req, crow::response& res) {
            this_shared->RenewSuggestions(req, res);
        });
}

HTTPServer::HTTPServer(const Config& cfg) : state_mutex() {
    InitState(cfg, state);
    InitStorage(cfg, store);
    state_mutex.unlock();
}

void HTTPServer::Serve(const Config& cfg) {
    return app.bindaddr(cfg.addr).port(cfg.port).multithreaded().run();
}
