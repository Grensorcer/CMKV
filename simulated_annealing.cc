#include <iostream>
#include <memory>
#include <random>
#include <cmath>
#include "board.hh"

template <typename DistributionType>
class Distribution
{
public:
    Distribution(std::shared_ptr<std::mt19937> gen, float a, float b)
        : gen_{ gen }
    {
        distribution_ = DistributionType(a, b);
    }

    void update(float a, float b)
    {
        distribution_ = DistributionType(a, b);
    }

    float operator()()
    {
        return distribution_(*gen_);
    }

protected:
    std::shared_ptr<std::mt19937> gen_;
    DistributionType distribution_;
};

bool simulated_annealing(Board &board, float decrease_factor, unsigned max_iterations=100000, unsigned stuck_max=100)
{
    auto gen = std::make_shared<std::mt19937>(std::random_device{}());
    auto uniform_index =
        Distribution<std::uniform_int_distribution<>>(gen, 0, board.count() - 1);
    auto uniform_distrib =
        Distribution<std::uniform_real_distribution<float>>(gen, 0, 1);

    board.random_initialize();
    unsigned stuck = 0;
    float sigma = 10.0;

    for (unsigned i = 0; i < max_iterations && board.score() != 0; i++)
    {
        auto old_score = board.score();
        unsigned index1 = uniform_index();
        unsigned index2 = index1;
        while (index2 == index1)
            index2 = uniform_index();
        board.swap(index1, index2);
        auto new_score = board.score();
        if (uniform_distrib() < std::exp((new_score - old_score) / sigma))
        {
            if (new_score >= old_score)
            {
                stuck++;
                if (stuck > stuck_max)
                    sigma += 2.0;
            }
        }
        else
            board.swap(index1, index2);
        sigma *= 1 - decrease_factor;

        std::cout << board << std::endl;
    }
    return board.score() == 0;
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
        bool win = simulated_annealing(b, 0.01);
        std::cout << "Game state: " << (win ? "Solved" : "Unsolved")
                  << std::endl
                  << b;
    }
    return return_code;
}
