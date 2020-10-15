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
#include <queue>

#include <cxxopts.hpp>

#include <restbed>

#include "matrix.h"


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

    const T &scene() const {
        return scene_;
    }
};

template<typename T>
void ServiceRun(SceneHolder<T> &holder) {

    std::unique_lock<SceneHolder<T>> lk(holder);
    while (holder.scene()->IsValid()) {
        std::cout << holder.scene()->Serialize() << std::endl;
        lk.unlock();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        lk.lock();
    }
}

class Map;

class Cell {
public:
    enum class ActionType {
        Left,
        Right,
        Top,
        Bottom,
        Stay
    };

    static std::pair<int, int> ActionTypeToPair(ActionType action) {
        switch (action) {
            case ActionType::Stay:
                return std::make_pair(0, 0);
            case ActionType::Left:
                return std::make_pair(-1, 0);
            case ActionType::Right:
                return std::make_pair(1, 0);
            case ActionType::Bottom:
                return std::make_pair(0, 1);
            case ActionType::Top:
                return std::make_pair(0, -1);
        }
    }

    virtual ~Cell() {}

    virtual ActionType Action(const Map &) = 0;

    virtual char Type() const = 0;

    virtual void OnEpochUpdate() {
        if (points_ > 0)
            AddPoints(-1);
        if (points_ < 0)
            AddPoints(1);
    }

    int points() const {
        return points_;
    }

    bool IsDied() const {
        return points_ == 0;
    }

    virtual std::shared_ptr<Cell> Clone() const = 0;

    Cell(int points, int min_points, int max_points) : points_(points), min_points_(min_points),
                                                       max_points_(max_points) {}

    friend class World;

private:

    void AddPoints(int points) {
        points_ += points;
        if (points_ > max_points_) {
            points_ = max_points_;
        }
        if (points_ < min_points_) {
            points_ = min_points_;
        }
    }

    int points_ = 0;
    int min_points_ = 0;
    int max_points_ = 0;
};

class Map {
public:
    Map() : map(0, 0) {}

    Map(size_t x, size_t y) : map(x, y) {
    }

    Map ViewRegion(size_t x, size_t y) const {
        Map view;
        view.map = Matrix<std::shared_ptr<Cell>>{
                {map[x - 1][y - 1]->Clone(), map[x][y - 1]->Clone(), map[x + 1][y - 1]->Clone()},
                {map[x - 1][y - 0]->Clone(), map[x][y - 0]->Clone(), map[x + 1][y - 0]->Clone()},
                {map[x - 1][y + 1]->Clone(), map[x][y + 1]->Clone(), map[x + 1][y + 1]->Clone()},
        };
        return view;
    }

    bool IsBound(size_t x, size_t y) const {
        return x == 0 || y == 0 || (x + 1) == map.size() || (y + 1) == map[x].size();
    }

    Matrix<std::shared_ptr<Cell>> map;
};

class EmptyCell : public Cell {
public:
    ActionType Action(const Map &) override final {
        return ActionType::Stay;
    }

    EmptyCell() : Cell(0, 0, 0) {}

    char Type() const override final {
        return ' ';
    }

    std::shared_ptr<Cell> Clone() const override final {
        return std::make_shared<EmptyCell>(*this);
    }

    void OnEpochUpdate() override final {}

private:
};

class Poison : public Cell {

public:
    ActionType Action(const Map &) override final {
        return ActionType::Stay;
    }

    Poison() : Cell(1000, 1000, 1000) {}

    char Type() const override final {
        return 'p';
    }

    std::shared_ptr<Cell> Clone() const override final {
        return std::make_shared<Poison>(*this);
    }

    void OnEpochUpdate() override final {}

private:
};

class Food : public Cell {
public:
    ActionType Action(const Map &) override final {
        return ActionType::Stay;
    }

    Food() : Cell(-10, -10, 0) {}

    char Type() const override {
        return 'f';
    }

    void OnEpochUpdate() override final {
        Cell::OnEpochUpdate();
    }

    std::shared_ptr<Cell> Clone() const override final {
        return std::make_shared<Food>(*this);
    }

private:
};

class Bug : public Cell {

public:
    Bug() : Cell(30, 0, 100) {}

    char Type() const override final {
        return type_;
    }

    void OnEpochUpdate() override final {
        Cell::OnEpochUpdate();
    }

    friend class World;

private:

    void set_type(char type) {
        type_ = type;
    }

    char type_ = 'b';

};

struct RandomBug : public Bug {
    ActionType Action(const Map &map) override {
        int i = rand() % 5;
        return static_cast<ActionType>(i);
    }

    std::shared_ptr<Cell> Clone() const override final {
        return std::make_shared<RandomBug>(*this);
    }
};

class World {
public:
    std::string Serialize() const {
        std::stringstream str;

        str << "Epoch: " << epoch_ << std::endl;

        for (size_t x = 0; x < world_.map.size(); ++x) {
            auto el = world_.map[x];

            for (size_t y = 0; y < el.size(); ++y) {
                str << el[y]->Type() << el[y]->points() << "\t";
            }
            str << std::endl;
        }

        return str.str();
    }

    bool IsValid() const {
        std::set<char> types;
        auto &world_map = world_.map;
        for (size_t x = 0; x < world_map.size(); ++x) {
            for (size_t y = 0; y < world_map[x].size(); ++y) {
                char type = world_map[x][y]->Type();
                if (type != Food{}.Type() && type != Poison{}.Type() && type != EmptyCell{}.Type()) {
                    types.insert(type);
                }
            }
        }

        return !types.empty();
    }

