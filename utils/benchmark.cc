#include <array>
#include <chrono>
#include <utility>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "../solvers.hh"

using namespace std::chrono;

const auto board_path = std::string("utils/ins/");
auto board_array = std::array<std::vector<std::filesystem::path>, 5>{};

auto measure = [](auto &&fn, const std::string &path, auto nb_iter,
                  auto &&...parameters) {
    auto board = Board(path);
    const auto &start = high_resolution_clock::now();
    auto fwd_fn = std::forward<decltype(fn)>(fn);

    size_t victories = 0;
    for (auto i = 0; i < nb_iter; ++i)
        victories +=
            fwd_fn(board, std::forward<decltype(parameters)>(parameters)...)
            ? 1
            : 0;

    const auto &stop = high_resolution_clock::now();

    return std::make_pair<>(
        (std::chrono::duration_cast<std::chrono::microseconds>(stop - start)
         / nb_iter)
            .count(),
        victories);
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

    for (short i = 2; i < 4; ++i)
    {
        double time_ms = 0.;
        size_t victories = 0;
        for (size_t j = 0; j < board_array[i - 2].size(); ++j)
        {
            const auto &b_path = board_array[i - 2][j];
            std::pair<double, size_t> result =
                measure(fn, b_path.string(), nb_iter, parameters...);

            time_ms += result.first;
            victories += result.second;
            std::cout << name << ' ' << b_path.stem().string() << '\n';
        }

        times << time_ms / (double)board_array[i - 2].size() << " ;";
        successes << (double)victories
                / (double)(nb_iter * board_array[i - 2].size())
                  << " ;";
    }

    for (short i = 4; i <= 6; ++i)
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

    for (short i = 2; i <= 6; ++i)
    {
        double time_ms = 0.;
        size_t victories = 0;
        for (size_t j = 0; j < board_array[i - 2].size(); ++j)
        {
            const auto &b_path = board_array[i - 2][j];
            std::pair<double, size_t> result =
                measure(fn, b_path.string(), nb_iter, parameters...);

            time_ms += result.first;
            victories += result.second;
            std::cout << name << ' ' << b_path.stem().string() << '\n';
        }

        times << time_ms / (double)board_array[i - 2].size() << " ;";
        successes << (double)victories
                / (double)(nb_iter * board_array[i - 2].size())
                  << " ;";
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
    {
        std::cerr << "Not enough arguments\n";
        return 1;
    }

    auto bench_path = std::string("utils/bench/") + argv[1];
    std::filesystem::create_directory(bench_path);

    auto times = std::ofstream(bench_path + "/chrono.csv");
    auto successes = std::ofstream(bench_path + "/success.csv");
    times << "bench ; nb_iter ;";
    successes << "bench ; nb_iter ;";

    for (short i = 2; i <= 6; ++i)
    {
        for (const auto &board_entry : std::filesystem::directory_iterator(
                 board_path + std::to_string(i)))
            board_array[i - 2].push_back(board_entry.path());
        times << ' ' << i << ';';
        successes << ' ' << i << ';';
    }

    times << '\n';
    successes << '\n';

    add_entry(times, successes, bench_easy(bruteforce, "bruteforce", 100, 0));
    add_entry(times, successes,
              bench_hard(simulated_annealing, "simulated_annealing", 10, 0.001,
                         10000, 100, 1));

    return 0;
}