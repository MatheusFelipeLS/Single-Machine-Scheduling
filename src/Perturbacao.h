#ifndef PERTURBACAO_H
#define PERTURBACAO_H

#include <iostream>
#include <algorithm>

#include "Data.h"
#include "Solution.h"

template<typename IT>
void swap_ranges(IT start_a, IT end_a, IT start_b, IT end_b) {
    auto it = std::rotate(start_a, start_b, end_b);
    auto new_start_a = (end_a - start_a) + it;
    std::rotate(it, new_start_a, end_b);
}


Solution Perturbacao1(const Data *data, const Solution *s);
Solution Perturbacao2(const Data *data, const Solution *s);

#endif
