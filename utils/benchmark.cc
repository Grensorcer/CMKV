#include <array>
#include <chrono>
#include <utility>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "solvers.hh"

using namespace std::chrono;

const auto board_path = std::string("../utils/ins/");
const auto board_names_easy = std::array<const char *, 1>{ "medium3.in" };
const auto board_names_hard =
    std::array<const char *, 4>{ "medium4.in", "medium5.in", "gen5.in",
                                 "gen6.in" };

auto measure = [](auto &&fn, const char *name, auto nb_iter,
                  auto &&...parameters) {
    auto board = Board(board_path + name);
    const auto &start = high_resolution_clock::now();
    auto fwd_fn = std::forward<decltype(fn)>(fn);

    size_t victories = 0;
    for (auto i = 0; i < nb_iter; ++i)
        victories +=
            fwd_fn(board, std::forward<decltype(parameters)>(parameters)...)
            ? 1
            : 0;

    const auto &stop = high_resolution_clock::now();

    return std::make_pair<>(((stop - start) / nb_iter).count(), victories);
};

auto bench_easy = [](auto &&fn, const char *name, auto nb_iter,
                     auto &&...parameters) {
    std::ostringstream times;
    std::ostringstream successes;

    times << name;
    ((times << "_" << std::forward<decltype(parameters)>(parameters)), ...);
    times << " ; " << nb_iter << " ;";
    successes << name;
    ((successes << "_" << std::forward<decltype(parameters)>(parameters)), ...);
    successes << " ; " << nb_iter << " ;";
    for (const auto &b_name : board_names_easy)
    {
        std::pair<float, size_t> result =
            measure(fn, b_name, nb_iter, parameters...);
        std::cout << name << ' ' << b_name << '\n';
        times << result.first << " ;";
        successes << result.second << " ;";
    }
    for (const auto &_ : board_names_hard)
    {
        times << ";";
        successes << ";";
    }

    times << '\n';
    successes << '\n';
    return std::make_pair<>(times.str(), successes.str());
};

auto bench_hard = [](auto &&fn, const char *name, auto nb_iter,
                     auto &&...parameters) {
    std::ostringstream times;
    std::ostringstream successes;

    times << name;
    ((times << "_" << std::forward<decltype(parameters)>(parameters)), ...);
    times << " ; " << nb_iter << " ;";
    successes << name;
    ((successes << "_" << std::forward<decltype(parameters)>(parameters)), ...);
    successes << " ; " << nb_iter << " ;";
    for (const auto &b_name : board_names_easy)
    {
        std::pair<float, size_t> result =
            measure(fn, b_name, nb_iter, parameters...);
        std::cout << name << ' ' << b_name << '\n';
        times << result.first << " ;";
        successes << result.second << " ;";
    }
    for (const auto &b_name : board_names_hard)
    {
        std::pair<float, size_t> result =
            measure(fn, b_name, nb_iter, parameters...);
        std::cout << name << ' ' << b_name << '\n';
        times << result.first << " ;";
        successes << result.second << " ;";
    }

    times << '\n';
    successes << '\n';
    return std::make_pair<>(times.str(), successes.str());
};

void add_entry(std::ofstream &times, std::ofstream &successes,
               std::pair<std::string, std::string> res)
{
    times << res.first;
    successes << res.second;
}

int main(int argc, char **argv)
{
    if (argc < 2)
        return 1;

    auto times = std::ofstream(argv[1] + std::string("_chrono.csv"));
    auto successes = std::ofstream(argv[1] + std::string("_success.csv"));
    times << "bench ; nb_iter ;";
    successes << "bench ; nb_iter ;";

    for (const auto &name : board_names_easy)
    {
        times << ' ' << name << ';';
        successes << ' ' << name << ';';
    }

    for (const auto &name : board_names_hard)
    {
        times << ' ' << name << ';';
        successes << ' ' << name << ';';
    }

    times << '\n';
    successes << '\n';

    add_entry(times, successes, bench_easy(bruteforce, "bruteforce", 100, 0));
    add_entry(times, successes,
              bench_hard(simulated_annealing, "simulated_annealing", 1, 0.001,
                         10000, 100, 1));

    return 0;
}