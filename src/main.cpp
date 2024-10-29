#include <iostream>
#include <cmath>
#include <time.h>

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
  int n = 10;
  srand(time(NULL)+23879);
  ios_base::sync_with_stdio(false);
  data.readFromFile(argv[1]);

  Solution r;

  start = clock();
  for(int cont = 0; cont < n; cont++) {
    r = ILS(&data, 20, 250);
    media += r.fine;
  }
  end = clock();

  cout << "Multa médio: " << media/n;
  double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
  cout << "; Tempo médio: " << fixed << time_taken/n << setprecision(5);
  cout << " sec " << endl << endl;

  return 0;
}
