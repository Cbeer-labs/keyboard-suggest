//
// Created by drewxa on 15.10.2020.
//

#ifndef APP_BUG_H
#define APP_BUG_H

#include "details/cell.h"

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


#endif //APP_BUG_H
