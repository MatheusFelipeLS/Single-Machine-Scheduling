#include <iostream>
#include <algorithm>    
#include <vector>
#include <limits>
#include <cmath>
#include <bits/stdc++.h>

#include "Data.h"

using namespace std;


int swapImprove = 0;
int rotateImprove = 0;
int Re1 = 0;
int Re2 = 0;
int Re3 = 0;

struct ProductionInfo {
  vector<int> sequence;
  vector<int> accumulatedTime;
  vector<int> accumulatedFine;
  int qtProductsWithFine;
};


struct timeToInsert {
  int node;
  int time;
};


struct Solution {
  vector<int> sequence;
  int fine;
};


void showProductionInfo(ProductionInfo *s);
void showResult(ProductionInfo *s);


void calculateTimes(const Data *data, ProductionInfo *s) {
  int currentTime = data->time(s->sequence[0]) + data->initialTime(s->sequence[0]);
  int i;

  for(i = 0; i < (int) data->getQtOrders()-1; i++) {
    s->accumulatedTime[i] = currentTime; 
    currentTime += data->time(s->sequence[i+1]) + data->switchTime(s->sequence[i], s->sequence[i+1]);
  }

  s->accumulatedTime[i] = currentTime; 
}


void calculatePenalties(const Data *data, ProductionInfo *s) {
  s->accumulatedFine[0] = max(0, data->penalty(s->sequence[0]) * (data->time(s->sequence[0]) - data->deadline(s->sequence[0])));

  for(int i = 1; i < (int) s->sequence.size(); i++) {
    s->accumulatedFine[i] = s->accumulatedFine[i-1] + max(0, data->penalty(s->sequence[i]) * (s->accumulatedTime[i] - data->deadline(s->sequence[i])));
  }
}


void attProductionInfo(const Data *data, ProductionInfo *s, int start) {
  int i;
  if(!start) {
    s->accumulatedTime[0] = data->time(s->sequence[0]) + data->initialTime(s->sequence[0]);
    s->accumulatedFine[0] = max(0, data->penalty(s->sequence[0]) * (s->accumulatedTime[0] - data->deadline(s->sequence[0])));
    start++;
  }

  for(i = start; i < (int) s->qtProductsWithFine; i++) {
    s->accumulatedTime[i] = s->accumulatedTime[i-1] + data->switchTime(s->sequence[i-1], s->sequence[i]) + data->time(s->sequence[i]);
    s->accumulatedFine[i] = s->accumulatedFine[i-1] + max(0, data->penalty(s->sequence[i]) * (s->accumulatedTime[i] - data->deadline(s->sequence[i])));
  }
}

//retorna true se a solução é melhor e false caso contrário
bool sequenceIsBetter(const Data *data, ProductionInfo *s, int start, int stop) {
  int currentTime;
  int currentPenalty;

  if(!start) {
    currentTime = data->initialTime(s->sequence[start]) + data->time(s->sequence[start]);
    currentPenalty = max(0, data->penalty(s->sequence[start]) * (currentTime - data->deadline(s->sequence[start])));
    s->accumulatedTime[0] = currentTime;
    s->accumulatedFine[0] = currentPenalty;
    start++;
  } else {
    currentTime = s->accumulatedTime[start-1];
    currentPenalty = s->accumulatedFine[start-1];
  }

  if(stop < s->qtProductsWithFine-1) stop++;

  int timeInStop = s->accumulatedTime[stop];
  int penaltyInStop = s->accumulatedFine[stop];
  
  for(int i = start; i <= stop; i++) {
    currentTime += data->switchTime(s->sequence[i-1], s->sequence[i]) + data->time(s->sequence[i]);
    currentPenalty += max(0, data->penalty(s->sequence[i]) * (currentTime - data->deadline(s->sequence[i])));
    s->accumulatedTime[i] = currentTime;
    s->accumulatedFine[i] = currentPenalty;
  }

  if(s->accumulatedTime[stop] > timeInStop && ( (s->accumulatedFine[stop] > penaltyInStop) || !s->accumulatedFine[stop]) ) {
    return false;
  }

  for(int i = stop+1; i < s->qtProductsWithFine; i++) {
    currentTime += data->switchTime(s->sequence[i-1], s->sequence[i]) + data->time(s->sequence[i]);
    currentPenalty += max(0, data->penalty(s->sequence[i]) * (currentTime - data->deadline(s->sequence[i])));

    if(currentPenalty > s->accumulatedFine[i]) {
      return false;
    }

    s->accumulatedTime[i] = currentTime;
    s->accumulatedFine[i] = currentPenalty;
  }


  return true;
}

