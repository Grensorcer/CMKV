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

    bool swap(size_t i, size_t j)
    {
        auto &p1 = board_.at(i);
        auto &p2 = board_.at(j);
        auto res = p1.has_value() && p2.has_value() && !p1->is_stuck()
            && !p2->is_stuck();
        if (res)
            p1.swap(p2);

        return res;
    }

    bool play(size_t i, size_t j)
    {
        auto &p1 = tiles_.at(i);
        auto &p2 = board_.at(j);
        auto res = p1.has_value() and !p2.has_value();
        if (res)
            p1.swap(p2);

        return res;
    }

    bool remove(size_t i)
    {
        auto &p = board_.at(i);
        auto res = p.has_value() and !p->is_stuck();
        if (res)
            p.swap(tiles_.at(p->index()));

        return res;
    }

    void clear()
    {
        for (size_t i = 0; i < board_.size(); ++i)
            remove(i);
    }

    bool is_full() const
    {
        return std::all_of(board_.begin(), board_.end(),
                           [](std::optional<Tile> t) { return t.has_value(); });
    }

    size_t count() const
    {
        return tiles_.size();
    }

    size_t score() const
    {
        size_t score = 0;
        for (size_t i = 0; i < board_size_; ++i)
        {
            for (size_t j = 0; j < board_size_; ++j)
            {
                const auto &tile = board_[j + i * board_size_].value();

                const short down = i < board_size_ - 1
                    ? board_[j + (i + 1) * board_size_]->north()
                    : tile.south();

                const short right = j < board_size_ - 1
                    ? board_[j + 1 + i * board_size_]->west()
                    : tile.east();

                score += down != tile.south();
                score += right != tile.east();
            }
        }

        return score;
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
