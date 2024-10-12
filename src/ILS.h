#include <iostream>
#include <algorithm>    
#include <vector>
#include <limits>
#include <cmath>
#include <bits/stdc++.h>
#include <time.h>


#include "Data.h"

using namespace std;


struct ProductionInfo {
  vector<int> sequence;
  vector<int> accumulatedTime;
  vector<int> accumulatedFine;
  int qtProductsWithFine;
  int lastProductWith0Fine;
};


struct fineToInsert {
  int node;
  int fine;
};


struct Solution {
  vector<int> sequence;
  int fine;
};


template<typename IT>
void swap_ranges(IT start_a, IT end_a, IT start_b, IT end_b) {
    auto it = std::rotate(start_a, start_b, end_b);
    auto new_start_a = (end_a - start_a) + it;
    std::rotate(it, new_start_a, end_b);
}

void showSolution(Solution *s);
void showProductionInfo(ProductionInfo *s);
void showResult(ProductionInfo *s);

void calculateTimes(const Data *data, ProductionInfo *s);
void calculatePenalties(const Data *data, ProductionInfo *s);
void attProductionInfo(const Data *data, ProductionInfo *s, int start);

ProductionInfo Guloso(const Data *data);

bool sequenceIsBetter(const Data *data, ProductionInfo *s, int start, int stop);
bool Swap(const Data*data, ProductionInfo *s, int range);
bool Rotate(const Data*data, ProductionInfo *s);
bool Reinsertion(const Data*data, ProductionInfo *s, int range);
void LocalSearch(const Data *data, ProductionInfo *s);

void Perturbacao1(const Data *data, ProductionInfo *s);
void Perturbacao2(const Data *data, ProductionInfo *s);

Solution ILS(const Data *data, int max_iter);
