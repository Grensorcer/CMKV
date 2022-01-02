#pragma once
#include <iostream>

class Tile
{
public:
    Tile(short north, short west, short east, short south, size_t idx,
         bool stuck = false)
        : north_{ north }
        , west_{ west }
        , east_{ east }
        , south_{ south }
        , idx_{ idx }
        , stuck_{ stuck }
    {}

    size_t index() const
    {
        return idx_;
    }

    bool is_stuck() const
    {
        return stuck_;
    }

    short north() const
    {
        return north_;
    }

    short south() const
    {
        return south_;
    }

    short east() const
    {
        return east_;
    }

    short west() const
    {
        return west_;
    }

    friend std::ostream &operator<<(std::ostream &os, const Tile &tile);

protected:
    short north_;
    short west_;
    short east_;
    short south_;
    size_t idx_;
    bool stuck_;
};
