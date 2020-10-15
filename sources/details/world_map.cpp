#include "details/world_map.h"

WorldMap::WorldMap() : map(0, 0) {}

WorldMap::WorldMap(size_t x, size_t y) : map(x, y) {}

WorldMap WorldMap::ViewRegion(size_t x, size_t y) const {
  WorldMap view;
  view.map = Matrix<Cell::Ptr>{
      {map[x - 1][y - 1]->Clone(), map[x][y - 1]->Clone(), map[x + 1][y - 1]->Clone()},
      {map[x - 1][y - 0]->Clone(), map[x][y - 0]->Clone(), map[x + 1][y - 0]->Clone()},
      {map[x - 1][y + 1]->Clone(), map[x][y + 1]->Clone(), map[x + 1][y + 1]->Clone()},
  };
  return view;
}

bool WorldMap::IsBound(size_t x, size_t y) const {
  return x == 0 || y == 0 || (x + 1) == map.size() || (y + 1) == map[x].size();
}
