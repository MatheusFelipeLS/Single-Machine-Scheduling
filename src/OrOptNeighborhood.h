#ifndef OROPTNEIGHBORHOOD_H
#define OROPTNEIGHBORHOOD_H

#include <iostream>
#include <algorithm>
#include "Data.h"
#include "Solution.h"

bool FrontalOrOptWasGood(const Data *data, Solution *s, int range, int i, int j, int *auxTime, int *auxFine);

bool BackOrOptWasGood(const Data *data, Solution *s, int range, int i, int j);

bool OrOpt(const Data* data, Solution *s, int range);

#endif