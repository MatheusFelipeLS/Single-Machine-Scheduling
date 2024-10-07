#include "ILS.h"

void calculateTimes(const Data *data, ProductionInfo *s) {
  int currentTime = data->time(s->sequence[0]) + data->initialTime(s->sequence[0]);
  int i;

  for(i = 0; i < (int) data->getQtOrders()-1; i++) {
    s->accumulatedTime[i] = currentTime; 
    currentTime += data->time(s->sequence[i+1]) + data->timeToExchange(s->sequence[i], s->sequence[i+1]);
  }

  s->accumulatedTime[i] = currentTime; 
}



void calculatePenalties(const Data *data, ProductionInfo *s) {
  s->accumulatedFine[0] = max(0, data->fine(s->sequence[0]) * (data->time(s->sequence[0]) - data->deadline(s->sequence[0])));

  for(int i = 1; i < (int) s->sequence.size(); i++) {
    s->accumulatedFine[i] = s->accumulatedFine[i-1] + max(0, data->fine(s->sequence[i]) * (s->accumulatedTime[i] - data->deadline(s->sequence[i])));
  }
}


void attProductionInfo(const Data *data, ProductionInfo *s, int start) {
  int i;
  if(!start) {
    s->accumulatedTime[0] = data->time(s->sequence[0]) + data->initialTime(s->sequence[0]);
    s->accumulatedFine[0] = max(0, data->fine(s->sequence[0]) * (s->accumulatedTime[0] - data->deadline(s->sequence[0])));
    start++;
  }

  for(i = start; i < (int) s->qtProductsWithFine; i++) {
    s->accumulatedTime[i] = s->accumulatedTime[i-1] + data->timeToExchange(s->sequence[i-1], s->sequence[i]) + data->time(s->sequence[i]);
    s->accumulatedFine[i] = s->accumulatedFine[i-1] + max(0, data->fine(s->sequence[i]) * (s->accumulatedTime[i] - data->deadline(s->sequence[i])));
  }
}


bool check(timeToInsert i, timeToInsert j) {
  return i.fine > j.fine;
}


ProductionInfo Guloso(const Data *data) {
  ProductionInfo s;
  s.qtProductsWithFine = data->getQtOrders();
  s.sequence = vector<int>(data->getQtOrders());
  s.accumulatedTime = vector<int>(data->getQtOrders());
  s.accumulatedFine = vector<int>(data->getQtOrders());
  vector<timeToInsert> CL;



  for(int i = 0; i < (int) data->getQtOrders(); i++) { 
    if(!data->fine(i)) {
      s.qtProductsWithFine--;
      s.sequence[s.qtProductsWithFine] = i;
    } else {
      timeToInsert v;
      v.node = i;
      CL.push_back(v);
    }
  }

  int idx = 0;
  int bestFine = 99999999;
  for(int i = 0; i < (int) CL.size(); i++) {
    CL[i].fine = data->fine(CL[i].node) * (data->initialTime(CL[i].node) + data->time(CL[i].node) - data->deadline(CL[i].node));
    if(CL[i].fine < bestFine) {
      bestFine = CL[i].fine;
      idx = i;
    }
  } 

  int currentTime = data->initialTime(CL[idx].node) + data->time(CL[idx].node);
  s.sequence[0] = CL[idx].node;
  swap(CL[idx], CL[CL.size()-1]);
  CL.pop_back();


  int k = 0;
  while(!CL.empty()) {
    for(int i = 0; i < (int) CL.size(); i++) {
      CL[i].fine = max(0, 
        data->fine(CL[i].node) *
        (
          currentTime 
          + data->timeToExchange(s.sequence[k], CL[i].node) 
          + data->time(CL[i].node) 
          - data->deadline(CL[i].node)
        )
      );
    } 

    idx = rand() % (((int) (CL.size() - 1) / 4) + 1);
    currentTime += data->timeToExchange(s.sequence[k], CL[idx].node) + data->time(CL[idx].node);
    k++;
    s.sequence[k] = CL[idx].node;
    swap(CL[idx], CL[CL.size()-1]);
    CL.pop_back();
  }

  attProductionInfo(data, &s, 0);

  return s;
}


