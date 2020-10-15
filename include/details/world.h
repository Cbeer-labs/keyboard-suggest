#ifndef APP_WORLD_H
#define APP_WORLD_H

#include <array>
#include <memory>
#include <random>
#include <string>

#include "details/cell.h"
#include "world_map.h"

class World {
 public:
  struct Params {
    size_t width = 0;
    size_t height = 0;
    float food_prob = 0;
    float poison_prob = 0;
    float epoch_food_prob = 0;
    float epoch_poison_prob = 0;

    static Params SmallWorldParams() { return {20, 10, 0.2, 0.05, 5, 0}; }

    static Params DefaultWorldParams() { return {30, 20, 0.2, 0.07, 3, 0.3}; }

    static Params BigWorldParams() { return {50, 50, 0.2, 0.1, 5, 1}; }
  };

  std::string Serialize() const;

  bool IsValid() const;

  void Step();

  World(const Params& params);

  template <typename T>
  void AddBugs(size_t num_bugs) {
    static const std::array<char, 3> bug_types = {'r', 'z', 'a'};

    ++num_kind_;
    if (num_kind_ > bug_types.size()) {
      throw std::runtime_error("can't add new kind");
    }

    auto& map = world_.map;
    size_t cnt = 0;
    for (size_t x = 0; x < map.size(); ++x) {
      for (size_t y = 0; y < map[x].size(); ++y) {
        if (random_() < 0.03 && (cnt < num_bugs)) {
          if (map[x][y]->Type() != EmptyCell{}.Type()) {
            continue;
          }

          auto bug = std::make_unique<T>();
          bug->set_type(bug_types[num_kind_ - 1]);
          map[x][y] = std::move(bug);
          ++cnt;
        }
      }
    }
  }

 private:
  void OnDied(Cell::Ptr& cell);

  void Move(Cell::Ptr& active, Cell::Ptr& passive);

  void Update();

  void Init(float init_food_prob, float init_poison_prob);

  WorldMap world_;
  size_t epoch_ = 0;
  float round_food_prob_ = 0;
  float round_poison_prob_ = 0;

  struct RandomWrapper {
    std::random_device rd;
    mutable std::mt19937 gen{rd()};
    mutable std::uniform_real_distribution<float> dis{0, 1};

    float operator()() const { return dis(gen); }
  };

  RandomWrapper random_;
  size_t num_kind_ = 0;
};

#endif  // APP_WORLD_H
