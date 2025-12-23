#include <iostream>
#include <random>
#include <chrono>
#include <iomanip>
#include "aco.h"

int main() {
    const std::size_t N = 7; //кол-во вершин
    Graph<double> graph(N);

    //Пример матрицы расстояний 7x7:
        double dist[N][N] = {
            {0, 8, 10, 3, 12, 3, 15},
            {12, 0, 4, 9, 11, 13, 14},
            {9, 12, 0, 12, 12, 10, 13},
            {13, 15, 18, 0, 17, 11, 19},
            {18, 19, 11, 14, 0, 14, 5},
            {12, 20, 6, 12, 14, 0, 6},
            {11, 20, 11, 10, 8, 1, 0}
        };

        // Копирование значений в граф
    for (std::size_t i = 0; i < N; ++i)
        for (std::size_t j = 0; j < N; ++j)
            graph(i, j) = dist[i][j];

    // // --- СЛУЧАЙНАЯ ГЕНЕРАЦИЯ МАТРИЦЫ ---
    // std::mt19937 gen(std::random_device{}());
    // std::uniform_int_distribution<> dist(1, 20);

    // for (std::size_t i = 0; i < N; ++i)
    //     for (std::size_t j = 0; j < N; ++j)
    //         if (i != j)
    //             graph(i, j) = dist(gen);

    // Вывод матрицы расстояний
    std::cout << "Distance matrix:\n";
    for (std::size_t i = 0; i < N; ++i) {
        for (std::size_t j = 0; j < N; ++j) {
            std::cout << std::setw(4) << graph(i, j) << " ";
        }
        std::cout << '\n';
    }
    std::cout << std::endl;

    // Засекаем время работы алгоритма
    auto start = std::chrono::high_resolution_clock::now();

    AntColonyOptimization aco(graph);
    AntPath result = aco.SolveSalesmansProblem();

    auto end = std::chrono::high_resolution_clock::now();
    auto time_ms =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    // Вывод результата
    std::cout << "Best path: ";
    for (auto v : result.vertices)
        std::cout << v << " ";
    std::cout << "\nDistance: " << result.distance << std::endl;
    std::cout << "Time: " << time_ms << " microseconds\n";

    return 0;
}