//compara ProductionInfo em um determinado trecho e retorna true se a segunda for melhor, e false caso contrário
bool sequenceIsBetter(const Data* data, const ProductionInfo *s1, ProductionInfo *s2, int start, int stop) {
  if(!start) {
    s2->accumulatedTime[0] = data->initialTime(s2->sequence[0]) + data->time(s2->sequence[0]);
    s2->accumulatedFine[0] = max(0, data->fine(s2->sequence[0]) * (s2->accumulatedTime[0] - data->deadline(s2->sequence[0])));
    start++;
  } 

  if(stop < s2->qtProductsWithFine-1) stop++;
  
  for(int i = start; i <= stop; i++) {
    s2->accumulatedTime[i] = s2->accumulatedTime[i-1] + data->timeToExchange(s2->sequence[i-1], s2->sequence[i]) + data->time(s2->sequence[i]);
    s2->accumulatedFine[i] = s2->accumulatedFine[i-1] + max(0, data->fine(s2->sequence[i]) * (s2->accumulatedTime[i] - data->deadline(s2->sequence[i])));
  }

  if((s2->accumulatedTime[stop] > s1->accumulatedTime[stop]) && (s2->accumulatedFine[stop] >= s1->accumulatedFine[stop])) {
    return false;
  } else if((s2->accumulatedTime[stop] <= s1->accumulatedTime[stop]) && (s2->accumulatedFine[stop] <= s1->accumulatedFine[stop])) {
    attProductionInfo(data, s2, stop+1);
    return true;
  }

  for(int i = stop; i < s2->qtProductsWithFine; i++) {
    s2->accumulatedTime[i] = s2->accumulatedTime[i-1] + data->timeToExchange(s2->sequence[i-1], s2->sequence[i]) + data->time(s2->sequence[i]);
    s2->accumulatedFine[i] = s2->accumulatedFine[i-1] + max(0, data->fine(s2->sequence[i]) * (s2->accumulatedTime[i] - data->deadline(s2->sequence[i])));
    
    if((s2->accumulatedFine[i] >= s1->accumulatedFine[i]) && (s2->accumulatedTime[i] > s1->accumulatedTime[i])) {
      return false;
    }
  }

  return true;
}


