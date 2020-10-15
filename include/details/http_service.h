//
// Created by drewxa on 15.10.2020.
//

#ifndef APP_HTTP_SERVICE_H
#define APP_HTTP_SERVICE_H

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <mutex>
#include <sstream>
#include <set>
#include <fstream>
#include <random>

#include <restbed>

template<typename T>
class SceneHolder {
    T scene_;
    std::mutex mutex_;

public:
    void Change(T scene) {
        std::lock_guard<std::mutex> lk(mutex_);
        scene_ = std::move(scene);
    }

    void lock() {
        mutex_.lock();
    }

    void unlock() {
        mutex_.unlock();
    }

    /// Not thread safe method.
    const T &scene() const {
        return scene_;
    }

    /// Thread safe method.
    T data() {
        T res{};
        {
            std::lock_guard<std::mutex> lk(mutex_);
            res = scene_;
        }
        return res;
    }
};

template<typename T>
void HttpServiceRun(SceneHolder<T> &holder, uint16_t port, const std::string& viewer_path) {

    auto resource = std::make_shared<restbed::Resource>();
    resource->set_path("/data");
    resource->set_method_handler("GET", [&holder](const std::shared_ptr<restbed::Session> session) {
        std::unique_lock<SceneHolder<T>> lk(holder);
//        if (holder.scene()->IsValid()) {
        session->close(restbed::OK, holder.scene());
        //, {{"Content-Type", "application/json"}});//, {"Content-Length", std::to_string(data.size())}});
//        }
    });

    auto viewer = std::make_shared<restbed::Resource>();
    viewer->set_path("/viewer/{filename: (index.html|render.js)}");
    viewer->set_method_handler("GET", [&holder, viewer_path](const std::shared_ptr<restbed::Session> session) {
        const auto request = session->get_request();
        const std::string filename = request->get_path_parameter("filename");

        std::ifstream stream(viewer_path + filename, std::ifstream::in);

        if (stream.is_open()) {
            const std::string body = std::string(std::istreambuf_iterator<char>(stream),
                                                 std::istreambuf_iterator<char>());

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


#endif //APP_HTTP_SERVICE_H
