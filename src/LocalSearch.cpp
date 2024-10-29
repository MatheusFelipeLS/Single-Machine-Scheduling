#include "LocalSearch.h"

void LocalSearch(const Data *data, Solution *s) {
  std::vector<int> n = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
  bool improved = false;
  while(!n.empty()) {
    int x = rand() % n.size();
    switch (n[x]) {
      case 1:  
        improved = OrOpt(data, s, 1);
        break;
      case 2:
        improved = OrOpt(data, s, 2);
        break; 
      case 3:
        improved = OrOpt(data, s, 3);
        break; 
      case 4:
        improved = OrOpt(data, s, 4);
        break; 
      case 5:
        improved = OrOpt(data, s, 5);
        break; 
      case 6:
        improved = OrOpt(data, s, 6);
        break; 
      case 7:
        improved = OrOpt(data, s, 7);
        break;
      case 8:
        improved = OrOpt(data, s, 8);
        break;  
      case 9:
        improved = OrOpt(data, s, 9);
        break;
      case 10:
        improved = OrOpt(data, s, 10);
        break;  
      case 11:
        improved = OrOpt(data, s, 11);
        break;  
      case 12:
        improved = OrOpt(data, s, 12);
        break;
      case 13:
        improved = OrOpt(data, s, 13);
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
      if(!s->fine) return;
      n = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
    } else {
      std::swap(n[x], n[n.size()-1]);
      n.pop_back();
    }
  }
}