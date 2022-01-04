#include <iostream>
#include <algorithm>
#include <memory>
#include <random>
#include <vector>
#include <cmath>
#include "utils.hh"
#include "solvers.hh"

bool bruteforce(Board &board, size_t idx)
{
    if (idx == board.count())
        return board.score() == 0;

    bool win = false;
    for (size_t j = 0; !win && j < board.count(); ++j)
    {
#ifdef MYDEBUG
        if (idx < 4)
            std::cout << idx << ": " << j << '\n';
#endif
        board.remove(idx);
        if (board.play(j, idx))
            win = bruteforce(board, idx + 1);
    }

    if (!win)
        board.remove(idx);
    return win;
}

bool simulated_annealing_binarysearch(Board &board, float decrease_factor,
                                      unsigned max_iterations,
                                      float max_temperature,
                                      float min_temperature)
{
    double min_std = 0.1;
    double max_std = 100.;

    auto gen = std::make_shared<std::mt19937>(std::random_device{}());
    auto uniform_index = Distribution<std::uniform_int_distribution<>>(
        gen, 0, board.count() - 1);
    auto uniform_distrib =
        Distribution<std::uniform_real_distribution<float>>(gen, 0, 1);

    board.random_initialize();
    double sigma = (max_temperature + min_temperature) / 2.;
    auto errors = std::vector<double>();
    errors.reserve(max_iterations);

    unsigned j;
    for (j = 0; j < 10000 && sigma <= max_temperature
         && sigma >= min_temperature && board.score() != 0;
         ++j)
    {
#ifdef MYDEBUG
        std::cout << "Tmp: " << sigma << " Min: " << min_temperature
                  << " Max: " << max_temperature;
#endif

        for (unsigned i = 0; i < max_iterations && board.score() != 0; ++i)
        {
            long old_score = board.score();
            unsigned index1 = uniform_index();
            unsigned index2 = index1;
            while (index2 == index1)
                index2 = uniform_index();
            board.swap(index1, index2);
            long new_score = board.score();
            auto tmp = std::exp((old_score - new_score) / sigma);

            if (uniform_distrib() > tmp)
                board.swap(index1, index2);

            sigma *= 1 - decrease_factor;
            errors.emplace_back(tmp > 1 ? 1 : tmp);
        }
#ifdef MYDEBUG
        std::cout << " Score: " << board.score() << std::endl;
#endif

        double mean = std::accumulate(errors.begin(), errors.end(), 0.)
            / (double)errors.size();
        // std::cout << mean << std::endl;

        std::transform(errors.begin(), errors.end(), errors.begin(),
                       [&](double x) { return (x - mean) * (x - mean); });

        /*
        for (auto v : errors)
            std::cout << v << " ";
        std::cout << std::endl;
        */

        double std =
            std::sqrt(std::accumulate(errors.begin(), errors.end(), 0.));
        auto expected_std = (min_std + max_std) / 2.;
        // std::cout << std << std::endl;

        if (std <= expected_std)
        {
            max_std = expected_std;
            max_temperature = (min_temperature + max_temperature) / 2;
        }
        else
        {
            min_std = expected_std;
            min_temperature = (min_temperature + max_temperature) / 2;
        }

        sigma = (min_temperature + max_temperature) / 2.;
        errors.clear();
    }

#ifdef MYDEBUG
    std::cout << "Iterations: " << j << " Temperature: " << sigma
              << " Deviation: " << (min_std + max_std) / 2. << std::endl;
#endif
    return board.score() == 0;
}

bool simulated_annealing_stuck(Board &board, float decrease_factor,
                               unsigned max_iterations, unsigned stuck_max,
                               float initial_temperature)
{
    auto gen = std::make_shared<std::mt19937>(std::random_device{}());
    auto uniform_index = Distribution<std::uniform_int_distribution<>>(
        gen, 0, board.count() - 1);
    auto uniform_distrib =
        Distribution<std::uniform_real_distribution<float>>(gen, 0, 1);

    board.random_initialize();
    unsigned stuck = 0;
    float sigma = initial_temperature;
    unsigned unstuck_count = 0;

    unsigned i;
    for (i = 0; i < max_iterations && board.score() != 0; i++)
    {
        long old_score = board.score();
        unsigned index1 = uniform_index();
        unsigned index2 = index1;
        while (index2 == index1)
            index2 = uniform_index();
        board.swap(index1, index2);
        long new_score = board.score();
        auto tmp = std::exp((old_score - new_score) / (double)(sigma));
        if (uniform_distrib() > tmp)
        {
            stuck++;
            if (stuck > stuck_max)
            {
                if (unstuck_count == 50)
                {
                    sigma = initial_temperature;
                    unstuck_count = 0;
                }
                else
                {
                    sigma += 1.0;
                    unstuck_count++;
                }
                stuck = 0;
            }
            board.swap(index1, index2);
        }
        sigma *= 1 - decrease_factor;
#ifdef MYDEBUG
        std::cout << sigma << " old: " << old_score << " new: " << new_score
                  << " " << tmp << std::endl;
#endif
    }
    return board.score() == 0;
}

bool simulated_annealing_progressive(Board &board, float decrease_factor,
                                     unsigned max_iterations,
                                     unsigned stuck_max, unsigned step,
                                     float initial_temperature)
{
    auto gen = std::make_shared<std::mt19937>(std::random_device{}());
    auto uniform_index = Distribution<std::uniform_int_distribution<>>(
        gen, 0, board.count() - 1);
    auto uniform_distrib =
        Distribution<std::uniform_real_distribution<float>>(gen, 0, 1);

    board.random_initialize();
    float sigma = initial_temperature;
    unsigned stuck = 0;
    unsigned count = 0;

    unsigned i;
    for (i = 0; i < max_iterations && board.score() != 0; i++)
    {
        long old_score = board.score();
        unsigned index1 = uniform_index();
        unsigned index2 = index1;
        while (index2 == index1)
            index2 = uniform_index();
        board.swap(index1, index2);
        long new_score = board.score();
        auto tmp = std::exp((old_score - new_score) / (double)(sigma));
        if (uniform_distrib() > tmp)
        {
            stuck++;
            if (stuck > stuck_max)
            {
                count++;
                stuck = 0;
                /*
                if (count % 40)
                    sigma += 20.0;
                else if (count % 20)
                    sigma += 10.0;
                else if (count % 10)
                    sigma += 5.0;
                else
                    sigma += 1.0;
                */
                if (count % step == 0)
                    sigma += count / step;
            }
            board.swap(index1, index2);
        }
        sigma *= 1 - decrease_factor;
#ifdef MYDEBUG
        std::cout << sigma << " old: " << old_score << " new: " << new_score
                  << " " << tmp << std::endl;
#endif
    }
    return board.score() == 0;
}