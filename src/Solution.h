#ifndef SOLUTION_H
#define SOLUTION_H

#include <iostream>
#include "Data.h"

struct Solution {
  std::vector<int> sequence;
  std::vector<int> accumulatedTime;
  std::vector<int> accumulatedFine;
  int qtProductsWithFine;
  int fine;
};

void showSolutionData(const Solution *s);

void showSolution(const Solution *s);

void attSolution(const Data *data, Solution *s, int start);

void calculateTimes(const Data *data, Solution *s);

void calculatePenalties(const Data *data, Solution *s);

#endif
