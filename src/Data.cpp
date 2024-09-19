#include "Data.h"
#include <fstream>
#include <iostream>

void Data::readFromFile(const std::string& filename) {
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo!\n";
        return;
    }

    // Lendo o número de pedidos
    file >> qtOrders;

    // Redimensionando os vetores com base no número de pedidos
    times.resize(qtOrders);
    deadlines.resize(qtOrders);
    penalties.resize(qtOrders);
    switchingTimeMatrix.resize(qtOrders, std::vector<int>(qtOrders));

    // Lendo os times
    for (int i = 0; i < qtOrders; i++) {
        file >> times[i];
    }

    // Lendo os prazos
    for (int i = 0; i < qtOrders; i++) {
        file >> deadlines[i];
    }

    // Lendo os custos
    for (int i = 0; i < qtOrders; i++) {
        file >> penalties[i];
    }

    // Lendo a matriz de times de troca
    for (int i = 0; i < qtOrders; i++) {
        for (int j = 0; j < qtOrders; j++) {
            file >> switchingTimeMatrix[i][j];
        }
    }

    file.close();
}

int Data::getQtOrders() const {
    return this->qtOrders;
}

const std::vector<int>& Data::getTimes() const {
    return this->times;
}

const std::vector<int>& Data::getDeadlines() const {
    return this->deadlines;
}

const std::vector<int>& Data::getPenalties() const {
    return this->penalties;
}

const std::vector<std::vector<int>>& Data::getSwitchingTimeMatrix() const {
    return switchingTimeMatrix;
}

const std::vector<int>& Data::getSwitchingTimeVector(int pos) const {
    return this->switchingTimeMatrix[pos];
}

const int Data::time(int pos) const {
    return this->times[pos];
}

const int Data::deadline(int pos) const {
    return this->deadlines[pos];
}

const int Data::penalty(int pos) const {
    return this->penalties[pos];
}

const int Data::switchTime(int x, int y) const {
    return this->switchingTimeMatrix[x][y];
}

