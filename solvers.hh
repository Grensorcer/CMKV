#pragma once
#include "board.hh"

bool bruteforce(Board &board, size_t idx);
bool simulated_annealing_binarysearch(Board &board, float decrease_factor,
                                      unsigned max_iterations = 10000,
                                      float max_temperature = 100.0,
                                      float min_temperature = 1);
bool simulated_annealing_stuck(Board &board, float decrease_factor,
                               unsigned max_iterations = 10000000,
                               unsigned stuck_max = 1000,
                               float initial_temperature = 20.0);
bool simulated_annealing_progressive(Board &board, float decrease_factor,
                                     unsigned max_iterations = 100000000,
                                     unsigned stuck_max = 10000,
                                     unsigned step = 30,
                                     float initial_temperature = 20.0);