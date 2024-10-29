#include "Solution.h"

void showSolutionData(Solution *s) {
  for(int i = 0; i < (int) s->sequence.size(); i++) std::cout << s->sequence[i] << " ";
  std::cout << std::endl << std::endl;

  for(int i = 0; i < (int) s->accumulatedTime.size(); i++) std::cout << s->accumulatedTime[i] << " ";
  std::cout << std::endl << std::endl;

  for(int i = 0; i < (int) s->accumulatedFine.size(); i++) std::cout << s->accumulatedFine[i] << " ";
  std::cout << std::endl << std::endl;
}

void showSolution(Solution *s) {
  for(int i = 0; i < (int) s->sequence.size(); i++) {
    std::cout << s->sequence[i] << " ";
  }

  std::cout << "\nCusto: " << s->fine << "\n\n";
}


void attSolution(const Data *data, Solution *s, int start) {
  int i;
  if(!start) {
    s->accumulatedTime[0] = data->time(s->sequence[0]) + data->initialTime(s->sequence[0]);
    s->accumulatedFine[0] = std::max(0, data->fine(s->sequence[0]) * (s->accumulatedTime[0] - data->deadline(s->sequence[0])));
    start++;
  }

  for(i = start; i < (int) s->qtProductsWithFine; i++) {
    s->accumulatedTime[i] = s->accumulatedTime[i-1] + data->timeToExchange(s->sequence[i-1], s->sequence[i]) + data->time(s->sequence[i]);
    s->accumulatedFine[i] = s->accumulatedFine[i-1] + std::max(0, data->fine(s->sequence[i]) * (s->accumulatedTime[i] - data->deadline(s->sequence[i])));
  }

  s->fine = s->accumulatedFine[s->qtProductsWithFine-1];
}


void calculateTimes(const Data *data, Solution *s) {
  int currentTime = data->time(s->sequence[0]) + data->initialTime(s->sequence[0]);
  int i;

  for(i = 0; i < (int) data->getQtOrders()-1; i++) {
    s->accumulatedTime[i] = currentTime; 
    currentTime += data->time(s->sequence[i+1]) + data->timeToExchange(s->sequence[i], s->sequence[i+1]);
  }

  s->accumulatedTime[i] = currentTime; 
}


void calculatePenalties(const Data *data, Solution *s) {
  s->accumulatedFine[0] = std::max(0, data->fine(s->sequence[0]) * (data->time(s->sequence[0]) - data->deadline(s->sequence[0])));

  for(int i = 1; i < (int) s->sequence.size(); i++) {
    s->accumulatedFine[i] = s->accumulatedFine[i-1] + std::max(0, data->fine(s->sequence[i]) * (s->accumulatedTime[i] - data->deadline(s->sequence[i])));
  }
}
