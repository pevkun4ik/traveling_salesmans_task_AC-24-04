#include <iostream>
#include <iomanip>
#include <algorithm>

#include "alg_api.h"

// сюда просто добавляются include'ы участников
#include "Leonidova_ant.h"
#include "Slueva_opt2.h"
#include "Volkov_bb.h"

int main() {

    std::vector<std::vector<double>> dist = {
        { 0, 12,  4,  9,  7, 15,  3, 10,  6, 14,  5, 11,  8, 13,  2},
        { 8,  0, 14, 10,  2,  5, 11,  6,  9,  4, 15,  7, 12,  3, 13},
        {14, 15,  0,  2,  9, 10,  6,  4, 12,  8,  3, 13,  5, 11,  7},
        { 4,  3, 11,  0, 13,  8,  2, 14, 10,  6,  9,  5, 15,  7, 12},
        {10,  6,  9, 11,  0,  1, 14,  7,  3, 12,  8,  4, 13, 15,  5},
        { 3, 13,  3, 14,  9,  0, 10,  6, 15,  2, 11,  8,  7, 12,  4},
        {11,  4,  6,  9, 15, 10,  0, 13,  2,  7, 14,  3, 12,  5,  8},
        { 7, 10, 12,  3,  6, 14,  9,  0,  5, 11,  2, 15,  4,  8, 13},
        { 6,  9,  5, 10,  3, 15,  2,  8,  0, 14,  7, 12, 11,  4, 13},
        {12,  5,  8,  6, 14,  2,  7, 11,  9,  0,  4, 15, 10, 13,  3},
        { 9, 14,  2,  7, 11,  6, 15,  4, 13,  5,  0, 10,  3, 12,  8},
        { 5,  8, 13,  4, 10,  9,  3, 15,  6, 12,  2,  0, 14,  7, 11},
        {15,  7,  4, 12,  8,  5, 11,  3, 10,  9,  6, 14,  0,  2, 13},
        { 2, 11, 10,  5, 15, 12,  8,  4,  7, 13,  9,  6,  3,  0, 14},
        {13,  2,  7, 15,  4, 11,  5, 12, 14,  3,  8, 10,  9,  6,  0}
    };

    // вывод матрицы
    std::cout << "Input distance matrix:\n";
    for (auto& r : dist) {
        for (double d : r) std::cout << std::setw(4) << d << " ";
        std::cout << "\n";
    }
    std::cout << "\n";

    // -------- ЗАПУСК ВСЕХ ПОДКЛЮЧЕННЫХ АЛГОРИТМОВ --------
    auto& algos = getAlgorithms();
    std::vector<std::pair<std::string, Result>> results;

    for (auto& a : algos) {
        Result r = a.func(dist);
        results.push_back({a.name, r});

        std::cout << a.name << ":\n";
        std::cout << "  Distance: " << r.distance << "\n";
        std::cout << "  Time: " << r.microseconds << " us\n";
        std::cout << "  Tour: ";
        for (int v : r.tour) std::cout << v << " ";
        std::cout << "\n\n";
    }

    if (results.empty()) {
        std::cout << "No algorithms registered\n";
        return 0;
    }

    // -------- ЛУЧШИЕ --------
    auto bestByLength = std::min_element(results.begin(), results.end(),
        [](auto& a, auto& b) {
            if (a.second.distance != b.second.distance)
                return a.second.distance < b.second.distance;
            return a.second.microseconds < b.second.microseconds;
        });

    auto fastest = std::min_element(results.begin(), results.end(),
        [](auto& a, auto& b) {
            return a.second.microseconds < b.second.microseconds;
        });

    std::cout << "Best by length: " << bestByLength->first << "\n";
    std::cout << "Fastest: " << fastest->first << "\n";

    return 0;
}