#include "ILS.h"

Solution ILS(const Data *data, int max_iter, int maxIterIls) {
  Solution bestOfAll;
  bestOfAll.fine = std::numeric_limits<int>::max();

  for(int i = 0; i < max_iter; i++) {
    Solution s = Guloso(data, 0.25);
    Solution best = s;
    LocalSearch(data, &s);
    int iterILS = 0;
    while(iterILS <= maxIterIls) {

      if(s.fine < best.fine) {
        best = s;
        iterILS = 0;
        if(!best.fine) return best;
      }
      
      if(iterILS % 2 == 0) {
        s = Perturbacao1(data, &best);
      } else {
        s = Perturbacao2(data, &best);
      } 

      LocalSearch(data, &s);
      
      iterILS++;
    }
    if(best.fine < bestOfAll.fine) {
      bestOfAll = best;
      if(!bestOfAll.fine) return bestOfAll;
    }
  }

	return bestOfAll;
}
