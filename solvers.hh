#pragma once
#include "board.hh"

bool bruteforce(Board &board, size_t idx);
bool simulated_annealing(Board &board, float decrease_factor,
                         unsigned max_iterations = 10000,
                         float max_temperature = 100.0,
                         float min_temperature = 1);