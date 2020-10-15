#ifndef APP_WORLD_H
#define APP_WORLD_H

#include <memory>
#include <random>
#include <string>

#include "details/cell.h"
#include "world_map.h"

class World {
 public:
  std::string Serialize() const;

  bool IsValid() const;

  void Step();

  World(size_t x, size_t y, float init_food_prob, float init_poison_prob, float round_food_prob,
        float round_poison_prob);

  template <typename T>
  void AddBugs(size_t num_bugs, char type) {
    auto& map = world_.map;
    size_t cnt = 0;
    for (size_t x = 0; x < map.size(); ++x) {
      for (size_t y = 0; y < map[x].size(); ++y) {
        if (random_() < 0.03 && (cnt < num_bugs)) {
          if (map[x][y]->Type() != EmptyCell{}.Type()) {
            continue;
          }

          auto bug = std::make_shared<T>();
          bug->set_type(type);
          map[x][y] = std::move(bug);
          ++cnt;
        }
      }
    }
  }

 private:
  void OnDied(std::shared_ptr<Cell>& cell);

  void Move(std::shared_ptr<Cell>& active, std::shared_ptr<Cell>& passive);

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
};

#endif  // APP_WORLD_H
