#ifndef ILS_H
#define ILS_H

#include <iostream>
#include <bits/stdc++.h>

#include "Data.h"
#include "Solution.h"
#include "Guloso.h"
#include "LocalSearch.h"
#include "Perturbacao.h"

Solution ILS(const Data *data, int max_iter, int maxIterIls);

#endif