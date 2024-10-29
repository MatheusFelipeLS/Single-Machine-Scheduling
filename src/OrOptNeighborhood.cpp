#include "OrOptNeighborhood.h"

bool FrontalOrOptWasGood(const Data *data, Solution *s, int range, int i, int j, int *auxTime, int *auxFine) {
  int time, fine, a;

  if(j - range == 0) {
    time = data->initialTime(s->sequence[j]) + data->time(s->sequence[j]);
    fine = 0;
  } else if(j - range == i) {
    time = s->accumulatedTime[i-1];
    fine = s->accumulatedFine[i-1];

    time += data->timeToExchange(s->sequence[i-1], s->sequence[j]) + data->time(s->sequence[j]); 
  }else {
    time = *auxTime;
    fine = *auxFine;
    time += data->timeToExchange(s->sequence[j-1], s->sequence[j]) + data->time(s->sequence[j]);
  }
  
  fine += std::max(0, data->fine(s->sequence[j]) * (time - data->deadline(s->sequence[j]))); 

  *auxTime = time;
  *auxFine = fine;

  time += data->timeToExchange(s->sequence[j], s->sequence[i]) + data->time(s->sequence[i]);
  fine += std::max(0, data->fine(s->sequence[i]) * (time - data->deadline(s->sequence[i]))); 

  for(a = i+1; a < i+range; a++) {
    time += data->timeToExchange(s->sequence[a-1], s->sequence[a]) + data->time(s->sequence[a]);
    fine += std::max(0, data->fine(s->sequence[a]) * (time - data->deadline(s->sequence[a]))); 
  }

  if(j + 1 < s->qtProductsWithFine) {
    time += data->timeToExchange(s->sequence[i+range-1], s->sequence[j+1]) + data->time(s->sequence[j+1]);
    fine += std::max(0, data->fine(s->sequence[j+1]) * (time - data->deadline(s->sequence[j+1]))); 

    for(a = j+2; a < s->qtProductsWithFine; a++) {
      time += data->timeToExchange(s->sequence[a-1], s->sequence[a]) + data->time(s->sequence[a]);
      fine += std::max(0, data->fine(s->sequence[a]) * (time - data->deadline(s->sequence[a]))); 

      if(s->accumulatedFine[a] <= fine && s->accumulatedTime[a] < time) return false;
    }
  }

  s->fine = fine;

  return true;
}


bool BackOrOptWasGood(const Data *data, Solution *s, int range, int i, int j) {
  int time, fine, a;

  if(j == 0) {
    time = data->initialTime(s->sequence[i]) + data->time(s->sequence[i]);
    fine = 0;
  } else {
    time = s->accumulatedTime[j-1];
    fine = s->accumulatedFine[j-1];

    time += data->timeToExchange(s->sequence[j-1], s->sequence[i]) + data->time(s->sequence[i]);
  }

  fine += std::max(0, data->fine(s->sequence[i]) * (time - data->deadline(s->sequence[i]))); 

  for(a = i+1; a < i+range; a++) {
    time += data->timeToExchange(s->sequence[a-1], s->sequence[a]) + data->time(s->sequence[a]);
    fine += std::max(0, data->fine(s->sequence[a]) * (time - data->deadline(s->sequence[a]))); 
  }
  
  time += data->timeToExchange(s->sequence[i+range-1], s->sequence[j]) + data->time(s->sequence[j]);
  fine += std::max(0, data->fine(s->sequence[j]) * (time - data->deadline(s->sequence[j]))); 

  for(a = j+1; a < i; a++) {
    time += data->timeToExchange(s->sequence[a-1], s->sequence[a]) + data->time(s->sequence[a]);
    fine += std::max(0, data->fine(s->sequence[a]) * (time - data->deadline(s->sequence[a]))); 
  }

  if(j+range+1 < s->qtProductsWithFine) {
    time += data->timeToExchange(s->sequence[i-1], s->sequence[i+range]) + data->time(s->sequence[i+range]);
    fine += std::max(0, data->fine(s->sequence[i+range]) * (time - data->deadline(s->sequence[i+range]))); 

    if(s->accumulatedFine[i+range] <= fine && s->accumulatedTime[i+range] <= time) return false;

    for(a = i+range+1; a < s->qtProductsWithFine; a++) {
      time += data->timeToExchange(s->sequence[a-1], s->sequence[a]) + data->time(s->sequence[a]);
      fine += std::max(0, data->fine(s->sequence[a]) * (time - data->deadline(s->sequence[a]))); 

      if(s->accumulatedFine[a] <= fine && s->accumulatedTime[a] <= time) return false;
    }
  }

  s->fine = fine;

  return true;
}


void performOrOpt(const Data* data, Solution *s, int range, int best_i, int best_j) {
  if(best_i > best_j) {
    std::rotate(s->sequence.begin()+best_j, s->sequence.begin()+best_i, s->sequence.begin()+best_i+range);
    best_i = best_j;
  } else {
    std::rotate(s->sequence.begin() + best_i, s->sequence.begin() + best_i + range, s->sequence.begin() + best_j+1);
  }

  attSolution(data, s, best_i);
}


bool OrOpt(const Data* data, Solution *s, int range) {
  int best_i = 0;
  int best_j = 0;
  int delta;
  int bestDelta = 0;
  int initialFine = s->fine;
  int *auxTime, auxT;
  int *auxFine, auxF;
  bool improved;

  auxFine = &auxF;
  auxTime = &auxT;

  for(int i = 0; i < s->qtProductsWithFine-range; i++) {
    for(int j = i+range; j < s->qtProductsWithFine; j++) {
      improved = FrontalOrOptWasGood(data, s, range, i, j, auxTime, auxFine);

      if(improved) {
        delta = s->fine - initialFine;
        if(delta < bestDelta) {
          if(!s->fine) {
            performOrOpt(data, s, range, i, j);    
            return true;
          }
          best_i = i;
          best_j = j;
          bestDelta = delta;
        }
      }
    }
  }

  for(int i = 1; i <= s->qtProductsWithFine - range; i++) {
    for(int j = 0; j < i; j++) {
      improved = BackOrOptWasGood(data, s, range, i, j);

      if(improved) {
        int delta = s->fine - initialFine;
        if(delta < bestDelta) {
          if(!s->fine) {
            performOrOpt(data, s, range, i, j);    
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
    performOrOpt(data, s, range, best_i, best_j);    

    return true;
  }

  s->fine = initialFine;

  return false;
}
