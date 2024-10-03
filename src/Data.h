#ifndef DATA_H
#define DATA_H

#include <vector>
#include <string>

class Data {
private:
    int qtOrders;
    std::vector<int> times;
    std::vector<int> deadlines;
    std::vector<int> fines;
    std::vector<int> initialPreparationTime;
    std::vector<std::vector<int>> timeToExchangeMatrix;

public:
    // Função para ler os dados de um arquivo
    void readFromFile(const std::string& filename);

    // Funções para retornar os valores
    int getQtOrders() const;

    const int initialTime(int pos) const;
    const int time(int pos) const;
    const int deadline(int pos) const;
    const int fine(int pos) const;
    const int timeToExchange(int x, int y) const;
};

#endif // DATA_H
