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
    fines.resize(qtOrders);
    initialPreparationTime.resize(qtOrders);
    timeToExchangeMatrix.resize(qtOrders, std::vector<int>(qtOrders));

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
        file >> fines[i];
    }

    for (int i = 0; i < qtOrders; i++) {
        file >> initialPreparationTime[i];
    }

    // Lendo a matriz de times de troca
    for (int i = 0; i < qtOrders; i++) {
        for (int j = 0; j < qtOrders; j++) {
            file >> timeToExchangeMatrix[i][j];
        }
    }

    file.close();
}

int Data::getQtOrders() const {
    return this->qtOrders;
}

const int Data::initialTime(int pos) const {
    return initialPreparationTime[pos];
}

const int Data::time(int pos) const {
    return this->times[pos];
}

const int Data::deadline(int pos) const {
    return this->deadlines[pos];
}

const int Data::fine(int pos) const {
    return this->fines[pos];
}

const int Data::timeToExchange(int x, int y) const {
    return this->timeToExchangeMatrix[x][y];
}

