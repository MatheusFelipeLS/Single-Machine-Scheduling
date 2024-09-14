#include <iostream>
#include <vector>

using namespace std;

struct Solution {
  int penalty;
  vector<int> sequence;
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

int calcularCustoTotal(vector<int> &s, vector<int> &productionTimes, vector<int> &deadlines, vector<int> &penalties, vector<vector<int>> &switchingCostMatrix) {
  int tempoTotal = productionTimes[s[0]];
  int multa = max(0, penalties[s[0]] * (productionTimes[s[0]] - deadlines[s[0]]));
  
  for(int i = 1; i < s.size(); i++) {
    multa += max(0, penalties[s[i]] * (productionTimes[s[i]] + tempoTotal + switchingCostMatrix[s[i-1]][s[i]] - deadlines[s[i]]));
    tempoTotal += productionTimes[s[i]] + switchingCostMatrix[s[i-1]][s[i]];
  }

  return multa;
}

Solution O_GANANCIOSO(int qtOrders, vector<int> &productionTimes, vector<int> &deadlines, vector<int> &penalties, vector<vector<int>> &switchingCostMatrix) {
  Solution s;
  vector<int> CL(qtOrders);

  for(int i = 0; i < CL.size(); i++) CL[i] = i;

  vector<int> exchangeInterval(qtOrders);
  int currentTime = 0;
  while(!CL.empty()) {
    vector<int> scores(CL.size());

    int i = 0;
    do {
      double score = penalties[CL[i]] * (productionTimes[CL[i]] + exchangeInterval[CL[i]] + currentTime - deadlines[CL[i]]);
      scores[i] = score;
      i++;
    } while(i < CL.size());

    int idxBest = 0;
    for(int j = 1; j < scores.size(); j++) {
      if(scores[j] > scores[idxBest]) idxBest = j;
    }

    s.sequence.push_back(CL[idxBest]);
    currentTime += exchangeInterval[idxBest] + productionTimes[idxBest];
    exchangeInterval = switchingCostMatrix[s.sequence[s.sequence.size()-1]];
    CL.erase(CL.begin() + idxBest);
  }

  s.penalty = calcularCustoTotal(s.sequence, productionTimes, deadlines, penalties, switchingCostMatrix);

  return s;
}

int main() {
  int qtOrders = 5;

  vector<int> productionTimes = {15, 25, 20, 30, 20};
  vector<int> deadlines = {25, 45, 75, 120, 135};
  vector<int> penalties = {10, 12, 30, 15, 10};

  vector<vector<int>> switchingCostMatrix = {
    {0, 10, 15, 8, 21},
    {10, 0, 10, 13, 9},
    {17, 9, 0, 10, 14},
    {11, 13, 12, 0, 10},
    {5, 10, 15, 20, 0}
  };

  Solution s;

  s = O_GANANCIOSO(qtOrders, productionTimes, deadlines, penalties, switchingCostMatrix);

  showSolution(s.sequence);
  cout << "Multa: " << s.penalty << endl; 

  return 0;
}