bool Swap(const Data*data, ProductionInfo *s, int range) {
  int best_i = 0;
  int best_j = 0;
  int bestDelta = 0;
  int initialValue = s->accumulatedFine[s->qtProductsWithFine-1];

  for(int i = 0; i < s->qtProductsWithFine-(2*range)+1; i++) {
    for(int j = i+range; j < s->qtProductsWithFine-range+1; j++) {
      ProductionInfo aux = *s;
      for(int k = 0; k < range; k++) {
        swap(aux.sequence[i+k], aux.sequence[j+k]);
      }

      bool improved = sequenceIsBetter(data, s, &aux, i, j+range-1);

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
  int bestDelta = 0;
  int initialValue = s->accumulatedFine[s->qtProductsWithFine-1];

  for(int i = 0; i < (int) s->qtProductsWithFine-1; i++) {
    for(int j = i+1; j < (int) s->qtProductsWithFine; j++) {
      ProductionInfo aux = *s;

      reverse(aux.sequence.begin()+i, aux.sequence.begin()+j);

      bool improved = sequenceIsBetter(data, s, &aux, i, j);
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


bool Reinsertion(const Data* data, ProductionInfo *s, int range) {
  int best_i = 0;
  int best_j = 0;
  int bestDelta = 0;
  int initialValue = s->accumulatedFine[s->qtProductsWithFine-1];

  for(int i = 0; i < s->qtProductsWithFine-range; i++) {
    ProductionInfo aux = *s;

    for(int j = i+range; j < s->qtProductsWithFine; j++) {
      rotate(aux.sequence.begin() + (j-range), aux.sequence.begin() + j, aux.sequence.begin() + j + 1);

      bool improved = sequenceIsBetter(data, s, &aux, j-range, j);

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

  for(int i = s->qtProductsWithFine-range; i > 0; i--) {
    ProductionInfo aux = *s;
    for(int j = i-1; j >= 0; j--) {
      rotate(aux.sequence.begin()+j, aux.sequence.begin()+j+1, aux.sequence.begin()+j+1+range);

      bool improved = sequenceIsBetter(data, s, &aux, j, i+range-1);

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
      rotate(s->sequence.begin() + best_i, s->sequence.begin() + best_i + range, s->sequence.begin() + best_j+1);
    }

    attProductionInfo(data, s, best_i);

    return true;
  }

  return false;
}

vector<int> qtImproves = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void LocalSearch(const Data *data, ProductionInfo *s) {
  // vector<int> n = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
  vector<int> n = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
  bool improved = false;
  // Rotate(data, s);
  while(!n.empty()) {
    int x = rand() % n.size();
    switch (n[x]) {
      case 0:
        improved = Rotate(data, s);
        break;
      case 1:  
        improved = Reinsertion(data, s, 1);
        break;
      case 2:
        improved = Reinsertion(data, s, 2);
        break; 
      case 3:
        improved = Reinsertion(data, s, 3);
        break; 
      case 4:
        improved = Reinsertion(data, s, 4);
        break; 
      case 5:
        improved = Reinsertion(data, s, 5);
        break; 
      case 6:
        improved = Reinsertion(data, s, 6);
        break; 
      case 7:
        improved = Reinsertion(data, s, 7);
        break;
      case 8:
        improved = Reinsertion(data, s, 8);
        break;  
      case 9:
        improved = Reinsertion(data, s, 9);
        break;
      case 10:
        improved = Reinsertion(data, s, 10);
        break;  
      case 11:
        improved = Reinsertion(data, s, 11);
        break;  
      case 12:
        improved = Reinsertion(data, s, 12);
        break;
      case 13:
        improved = Reinsertion(data, s, 13);
        break;  
      case 14:
        improved = Swap(data, s, 1);
        break;
      case 15:
        improved = Swap(data, s, 2);
        break;
      case 16:
        improved = Swap(data, s, 3);
        break;
      case 17:
        improved = Swap(data, s, 4);
        break;
      case 18:
        improved = Swap(data, s, 5);
        break;
      case 19:
        improved = Swap(data, s, 6);
        break;
    }
    if(improved) {
      if(!s->accumulatedFine[s->qtProductsWithFine-1]) return;
      qtImproves[n[x]]++;
      // n = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
      n = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
    } else {
      swap(n[x], n[n.size()-1]);
      n.pop_back();
    }
  }
}


void Perturbacao(const Data *data, ProductionInfo *s) {
  int tamMax = (int) data->getQtOrders() / 12;

  int bloco1 = (rand() % (tamMax - 1)) + 2;
  int bloco2 = (rand() % (tamMax - 1)) + 2;

  int inicioDoBloco2 = (rand() % (s->qtProductsWithFine - bloco2 - bloco1 + 1)) + bloco1;
  int inicioDoBloco1 = (rand() % (inicioDoBloco2 - bloco1 + 1));

  swap_ranges(
    s->sequence.begin() + inicioDoBloco1,   
    s->sequence.begin() + inicioDoBloco1 + bloco1, 
    s->sequence.begin() + inicioDoBloco2, 
    s->sequence.begin() + inicioDoBloco2 + bloco2 
  );

  attProductionInfo(data, s, inicioDoBloco1);
}


void totalDestruction(const Data *data, ProductionInfo *s) {
  // random_shuffle(s->sequence.begin(), s->sequence.begin()+s->qtProductsWithFine);

  // attProductionInfo(data, s, 0);
  int tamMax = (int) data->getQtOrders() / 6;

  int bloco1 = (rand() % tamMax) + 2;
  int bloco2 = (rand() % tamMax) + 2;

  int inicioDoBloco2 = (rand() % (s->qtProductsWithFine - bloco2 - bloco1 + 1)) + bloco1;
  int inicioDoBloco1 = (rand() % (inicioDoBloco2 - bloco1 + 1));

  swap_ranges(
    s->sequence.begin() + inicioDoBloco1,   
    s->sequence.begin() + inicioDoBloco1 + bloco1, 
    s->sequence.begin() + inicioDoBloco2, 
    s->sequence.begin() + inicioDoBloco2 + bloco2 
  );

  attProductionInfo(data, s, inicioDoBloco1);
}


Solution ILS(const Data *data, int max_iter) {
  Solution bestSolution;
  bestSolution.fine = numeric_limits<int>::max();
  int maxIterIls = 150;


  for(int i = 0; i < max_iter; i++) {
    ProductionInfo s = Guloso(data);
    ProductionInfo best = s;
    int iterILS = 0;
    while(iterILS <= maxIterIls) {
      LocalSearch(data, &s);
  
      if(s.accumulatedFine[s.qtProductsWithFine-1] < best.accumulatedFine[best.qtProductsWithFine-1]) {
        best = s;
        iterILS = 0;
        if(!s.accumulatedFine[s.qtProductsWithFine-1]) break;
      }
      
      if(iterILS <= max_iter * 8 / 10) Perturbacao(data, &s);
      else totalDestruction(data, &s);
      iterILS++;
    }
    if(best.accumulatedFine[best.qtProductsWithFine-1] < bestSolution.fine) {
      bestSolution.sequence = best.sequence;
      bestSolution.fine = best.accumulatedFine[best.qtProductsWithFine-1];
      if(!bestSolution.fine) return bestSolution;
    }
  }

  for(int i = 0; i < (int) qtImproves.size(); i++) {
    cout << i << ": " << qtImproves[i] << endl;
  }

  showSolution(&bestSolution);
	return bestSolution;
}

void showProductionInfo(ProductionInfo *s) {
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

void showSolution(Solution *s) {
  for(int i = 0; i < (int) s->sequence.size(); i++) {
    cout << s->sequence[i] << " ";
  }
  cout << "\nCusto: " << s->fine << "\n\n";
}