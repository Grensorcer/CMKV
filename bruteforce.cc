#include <iostream>
#include "board.hh"

bool bruteforce(Board &board)
{
    for (size_t i = 0; i < board.count(); ++i)
    {
        for (size_t j = 0; i < board.count(); ++j)
        {
            board.play(i, j);
        }
    }
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
        std::cout << b << std::endl;
        b.play(0, 1);
        b.play(1, 0);
        std::cout << b << std::endl;
        b.play(1, 1);
        b.play(2, 2);
        std::cout << b << std::endl;
        b.swap(0, 1);
        b.swap(1, 2);
        std::cout << b << std::endl;
        b.remove(0);
        b.remove(1);
        std::cout << b << std::endl;
        b.play(2, 1);
        b.play(3, 3);
        std::cout << b << std::endl;
    }
    return return_code;
}