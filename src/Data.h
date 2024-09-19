#ifndef DATA_H
#define DATA_H

#include <vector>
#include <string>

class Data {
private:
    int qtOrders;
    std::vector<int> times;
    std::vector<int> deadlines;
    std::vector<int> penalties;
    std::vector<std::vector<int>> switchingTimeMatrix;

public:
    // Função para ler os dados de um arquivo
    void readFromFile(const std::string& filename);

    // Funções para retornar os valores
    int getQtOrders() const;
    const std::vector<int>& getTimes() const;
    const std::vector<int>& getDeadlines() const;
    const std::vector<int>& getPenalties() const;
    const std::vector<std::vector<int>>& getSwitchingTimeMatrix() const;

    const std::vector<int>& getSwitchingTimeVector(int pos) const;
    const int time(int pos) const;
    const int deadline(int pos) const;
    const int penalty(int pos) const;
    const int switchTime(int x, int y) const;
};

#endif // DATA_H
