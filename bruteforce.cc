#include <iostream>
#include "board.hh"

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
        bool win = bruteforce(b, 0);
        std::cout << "Game state: " << (win ? "Solved" : "Unsolved")
                  << std::endl
                  << b;
    }
    return return_code;
}