#include <fstream>
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
        bool win = simulated_annealing_progressive(b, 0.001);
        // bool win = bruteforce(b, 0);
        std::cout << "Game state: " << (win ? "Solved" : "Unsolved")
                  << std::endl
#ifdef MYDEBUG
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
