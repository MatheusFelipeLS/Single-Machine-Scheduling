#include <iostream>
#include <algorithm>    
#include <vector>
#include <limits>
#include <cmath>
#include <bits/stdc++.h>

#include "Data.h"
#include "ILS.h"

using namespace std;

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "Uso: " << argv[0] << "<caminho_do_arquivo>\n";
    return 1;
  }

  Data data;
  time_t start, end;
  double media = 0;
  int n = 1;
  srand(time(NULL)+23879);
  ios_base::sync_with_stdio(false);
  data.readFromFile(argv[1]);

  ProductionInfo s;
  Solution r;

  start = clock();
  for(int cont = 0; cont < n; cont++) {
    r = ILS(&data, 10);
    media += r.fine;
  }
  end = clock();

  s.sequence = r.sequence;
  s.accumulatedFine = vector<int>(s.sequence.size());
  s.accumulatedTime = vector<int>(s.sequence.size());

  calculateTimes(&data, &s);
  calculatePenalties(&data, &s);


  cout << "Número de iterações: " << n << "\nMulta médio: " << media/n;
  double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
  cout << "; Tempo médio: " << fixed 
      << time_taken/n << setprecision(5);
  cout << " sec " << endl << endl;


  cout << "Confirmação do resultado\n";
  showResult(&s);

  return 0;
}
