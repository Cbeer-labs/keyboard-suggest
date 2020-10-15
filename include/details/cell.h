#ifndef APP_CELL_H
#define APP_CELL_H

#include <memory>

class WorldMap;

enum class ActionType { Left, Right, Top, Bottom, Stay };

class Cell {
 public:
  using Ptr = std::shared_ptr<Cell>;

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

  virtual ActionType Action(const WorldMap &) = 0;

  virtual char Type() const = 0;

  virtual void OnEpochUpdate() {
    if (points_ > 0) AddPoints(-1);
    if (points_ < 0) AddPoints(1);
  }

  int points() const { return points_; }

  bool IsDied() const { return points_ == 0; }

  virtual std::shared_ptr<Cell> Clone() const = 0;

  Cell(int points, int min_points, int max_points)
      : points_(points), min_points_(min_points), max_points_(max_points) {}

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

class EmptyCell : public Cell {
 public:
  ActionType Action(const WorldMap &) override final { return ActionType::Stay; }

  EmptyCell() : Cell(0, 0, 0) {}

  char Type() const override final { return ' '; }

  std::shared_ptr<Cell> Clone() const override final { return std::make_shared<EmptyCell>(*this); }

  void OnEpochUpdate() override final {}
};

class Poison : public Cell {
 public:
  ActionType Action(const WorldMap &) override final { return ActionType::Stay; }

  Poison() : Cell(1000, 1000, 1000) {}

  char Type() const override final { return 'p'; }

  std::shared_ptr<Cell> Clone() const override final { return std::make_shared<Poison>(*this); }

  void OnEpochUpdate() override final {}
};

class Food : public Cell {
 public:
  ActionType Action(const WorldMap &) override final { return ActionType::Stay; }

  Food() : Cell(-10, -10, 0) {}

  char Type() const override { return 'f'; }

  void OnEpochUpdate() override final { Cell::OnEpochUpdate(); }

  std::shared_ptr<Cell> Clone() const override final { return std::make_shared<Food>(*this); }
};
#endif  // APP_CELL_H
