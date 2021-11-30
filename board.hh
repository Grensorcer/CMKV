#pragma once
#include <algorithm>
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <optional>
#include "tile.hh"

class Board
{
public:
    Board(const char *file)
    {
        std::ifstream board_stream(file);
        std::string input;
        char north, west, east, south;
        size_t i = 0;
        while (board_stream >> north >> west >> east >> south)
        {
            bool stuck = false;
            while (char(board_stream.peek()) == ' ')
                board_stream.get();
            if (board_stream.peek() == '@')
            {
                stuck = true;
                board_stream.get();
            }
            tiles_.push_back(std::make_optional<Tile>(
                north - '0', west - '0', east - '0', south - '0', i++, stuck));
        }

        board_size_ = std::sqrt(tiles_.size());
        board_.reserve(board_size_);
        for (size_t i = 0; i < tiles_.size(); ++i)
        {
            board_.push_back(std::nullopt);
            if (tiles_[i]->is_stuck())
                tiles_[i].swap(board_[i]);
        }
    }

    void swap(size_t i, size_t j)
    {
        auto &p1 = board_.at(i);
        auto &p2 = board_.at(j);
        if (p1.has_value() && p2.has_value() && !p1->is_stuck()
            && !p2->is_stuck())
            p1.swap(p2);
    }

    void play(size_t i, size_t j)
    {
        auto &p1 = tiles_.at(i);
        auto &p2 = board_.at(j);
        if (p1.has_value() and !p2.has_value())
            p1.swap(p2);
    }

    void remove(size_t i)
    {
        auto &p = board_.at(i);
        if (p.has_value() and !p->is_stuck())
            p.swap(tiles_.at(p->index()));
    }

    bool is_full()
    {
        return std::all_of(board_.begin(), board_.end(),
                           [](std::optional<Tile> t) { return t.has_value(); });
    }

    size_t count()
    {
        return tiles_.size();
    }

    friend std::ostream &operator<<(std::ostream &os, const Board &board);

protected:
    std::vector<std::optional<Tile>> tiles_;
    std::vector<std::optional<Tile>> board_;
    size_t board_size_;
};

std::ostream &operator<<(std::ostream &os, const Board &board)
{
    for (const auto &tile : board.board_)
        if (tile.has_value())
            os << *tile << std::endl;
        else
            os << "null" << std::endl;

    return os;
}
