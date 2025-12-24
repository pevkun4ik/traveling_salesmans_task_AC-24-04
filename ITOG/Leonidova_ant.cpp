#include "Leonidova_ant.h"
#include "aco.h"
#include <chrono>

Result LeonidovaAntSolver(const std::vector<std::vector<double>>& dist)
{
    std::size_t n = dist.size();
    Graph<double> graph(n);
    for (std::size_t i = 0; i < n; ++i)
        for (std::size_t j = 0; j < n; ++j)
            graph(i, j) = dist[i][j];

    auto start = std::chrono::high_resolution_clock::now();

    AntColonyOptimization aco(graph);
    AntPath best = aco.SolveSalesmansProblem();
    auto end = std::chrono::high_resolution_clock::now();

    Result r;
    r.distance = best.distance;
    r.microseconds =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    for (auto v : best.vertices)
        r.tour.push_back(static_cast<int>(v));

    return r;
}


void registerLeonidovaAnt() {
    registerAlgorithm("Leonidova ACO", LeonidovaAntSolver);
}

namespace {
    bool registered = []() {
        registerLeonidovaAnt();
        return true;
        }();
}