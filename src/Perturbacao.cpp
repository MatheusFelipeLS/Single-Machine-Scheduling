#include "Perturbacao.h"

Solution Perturbacao1(const Data *data, const Solution *s) {
  Solution PI = *s;

  int tamMax = (int) data->getQtOrders() / 12;

  int bloco1 = (rand() % (tamMax - 1)) + 2;
  int bloco2 = (rand() % (tamMax - 1)) + 2;

  int inicioDoBloco2 = (rand() % (PI.qtProductsWithFine - bloco2 - bloco1 + 1)) + bloco1;
  int inicioDoBloco1 = (rand() % (inicioDoBloco2 - bloco1 + 1));

  swap_ranges(
    PI.sequence.begin() + inicioDoBloco1,   
    PI.sequence.begin() + inicioDoBloco1 + bloco1, 
    PI.sequence.begin() + inicioDoBloco2, 
    PI.sequence.begin() + inicioDoBloco2 + bloco2 
  );

  attSolution(data, &PI, inicioDoBloco1);

  return PI;
}


Solution Perturbacao2(const Data *data, const Solution *s) {
  Solution PI = *s;

  int tamMax = data->getQtOrders() / 6;
  int bloco1 = (rand() % (tamMax - 1)) + 4;
  int bloco2 = (rand() % (tamMax - 1)) + 4;

  int inicioDoBloco2 = (rand() % (PI.qtProductsWithFine - bloco2 - bloco1 + 1)) + bloco1;
  int inicioDoBloco1 = (rand() % (inicioDoBloco2 - bloco1 + 1));

  swap_ranges(
    PI.sequence.begin() + inicioDoBloco1,   
    PI.sequence.begin() + inicioDoBloco1 + bloco1, 
    PI.sequence.begin() + inicioDoBloco2, 
    PI.sequence.begin() + inicioDoBloco2 + bloco2 
  );

  attSolution(data, &PI, inicioDoBloco1);

  return PI;
}