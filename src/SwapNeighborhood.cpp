#include "SwapNeighborhood.h"

bool SwapWasGood(const Data *data, Solution *s, int range, int i, int j) {
  int time, fine, a;

  if(!i) {
    time = data->initialTime(s->sequence[j]) + data->time(s->sequence[j]);
    fine = 0;
  } else {
    time = s->accumulatedTime[i-1]; 
    time += data->timeToExchange(s->sequence[i-1], s->sequence[j]) + data->time(s->sequence[j]);

    fine = s->accumulatedFine[i-1]; 
  }

  fine += std::max(0, data->fine(s->sequence[j]) * (time - data->deadline(s->sequence[j])));
  
  for(a = j+1; a < j+range; a++) {
    time += data->timeToExchange(s->sequence[a-1], s->sequence[a]) + data->time(s->sequence[a]);
    fine += std::max(0, data->fine(s->sequence[a]) * (time - data->deadline(s->sequence[a]))); 
  }

  if(i+range == j) {
    time += data->timeToExchange(s->sequence[j+range-1], s->sequence[i]) + data->time(s->sequence[i]);
  } else {
    time += data->timeToExchange(s->sequence[j+range-1], s->sequence[i+range]) + data->time(s->sequence[i+range]);
    fine += std::max(0, data->fine(s->sequence[i+range]) * (time - data->deadline(s->sequence[i+range])));


    for(a = i+range+1; a < j; a++) {
      time += data->timeToExchange(s->sequence[a-1], s->sequence[a]) + data->time(s->sequence[a]);
      fine += std::max(0, data->fine(s->sequence[a]) * (time - data->deadline(s->sequence[a])));
    }

    time += data->timeToExchange(s->sequence[j-1], s->sequence[i]) + data->time(s->sequence[i]);     
  }

  fine += std::max(0, data->fine(s->sequence[i]) * (time - data->deadline(s->sequence[i])));

  for(a = i+1; a < i+range; a++) {
    time += data->timeToExchange(s->sequence[a-1], s->sequence[a]) + data->time(s->sequence[a]);
    fine += std::max(0, data->fine(s->sequence[a]) * (time - data->deadline(s->sequence[a])));
  } 

  if(j + range < s->qtProductsWithFine) {
    time += data->timeToExchange(s->sequence[i+range-1], s->sequence[j+range]) + data->time(s->sequence[j+range]);
    fine += std::max(0, data->fine(s->sequence[j+range]) * (time - data->deadline(s->sequence[j+range])));

    if(s->accumulatedFine[j+range] <= fine && s->accumulatedTime[j+range] <= time) return false;

    for(a = j+range+1; a < s->qtProductsWithFine; a++) {
      time += data->timeToExchange(s->sequence[a-1], s->sequence[a]) + data->time(s->sequence[a]);
      fine += std::max(0, data->fine(s->sequence[a]) * (time - data->deadline(s->sequence[a])));
      
      if(s->accumulatedFine[a] <= fine && s->accumulatedTime[a] <= time) return false;
    }
  }

  s->fine = fine;

  return true;
}

void performSwap(const Data *data, Solution *s, int range, int i, int j) {
  for(int k = 0; k < range; k++) {
    std::swap(s->sequence[i+k], s->sequence[j+k]);
  }

  attSolution(data, s, i);
}

bool Swap(const Data*data, Solution *s, int range) {
  int best_i = 0;
  int best_j = 0;
  int delta;
  int bestDelta = 0;
  int initialFine = s->fine;
  bool improved;

  for(int i = 0; i < s->qtProductsWithFine-(2*range)+1; i++) {
    for(int j = i+range; j < s->qtProductsWithFine-range+1; j++) {
      improved = SwapWasGood(data, s, range, i, j);

      if(improved) {
        delta = s->fine - initialFine;
        if(delta < bestDelta) {
          if(!s->fine) {
            performSwap(data, s, range, i, j);
            return true;
          }
          best_i = i;
          best_j = j;
          bestDelta = delta;
        }
      }
    }
  }

  if(bestDelta < 0) {
    performSwap(data, s, range, best_i, best_j);
    return true;
  }

  s->fine = initialFine;
  return false;
}