bool check(timeToInsert i, timeToInsert j){
  return i.time < j.time;
}

ProductionInfo Guloso(const Data *data) {
  ProductionInfo s;
  s.qtProductsWithFine = data->getQtOrders();
  s.sequence = vector<int>(data->getQtOrders());
  s.accumulatedTime = vector<int>(data->getQtOrders());
  s.accumulatedFine = vector<int>(data->getQtOrders());
  vector<timeToInsert> CL;
  vector<int> switchTime(data->getQtOrders());

  for(int i = 0; i < (int) data->getQtOrders(); i++) { 
    if(!data->penalty(i)) {
      s.qtProductsWithFine--;
      s.sequence[s.qtProductsWithFine] = i;
    } else {
      timeToInsert v;
      v.node = i;
      CL.push_back(v);
    }
  }

  vector<int> times(CL.size());

  for(int i = 0; i < (int) CL.size(); i++) {
    CL[i].time = data->deadline(CL[i].node) - data->initialTime(CL[i].node) + data->time(CL[i].node);
  } 

  sort(CL.begin(), CL.end(), check);
  int idx = rand() % ((int) CL.size() / 10);
  s.sequence[0] = CL[idx].node;
  s.accumulatedTime[0] = CL[idx].time;
  swap(CL[idx], CL[CL.size()-1]);
  CL.erase(CL.begin()+CL.size()-1);
 

  int k = 1;
  while(!CL.empty()) {
    for(int i = 0; i < (int) CL.size(); i++) {
      CL[i].time = data->deadline(CL[i].node) - data->switchTime(s.sequence[s.sequence.size()-1], CL[i].node) + data->time(CL[i].node);
    } 

    sort(CL.begin(), CL.end(), check);
    int idx = rand() % (((int) (CL.size() - 1) / 4) + 1);
    s.sequence[k] = CL[idx].node;
    k++;
    swap(CL[idx], CL[CL.size()-1]);
    CL.erase(CL.begin()+CL.size()-1);
  }

  attProductionInfo(data, &s, 0);

  return s;
}


bool Swap(const Data*data, ProductionInfo *s) {
  int best_i = 0;
  int best_j = 0;
  int bestDelta = 1;
  vector<int> bestTimes, bestPenalties;
  int initialValue = s->accumulatedFine[s->qtProductsWithFine-1];

  for(int i = 0; i < s->qtProductsWithFine-1; i++) {
    for(int j = i+1; j < s->qtProductsWithFine; j++) {
      ProductionInfo aux = *s;

      swap(aux.sequence[i], aux.sequence[j]);

      bool improved = sequenceIsBetter(data, &aux, i, j);
      if(improved) {
        int delta = aux.accumulatedFine[aux.qtProductsWithFine-1] - initialValue;
        if(delta < bestDelta) {
          best_i = i;
          best_j = j;
          bestDelta = delta;
        }
      }
    }
  }

  if(bestDelta < 0) {
    swap(s->sequence[best_i], s->sequence[best_j]);
    attProductionInfo(data, s, best_i);
    return true;
  }

  return false;
}


bool Rotate(const Data*data, ProductionInfo *s) {
  int best_i = 0;
  int best_j = 0;
  int bestDelta = 1;
  vector<int> bestTimes, bestPenalties;
  int initialValue = s->accumulatedFine[s->qtProductsWithFine-1];

  for(int i = 0; i < (int) s->qtProductsWithFine-1; i++) {
    for(int j = i+1; j < (int) s->qtProductsWithFine; j++) {
      ProductionInfo aux = *s;

      reverse(aux.sequence.begin()+i, aux.sequence.begin()+j);

      bool improved = sequenceIsBetter(data, &aux, i, j);
      if(improved) {
        int delta = aux.accumulatedFine[aux.qtProductsWithFine-1] - initialValue;
        if(delta < bestDelta) {
          best_i = i;
          best_j = j;
          bestDelta = delta;
        }
      }
    }
  }

  if(bestDelta < 0) {
    reverse(s->sequence.begin()+best_i, s->sequence.begin()+best_j);
    attProductionInfo(data, s, best_i);
    return true;
  }

  return false;
}


