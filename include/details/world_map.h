#ifndef APP_WORLD_MAP_H
#define APP_WORLD_MAP_H

#include <memory>

#include "details/cell.h"
#include "matrix.h"

class WorldMap {
 public:
  WorldMap();

  WorldMap(size_t x, size_t y);

  WorldMap ViewRegion(size_t x, size_t y) const;

  bool IsBound(size_t x, size_t y) const;

  friend class World;
 private:
  Matrix<std::shared_ptr<Cell>> map;
};

#endif  // APP_WORLD_MAP_H
