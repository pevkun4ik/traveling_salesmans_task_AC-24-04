#pragma once
#include <vector>
#include <cstddef>
#include "graph.h"
#include "matrix.h"

/*
    AntPath — путь муравья:
    vertices — последовательность вершин
    distance — длина маршрута
*/
struct AntPath {
    std::vector<std::size_t> vertices;
    double distance = 0.0;
};

/*
    Ant — один муравей в алгоритме
*/
struct Ant {
    explicit Ant(std::size_t start = 0);

    AntPath path;                  // текущий путь
    std::vector<std::size_t> visited; // посещённые вершины

    std::size_t start_location;    // стартовая вершина
    std::size_t current_location;  // текущая вершина
    bool can_continue = true;      // может ли муравей идти дальше

    /*
        Выбор следующей вершины по формуле вероятности:

        P(i → j) =
        (τ_ij^α · (1 / d_ij)^β) /
        Σ(τ_ik^α · (1 / d_ik)^β)
    */
    void MakeChoice(const Graph<double>& g,
        const Matrix<double>& pheromone,
        double alpha,
        double beta);

private:
    // Случайное число [0, 1]
    double getRandomChoice();

    // Непосещённые соседние вершины
    std::vector<std::size_t> getNeighborVertexes(const Graph<double>& g);
};