bool Reinsertion(const Data*data, ProductionInfo *s, int range) {
  int best_i = 0;
  int best_j = 0;
  int bestDelta = 1;
  vector<int> bestTimes, bestPenalties;
  int initialValue = s->accumulatedFine[s->qtProductsWithFine-1];

  for(int i = 0; i < s->qtProductsWithFine-range+1; i++) {
    for(int j = i+range; j < s->qtProductsWithFine; j++) {
      ProductionInfo aux = *s;
      rotate(aux.sequence.begin() + i, aux.sequence.begin() + i + range, aux.sequence.begin() + j);

      bool improved = sequenceIsBetter(data, &aux, i, j);
      if(improved) {
        int delta = aux.accumulatedFine[aux.qtProductsWithFine-1] - initialValue;
        if(delta < bestDelta) {
          best_i = i;
          best_j = j;
          bestDelta = delta;
        }
      }
    }

    for(int j = i-1; j >= 0; j--) {
      ProductionInfo aux = *s;
      rotate(aux.sequence.begin()+j, aux.sequence.begin()+i, aux.sequence.begin()+i+range);

      bool improved = sequenceIsBetter(data, &aux, j, i+range);
      if(improved) {
        int delta = aux.accumulatedFine[aux.qtProductsWithFine-1] - initialValue;
        if(delta < bestDelta) {
          best_i = i;
          best_j = j;
          bestDelta = delta;
        }
      }
    }
  }

  if(bestDelta < 0) {
    if(best_i > best_j) {
      rotate(s->sequence.begin()+best_j, s->sequence.begin()+best_i, s->sequence.begin()+best_i+range);
      best_i = best_j;
    } else {
      rotate(s->sequence.begin() + best_i, s->sequence.begin() + best_i + range, s->sequence.begin() + best_j);
    }

    attProductionInfo(data, s, best_i);

    return true;
  }

  return false;
}


void LocalSearch(const Data *data, ProductionInfo *s) {
  // vector<int> n = {1, 2, 3, 4, 5};
  vector<int> n = {1, 3, 4, 5};
  bool improved = false;
  while(!n.empty()) {
    int x = rand() % n.size();
    switch (n[x]) {
      case 1:
        improved = Swap(data, s);
        break; 
      case 2:
        improved = Rotate(data, s);
        break; 
      case 3:
        improved = Reinsertion(data, s, 1);
        break; 
      case 4:
        improved = Reinsertion(data, s, 2);
        break; 
      case 5:
        improved = Reinsertion(data, s, 3);
        break; 
    }
    if(improved) {
      if(n[x] == 1) {
        swapImprove++;
      }
      else if(n[x] == 2) {
        rotateImprove++;
      }
      else if(n[x] == 3) {
        Re1++;
      }
      else if(n[x] == 4) {
        Re2++;
      }
      else if(n[x] == 5) {
        Re3++;
      }
      if(!s->accumulatedFine[s->qtProductsWithFine-1]) return;
      // n = {1, 2, 3, 4, 5};
      n = {1, 3, 4, 5};
    } else {
      n.erase(n.begin()+x);
    }
  }
}


template<typename IT>
void swap_ranges(IT start_a, IT end_a, IT start_b, IT end_b) {
    auto it = std::rotate(start_a, start_b, end_b);
    auto new_start_a = (end_a - start_a) + it;
    std::rotate(it, new_start_a, end_b);
}


