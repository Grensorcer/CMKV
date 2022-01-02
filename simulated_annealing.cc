#include <iostream>
#include <fstream>
#include "board.hh"
#include "solvers.hh"

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
        std::cout.precision(2);
        Board b(argv[1]);
        bool win = simulated_annealing(b, 0.001);
        std::cout << "Game state: " << (win ? "Solved" : "Unsolved")
                  << std::endl
#ifndef NDEBUG
                  << b;
#else
            ;
#endif

        if (win)
        {
            auto out = std::ofstream(argv[2]);
            out << b;
        }
    }
    return return_code;
}
