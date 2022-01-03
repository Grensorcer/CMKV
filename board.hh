#pragma once
#include <algorithm>
#include <random>
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <optional>
#include "tile.hh"

class Board
{
public:
    Board(const std::string &file);

    bool swap(size_t i, size_t j);
    bool play(size_t i, size_t j);
    bool remove(size_t i);
    void random_initialize();
    void clear();
    bool is_full() const;
    size_t count() const;
    size_t score() const;
    friend std::ostream &operator<<(std::ostream &os, const Board &board);

protected:
    std::vector<std::optional<Tile>> tiles_;
    std::vector<std::optional<Tile>> board_;
    size_t board_size_;
};
