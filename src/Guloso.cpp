#include "Guloso.h"

bool check(fineToInsert i, fineToInsert j) {
  return i.fine > j.fine;
}

Solution Guloso(const Data *data, double alpha) {
  Solution s;
  s.sequence = std::vector<int>(data->getQtOrders());
  std::vector<fineToInsert> CL;

  for(int i = 0; i < (int) data->getQtOrders(); i++) { 
    if(!data->fine(i)) {
      s.sequence[s.qtProductsWithFine] = i;
    } else {
      fineToInsert v;
      v.node = i;
      CL.push_back(v);
    }
  }

  s.qtProductsWithFine = CL.size();
  s.accumulatedTime = std::vector<int>(data->getQtOrders());
  s.accumulatedFine = std::vector<int>(data->getQtOrders());

  int idx = 0;
  int bestFine = 99999999;
  for(int i = 0; i < (int) CL.size(); i++) {
    CL[i].fine = std::max(0, 
      data->fine(CL[i].node) * 
      (
        data->initialTime(CL[i].node) +
        data->time(CL[i].node) - 
        data->deadline(CL[i].node)
      )
    );
    if(CL[i].fine < bestFine) {
      bestFine = CL[i].fine;
      idx = i;
    }
  } 

  int currentTime = data->initialTime(CL[idx].node) + data->time(CL[idx].node);
  s.sequence[0] = CL[idx].node;
  std::swap(CL[idx], CL[CL.size()-1]);
  CL.pop_back();


  int k = 0;
  while(!CL.empty()) {
    for(int i = 0; i < (int) CL.size(); i++) {
      CL[i].fine = std::max(0, 
        data->fine(CL[i].node) *
        (
          currentTime 
          + data->timeToExchange(s.sequence[k], CL[i].node) 
          + data->time(CL[i].node) 
          - data->deadline(CL[i].node)
        )
      );
    } 

    idx = rand() % ((int) ((CL.size() - 1) * alpha) + 1);
    currentTime += data->timeToExchange(s.sequence[k], CL[idx].node) + data->time(CL[idx].node);
    k++;
    s.sequence[k] = CL[idx].node;
    std::swap(CL[idx], CL[CL.size()-1]);
    CL.pop_back();
  }

  attSolution(data, &s, 0);

  return s;
}
