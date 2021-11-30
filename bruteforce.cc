#include <iostream>
#include "board.hh"

bool bruteforce_(Board &board, size_t idx)
{
    if (idx == board.count())
        return board.score() == 0;

    bool win = false;
    for (size_t j = 0; !win && j < board.count(); ++j)
    {
        board.remove(idx);
        if (board.play(j, idx))
            win = bruteforce_(board, idx + 1);
    }

    if (!win)
        board.remove(idx);
    return win;
}

int main(int argc, char **argv)
{
    int return_code = 0;
    if (argc != 3)
    {
        std::cerr << "There should be 2 arguments: in.txt and out.txt."
                  << std::endl;
        return_code = 1;
    }
    else
    {
        Board b(argv[1]);
        bool win = bruteforce_(b, 0);
        b.play(0, 0);
        b.play(3, 1);
        b.play(1, 2);
        b.play(2, 3);

        std::cout << "Game state: " << (win ? "Solved" : "Unsolved")
                  << std::endl
                  << b;
    }
    return return_code;
}