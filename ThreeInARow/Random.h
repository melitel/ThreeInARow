#pragma once
#include <random>

inline std::random_device rd; // obtain a random number from hardware
inline std::mt19937 gen{ rd() }; // seed the generator
inline std::uniform_int_distribution<> distr{ 0, 4 }; // define the range