    void Step() {

        std::set<Cell *> activated_cells;
        auto &world_map = world_.map;
        for (size_t x = 0; x < world_map.size(); ++x) {
            for (size_t y = 0; y < world_map[x].size(); ++y) {

                auto &current_cell = world_map[x][y];

                if (activated_cells.count(current_cell.get()) != 0) {
                    continue;
                }
                activated_cells.insert(current_cell.get());

                if (world_.IsBound(x, y)) {
                    continue;
                }

                current_cell->OnEpochUpdate();

                if (current_cell->IsDied()) {
                    OnDied(current_cell);
                }

                Map view = world_.ViewRegion(x, y);

                // TODO: wait for
                auto action = Cell::ActionTypeToPair(current_cell->Action(view));
                auto &move_to = world_map[x + action.first][y + action.second];

                Move(current_cell, move_to);
            }
        }

        Update();
        ++epoch_;
    }

    World(size_t x, size_t y, size_t num_bugs, float init_food_prob, float init_poison_prob, float round_food_prob,
          float round_poison_prob) : world_(x, y), epoch_(0), round_food_prob_(round_food_prob),
                                     round_poison_prob_(round_poison_prob) {
        Init(num_bugs, init_food_prob, init_poison_prob);
    }

private:

    void OnDied(std::shared_ptr<Cell> &cell) {
        cell = std::make_shared<EmptyCell>();
    }

    void Move(std::shared_ptr<Cell> &active, std::shared_ptr<Cell> &passive) {
        if (active == passive) {
            return;
        }
        auto pa = active->points();
        auto pp = passive->points();

        if (pa > pp) {
            active->AddPoints(pp == 0 ? 0 : pa - pp);
            passive = std::move(active);
            OnDied(active);
        } else if (pa < pp) {
            passive->AddPoints(pp - pa);
            OnDied(active);
        } else {
            OnDied(passive);
            OnDied(active);
        }
    }

    void Update() {
        auto &map = world_.map;
        for (size_t x = 0; x < map.size(); ++x) {
            for (size_t y = 0; y < map[x].size(); ++y) {
                if (dis(gen) < round_food_prob_) {
                    if (map[x][y]->Type() == EmptyCell{}.Type()) {
                        map[x][y] = std::make_shared<Food>();
                    }
                } else if (dis(gen) < round_poison_prob_) {
                    if (map[x][y]->Type() == EmptyCell{}.Type()) {
                        map[x][y] = std::make_shared<Poison>();
                    }
                }
            }
        }
    }

    void Init(size_t num_bugs, float init_food_prob, float init_poison_prob) {
        const int mod = 1000;
        auto &map = world_.map;
        size_t cnt = 0;
        for (size_t x = 0; x < map.size(); ++x) {
            for (size_t y = 0; y < map[x].size(); ++y) {
                if (world_.IsBound(x, y)) {
                    map[x][y] = std::make_shared<Poison>();
                } else if (dis(gen) < init_food_prob) {
                    map[x][y] = std::make_shared<Food>();
                } else if (dis(gen) < init_poison_prob) {
                    map[x][y] = std::make_shared<Poison>();
                } else if ((dis(gen) < 0.03) && (cnt < num_bugs)) {
                    auto bug = std::make_shared<RandomBug>();
                    bug->set_type('r');
                    map[x][y] = std::move(bug);
                    ++cnt;
                } else {
                    map[x][y] = std::make_shared<EmptyCell>();
                }
            }
        }
    }

    Map world_;
    size_t epoch_ = 0;
    float round_food_prob_ = 0;
    float round_poison_prob_ = 0;
    std::random_device rd;
    std::mt19937 gen{rd()};
    std::uniform_real_distribution<float> dis{0, 1};

};

template<typename T>
void HttpServiceRun(SceneHolder<T> &holder, uint16_t port, std::string viewer_path) {

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

            session->close(restbed::OK, body);//, headers);
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


int main(int argc, char *argv[]) {

    size_t port = 0;
    std::string resources_path;
    long long timeout = 0;

    {
        cxxopts::Options options("bugs-world", "Cpp contest");
        options.add_options()
                ("t,timeout", "Sleep for between epochs", cxxopts::value<long long>()->default_value("300"))
                ("r,resources", "Path to viewer's resources",
                 cxxopts::value<std::string>())
                ("p,port", "Port number", cxxopts::value<size_t>()->default_value("1984"))
                ("h,help", "Print usage");
        auto result = options.parse(argc, argv);

        timeout = result["timeout"].as<long long>();
        resources_path = result["resources"].as<std::string>();
        port = result["port"].as<size_t>();

        std::cout << "Timeout: " << timeout << "ms" << std::endl;
        std::cout << "Resources: '" << resources_path << "'" << std::endl;
        std::cout << "Service port: " << port << std::endl;
    }

    auto world = std::make_unique<World>(50, 50, 30, 0.2, 0.05, 0.000005, 0);
    SceneHolder<std::string> holder;

    std::thread th(HttpServiceRun<std::string>, std::ref(holder), port, resources_path);

    bool is_alive = true;
    using namespace std::chrono;
    for (size_t i = 0; i < 1000 && is_alive; ++i) {
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        world->Step();
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        duration<double, std::milli> time_span = t2 - t1;

        std::cout << "It took me " << time_span.count() << " milliseconds." << std::endl;
        holder.Change(world->Serialize());
        is_alive = world->IsValid();
        std::this_thread::sleep_for(std::chrono::milliseconds(timeout));
        std::cout << i << "  ========" << std::endl;
    }

    th.join();
}