void Perturbacao(const Data *data, ProductionInfo *s) {
  int tamMax = (int) s->qtProductsWithFine / 10;

  int bloco1 = rand() % (tamMax -1) + 2;
  int bloco2 = rand() % (tamMax -1) + 2;

  int inicioDoBloco2 = (rand() % (s->qtProductsWithFine - bloco2 - bloco1-1)) + bloco1 +2;
  int inicioDoBloco1 = rand() % (inicioDoBloco2 - bloco1-1) +1;


  swap_ranges(
    s->sequence.begin() + inicioDoBloco1, 
    s->sequence.begin() + inicioDoBloco1 + bloco1, 
    s->sequence.begin() + inicioDoBloco2, 
    s->sequence.begin() + inicioDoBloco2 + bloco2 
  );

  attProductionInfo(data, s, inicioDoBloco1-1);
}

  

Solution ILS(const Data *data, int max_iter) {
  Solution bestSolution;
  bestSolution.fine = numeric_limits<int>::max();
  
  int maxIterIls = 50;

  for(int i = 0; i < max_iter; i++) {
    ProductionInfo s = Guloso(data);
    ProductionInfo best = s;
    // cout << "custo após guloso: " << best.accumulatedFine[best.qtProductsWithFine-1] << endl;
    int iterILS = 0;
    while(iterILS <= maxIterIls) {
      LocalSearch(data, &s);
      // cout << "custo após busca local: " << s.accumulatedFine[s.qtProductsWithFine-1] << endl;
      if(s.accumulatedFine[s.qtProductsWithFine-1] < best.accumulatedFine[best.qtProductsWithFine-1]) {
        best = s;
        if(!s.accumulatedFine[s.qtProductsWithFine-1]) break;
        iterILS = 0;
      }
      Perturbacao(data, &s);
      // cout << "custo após perturbação: " << s.accumulatedFine[s.qtProductsWithFine-1] << endl;
      iterILS++;
    }

    if(best.accumulatedFine[best.qtProductsWithFine-1] < bestSolution.fine) {
      bestSolution.sequence = best.sequence;
      bestSolution.fine = best.accumulatedFine[best.qtProductsWithFine-1];
      if(!bestSolution.fine) return bestSolution;
    }
  }
	return bestSolution;
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "Uso: " << argv[0] << "<caminho_do_arquivo>\n";
    return 1;
  }

  Data data;
  time_t start, end;
  double media = 0;
  int n = 1;
  start = clock();
  srand(time(NULL)+23879);
  ios_base::sync_with_stdio(false);

  ProductionInfo s;
  Solution r;
  for(int cont = 0; cont < n; cont++) {
    data.readFromFile(argv[1]);

    r = ILS(&data, 25);
    media += r.fine;
  }

  s.sequence = r.sequence;
  s.accumulatedFine = vector<int>(s.sequence.size());
  s.accumulatedTime = vector<int>(s.sequence.size());

  calculateTimes(&data, &s);
  calculatePenalties(&data, &s);

  cout << "improves:\nSwap: " << swapImprove << "\nRotate: " << rotateImprove << "\nre1: " << Re1 << "\nr2: " << Re2 << "\nr3: " << Re3 << endl;  
  cout << "cost médio: " << media/n;
  end = clock();
  double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
  cout << "; Tempo médio: " << fixed 
      << time_taken/n << setprecision(5);
  cout << " sec " << endl << endl;


  cout << "Confirmação do resultado\n";
  showResult(&s);

  return 0;
}


// AUXILIAR FUNCTIONS
void showSolution(ProductionInfo *s) {
  for(int i = 0; i < (int) s->sequence.size(); i++) cout << s->sequence[i] << " ";
  cout << endl << endl;

  for(int i = 0; i < (int) s->accumulatedTime.size(); i++) cout << s->accumulatedTime[i] << " ";
  cout << endl << endl;

  for(int i = 0; i < (int) s->accumulatedFine.size(); i++) cout << s->accumulatedFine[i] << " ";
  cout << endl << endl;
}


void showResult(ProductionInfo *s) {
  for(int i = 0; i < (int) s->sequence.size(); i++) {
    cout << s->sequence[i] << " ";
  }

  cout << "\nCusto: " << s->accumulatedFine[s->accumulatedFine.size()-1] << "\n\n";
}