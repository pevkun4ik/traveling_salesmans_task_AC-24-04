#include <iostream>
#include <random>
#include <chrono>
#include <iomanip>
#include "aco.h"

int main() {
    const std::size_t N = 7; //кол-во вершин
    Graph<double> graph(N);

    /*
    ==================== РУЧНОЕ ЗАДАНИЕ МАТРИЦЫ ====================

    При необходимости матрицу расстояний можно задать ВРУЧНУЮ,
    а не генерировать случайным образом.

    Пример матрицы расстояний 7x7:
        double dist[N][N] = {
            { 0, 10, 15, 20, 25, 30, 35 },
            { 10, 0, 12, 18, 22, 28, 32 },
            { 15, 12, 0, 14, 19, 26, 29 },
            { 20, 18, 14, 0, 16, 24, 27 },
            { 25, 22, 19, 16, 0, 21, 23 },
            { 30, 28, 26, 24, 21, 0, 17 },
            { 35, 32, 29, 27, 23, 17, 0 }
        };

        // Копирование значений в граф
        for (std::size_t i = 0; i < N; ++i)
            for (std::size_t j = 0; j < N; ++j)
                graph(i, j) = dist[i][j];

    Для использования ручной матрицы достаточно закомментировать
    случайную генерацию ниже и раскомментировать этот код.

    =================================================================
    */
    // --- СЛУЧАЙНАЯ ГЕНЕРАЦИЯ МАТРИЦЫ ---
    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> dist(1, 20);

    for (std::size_t i = 0; i < N; ++i)
        for (std::size_t j = 0; j < N; ++j)
            if (i != j)
                graph(i, j) = dist(gen);

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
