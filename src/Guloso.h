#ifndef GULOSO_H
#define GULOSO_H

#include <iostream>
#include "Data.h"
#include "Solution.h"

struct fineToInsert {
  int node;
  int fine;
};


bool check(fineToInsert i, fineToInsert j);

Solution Guloso(const Data *data, double alpha);

#endif