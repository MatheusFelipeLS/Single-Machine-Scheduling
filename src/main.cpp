#include <iostream>
#include <vector>

#include "Data.h"

using namespace std;

struct Solution {
  vector<int> sequence;
  vector<int> times;
  vector<int> penalty;
};

void showSolution(Solution *s) {
  for(int i = 0; i < (int) s->sequence.size(); i++) cout << s->sequence[i] << " ";
  cout << endl;

  for(int i = 0; i < (int) s->times.size(); i++) cout << s->times[i] << " ";
  cout << endl;

  for(int i = 0; i < (int) s->penalty.size(); i++) cout << s->penalty[i] << " ";
  cout << endl;
}

void calculateTimes(Data *data, Solution *s) {
  int currentTime = data->time(s->sequence[0]);
  int i;

  for(i = 0; i < (int) data->getQtOrders()-1; i++) {
    s->times[i] = currentTime; 
    currentTime += data->time(s->sequence[i+1]) + data->switchTime(s->sequence[i], s->sequence[i+1]);
  }

  s->times[i] = currentTime; 
}

void calculatePenalties(Data *data, Solution *s) {
  s->penalty[0] = max(0, data->penalty(s->sequence[0]) * (data->time(s->sequence[0]) - data->deadline(s->sequence[0])));

  for(int i = 1; i < (int) s->sequence.size(); i++) {
    s->penalty[i] = s->penalty[i-1] + max(0, data->penalty(s->sequence[i]) * (s->times[i] - data->deadline(s->sequence[i])));
  }
}


Solution Guloso(Data *data) {
  Solution s;
  s.times = vector<int>(data->getQtOrders());
  s.penalty = vector<int>(data->getQtOrders());
  vector<int> CL(data->getQtOrders());
  vector<int> switchTime(data->getQtOrders());
  int currentTime = 0;

  for(int i = 0; i < (int) CL.size(); i++) CL[i] = i;

  while(!CL.empty()) {
    vector<int> penalties(CL.size());

    int i = 0;
    do {
      double penalty = data->penalty(CL[i]) * (data->time(CL[i]) + switchTime[CL[i]] + currentTime - data->deadline(CL[i]));
      penalties[i] = penalty;
      i++;
    } while(i < (int) CL.size());

    int idxBest = 0;
    for(int j = 1; j < (int) penalties.size(); j++) {
      if(penalties[j] > penalties[idxBest]) idxBest = j;
    }

    s.sequence.push_back(CL[idxBest]);
    currentTime += switchTime[idxBest] + data->time(idxBest);
    switchTime = data->getSwitchingTimeVector(s.sequence[s.sequence.size()-1]);
    CL.erase(CL.begin() + idxBest);
  }

  calculateTimes(data, &s);
  calculatePenalties(data, &s);

  return s;
}

void recalculateTimes(Data *data, Solution *s, int start) {
  if(!start) {
    calculateTimes(data, s);
    return;
  }

  int currentTime = data->time(s->sequence[start-1]);
  int i;

  for(i = start; i < (int) data->getQtOrders()-1; i++) {
    s->times[i] = currentTime; 
    currentTime += data->time(s->sequence[i+1]) + data->switchTime(s->sequence[i], s->sequence[i+1]);
  }

  s->times[i] = currentTime; 
}

void recalculatePenalties(Data *data, Solution *s, int start) {
  s->penalty[start] = max(0, data->penalty(s->sequence[start]) * (data->time(s->sequence[start]) - data->deadline(s->sequence[start])));

  for(int i = start+1; i < (int) s->sequence.size(); i++) {
    s->penalty[i] = s->penalty[i-1] + max(0, data->penalty(s->sequence[i]) * (s->times[i] - data->deadline(s->sequence[i])));
  }
}

bool Swap(Data*data, Solution *s) {
  int best_i = 0;
  int best_j = 0;
  int bestDelta = 0;
  vector<int> bestTimes, bestPenalties;
  int initialValue = s->penalty[s->penalty.size()-1];

  for(int i = 0; i < data->getQtOrders()-2; i++) {
    for(int j = i+2; j < data->getQtOrders(); j++) {
      Solution aux = *s;

      swap(aux.sequence[i], aux.sequence[j]);

      recalculateTimes(data, &aux, i);
      recalculatePenalties(data, &aux, i);
      int delta = aux.penalty[aux.penalty.size()-1] - initialValue;
      if(delta < bestDelta) {
        best_i = i;
        best_j = j;
        bestDelta = delta;
        bestPenalties = aux.penalty;
        bestTimes = aux.times;
      }
    }
  }

  if(bestDelta < 0) {
    swap(s->sequence[best_i], s->sequence[best_j]);
    s->times = bestTimes;
    s->penalty = bestPenalties;
    return true;
  }


  return false;
}


void BuscaLocal(Data *data, Solution *s) {
  vector<int> n = {0};

  while(!n.empty()) {
    int x = rand() % n.size();
    bool improvement;
    switch (x) {
      case 0:
        improvement = Swap(data, s);
        break; 
    }
    if(improvement) {
      n = {0};
    } else {
      n.erase(n.begin()+x);
    }
  }
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "Uso: " << argv[0] << " <nome_do_arquivo>\n";
    return 1;
  }

  srand(time(NULL));
  Data data;

  data.readFromFile(argv[1]);

  Solution s;

  s = Guloso(&data);

  BuscaLocal(&data, &s);

  showSolution(&s);

  return 0;
}

