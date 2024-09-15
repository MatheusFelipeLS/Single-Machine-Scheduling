#include <iostream>
#include <vector>

using namespace std;

struct Solution {
  int penalty;
  vector<int> sequence;
};

struct Data {
  int qtOrders;
  vector<int> productionTimes;
  vector<int> deadlines;
  vector<int> penalties;
  vector<vector<int>> switchingCostMatrix;
};


void showSolution(vector<int> &s) {
  cout << "Solução:\n";
  int i;
  if(s.size() == 0) return;
  for(i = 0; i < s.size()-1; i++) {
    cout << s[i] << " - ";
  }
  cout << s[i] << endl;
}


vector<int> calculateTimes(Data *data, vector<int> &s, int start, vector<int> &times) {
  int currentTime;
  if(start == 0) {
    currentTime = data->productionTimes[s[0]];
    times[0] = currentTime;
    start += 1;
  } else {
    currentTime = times[start-1];
  }

  for(int i = start; i < data->qtOrders; i++) {
    times[i] = currentTime; 
    currentTime += data->productionTimes[s[i]] + data->switchingCostMatrix[s[i-1]][s[i]];
  }

  return times;
}


int calculatePenalties(vector<int> &s, Data *data) {
  vector<int> times(data->qtOrders);
  times = calculateTimes(data, s, 0, times);
  int penalty = max(0, data->penalties[s[0]] * (data->productionTimes[s[0]] - data->deadlines[s[0]]));
  
  for(int i = 1; i < s.size(); i++) {
    penalty += max(0, data->penalties[s[i]] * (data->productionTimes[s[i]] + times[i] + data->switchingCostMatrix[s[i-1]][s[i]] - data->deadlines[s[i]]));
  }

  return penalty;
}


Solution O_GANANCIOSO(Data *data) {
  Solution s;
  vector<int> CL(data->qtOrders);

  for(int i = 0; i < CL.size(); i++) CL[i] = i;

  vector<int> exchangeInterval(data->qtOrders);
  int currentTime = 0;
  while(!CL.empty()) {
    vector<int> scores(CL.size());

    int i = 0;
    do {
      double score = data->penalties[CL[i]] * (data->productionTimes[CL[i]] + exchangeInterval[CL[i]] + currentTime - data->deadlines[CL[i]]);
      scores[i] = score;
      i++;
    } while(i < CL.size());

    int idxBest = 0;
    for(int j = 1; j < scores.size(); j++) {
      if(scores[j] > scores[idxBest]) idxBest = j;
    }

    s.sequence.push_back(CL[idxBest]);
    currentTime += exchangeInterval[idxBest] + data->productionTimes[idxBest];
    exchangeInterval = data->switchingCostMatrix[s.sequence[s.sequence.size()-1]];
    CL.erase(CL.begin() + idxBest);
  }

  return s;
}


bool Swap(Data*data, Solution *s) {
  int best_i = 0;
  int best_j = 0;
  int bestValue = 0;
  vector<int> times(data->qtOrders);

  for(int i = 0; i < data->qtOrders-1; i++) {
    for(int j = i+1; j < data->qtOrders; j++) {
      vector<int> aux = s->sequence;

      swap(aux[i], aux[j]);

      int value = calculatePenalties(aux, data) - s->penalty;
      if(value < bestValue) {
        best_i = i;
        best_j = j;
        bestValue = value;
      }
    }
  }

  if(bestValue < 0) {
    swap(s->sequence[best_i], s->sequence[best_j]);
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


int main() {
  Data data;
  srand(time(NULL));
  data.qtOrders = 5;

  data.productionTimes = {15, 25, 20, 30, 20};
  data.deadlines = {25, 45, 75, 120, 135};
  data.penalties = {10, 12, 30, 15, 10};

  data.switchingCostMatrix = {
    {0, 10, 15, 8, 21},
    {10, 0, 10, 13, 9},
    {17, 9, 0, 10, 14},
    {11, 13, 12, 0, 10},
    {5, 10, 15, 20, 0}
  };

  Solution s;
  s = O_GANANCIOSO(&data);
  s.penalty = calculatePenalties(s.sequence, &data);
  
  BuscaLocal(&data, &s);

  showSolution(s.sequence);
  cout << "Multa: " << s.penalty << endl; 

  return 0;
}
