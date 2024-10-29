#ifndef SWAPNEIGHBORHOOD_H
#define SWAPNEIGHBORHOOD_H

#include <iostream>
#include "Data.h"
#include "Solution.h"


bool SwapWasGood(const Data *data, Solution *s, int range, int i, int j);

bool Swap(const Data*data, Solution *s, int range);

#endif