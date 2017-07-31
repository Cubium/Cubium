#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <random>

int rand(int low, int high)
{
  static std::random_device rd;
  static std::mt19937 mt(rd());
  std::uniform_int_distribution<> dist(low, high);
  return dist(mt);
}

#endif
