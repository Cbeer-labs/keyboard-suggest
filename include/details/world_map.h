#ifndef APP_WORLD_MAP_H
#define APP_WORLD_MAP_H

#include "details/cell.h"
#include "matrix.h"

class WorldMap {
 public:
  WorldMap();
  WorldMap(const WorldMap&) = delete;
  WorldMap(WorldMap&&) noexcept  = default;
  WorldMap& operator=(const WorldMap&) = delete;
  WorldMap& operator=(WorldMap&&) noexcept  = default;

  WorldMap(size_t x, size_t y);

  WorldMap ViewRegion(size_t x, size_t y) const;

  bool IsBound(size_t x, size_t y) const;

  friend class World;
 private:
  Matrix<Cell::Ptr> map;
};

#endif  // APP_WORLD_MAP_H
