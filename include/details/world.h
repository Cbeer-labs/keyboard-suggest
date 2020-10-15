//
// Created by drewxa on 15.10.2020.
//

#ifndef APP_WORLD_H
#define APP_WORLD_H

#include <memory>
#include <random>
#include <set>
#include <sstream>
#include <string>

#include "details/cell.h"
#include "world_map.h"

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
    auto& world_map = world_.map;
    for (size_t x = 0; x < world_map.size(); ++x) {
      for (size_t y = 0; y < world_map[x].size(); ++y) {
        char type = world_map[x][y]->Type();
        if (type != Food{}.Type() && type != Poison{}.Type() &&
            type != EmptyCell{}.Type()) {
          types.insert(type);
        }
      }
    }

    return !types.empty();
  }

  void Step() {
    std::set<Cell*> activated_cells;
    auto& world_map = world_.map;
    for (size_t x = 0; x < world_map.size(); ++x) {
      for (size_t y = 0; y < world_map[x].size(); ++y) {
        auto& current_cell = world_map[x][y];

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
        auto& move_to = world_map[x + action.first][y + action.second];

        Move(current_cell, move_to);
      }
    }

    Update();
    ++epoch_;
  }

  World(size_t x,
        size_t y,
        float init_food_prob,
        float init_poison_prob,
        float round_food_prob,
        float round_poison_prob)
      : world_(x, y),
        epoch_(0),
        round_food_prob_(round_food_prob),
        round_poison_prob_(round_poison_prob) {
    Init(init_food_prob, init_poison_prob);
  }

  template <typename T>
  void AddBugs(size_t num_bugs, char type) {
    auto& map = world_.map;
    size_t cnt = 0;
    for (size_t x = 0; x < map.size(); ++x) {
      for (size_t y = 0; y < map[x].size(); ++y) {
        if (random_() < 0.03 && (cnt < num_bugs)) {
          auto bug = std::make_shared<T>();
          bug->set_type(type);
          map[x][y] = std::move(bug);
          ++cnt;
        }
      }
    }
  }

 private:
  void OnDied(std::shared_ptr<Cell>& cell) {
    cell = std::make_shared<EmptyCell>();
  }

  void Move(std::shared_ptr<Cell>& active, std::shared_ptr<Cell>& passive) {
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
    auto& map = world_.map;
    for (size_t x = 0; x < map.size(); ++x) {
      for (size_t y = 0; y < map[x].size(); ++y) {
        if (random_() < round_food_prob_) {
          if (map[x][y]->Type() == EmptyCell{}.Type()) {
            map[x][y] = std::make_shared<Food>();
          }
        } else if (random_() < round_poison_prob_) {
          if (map[x][y]->Type() == EmptyCell{}.Type()) {
            map[x][y] = std::make_shared<Poison>();
          }
        }
      }
    }
  }

  void Init(float init_food_prob, float init_poison_prob) {
    const int mod = 1000;
    auto& map = world_.map;
    size_t cnt = 0;
    for (size_t x = 0; x < map.size(); ++x) {
      for (size_t y = 0; y < map[x].size(); ++y) {
        if (world_.IsBound(x, y)) {
          map[x][y] = std::make_shared<Poison>();
        } else if (random_() < init_food_prob) {
          map[x][y] = std::make_shared<Food>();
        } else if (random_() < init_poison_prob) {
          map[x][y] = std::make_shared<Poison>();
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

  struct RandomWrapper {
    std::random_device rd;
    mutable std::mt19937 gen{rd()};
    mutable std::uniform_real_distribution<float> dis{0, 1};

    float operator()() const { return dis(gen); }
  };

  RandomWrapper random_;
};

#endif  // APP_WORLD_H
