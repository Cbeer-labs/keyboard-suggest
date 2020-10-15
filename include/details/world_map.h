//
// Created by drewxa on 15.10.2020.
//

#ifndef APP_WORLD_MAP_H
#define APP_WORLD_MAP_H

#include <memory>

#include "matrix.h"
#include "details/cell.h"

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



#endif  // APP_WORLD_MAP_H
