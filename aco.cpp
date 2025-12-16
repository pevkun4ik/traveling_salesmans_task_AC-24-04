#include "aco.h"
#include <limits>

// Инициализация алгоритма
AntColonyOptimization::AntColonyOptimization(const Graph<double>& graph)
    : graph_(graph),
    kQ_(0.015 * graph.getGraphWeight()) {

    std::size_t n = graph_.getVertexesCount();
    pheromone_ = Matrix<double>(n);

    // Начальное заполнение феромона
    for (std::size_t i = 0; i < n; ++i)
        for (std::size_t j = 0; j < n; ++j)
            if (i != j)
                pheromone_(i, j) = kPheromone0_;
}

// Создание муравьёв (по одному на вершину)
void AntColonyOptimization::CreateAnts() {
    ants_.clear();
    for (std::size_t i = 0; i < graph_.getVertexesCount(); ++i)
        ants_.emplace_back(i);
}

// Глобальное обновление феромона
/*
    ?_ij = (1 ? ?) · ?_ij + ??_ij
*/
void AntColonyOptimization::UpdateGlobalPheromone(
    const Matrix<double>& local_update) {

    for (std::size_t i = 0; i < pheromone_.getRows(); ++i)
        for (std::size_t j = 0; j < pheromone_.getRows(); ++j)
            pheromone_(i, j) =
            (1.0 - kEvaporation_) * pheromone_(i, j) + local_update(i, j);
}

AntPath AntColonyOptimization::SolveSalesmansProblem() {
    AntPath best;
    best.distance = std::numeric_limits<double>::max();

    std::size_t n = graph_.getVertexesCount();

    // Основной цикл алгоритма
    for (std::size_t iter = 0; iter < kIterations_; ++iter) {
        CreateAnts();
        Matrix<double> local_pheromone(n);

        for (auto& ant : ants_) {
            // Построение маршрута муравьём
            while (ant.can_continue)
                ant.MakeChoice(graph_, pheromone_, kAlpha_, kBeta_);

            // Обновление лучшего пути
            if (ant.path.distance < best.distance)
                best = ant.path;

            /*
                Локальное обновление феромона:
                ??_ij = Q / L
            */
            double delta = kQ_ / ant.path.distance;

            for (std::size_t i = 1; i < ant.path.vertices.size(); ++i) {
                std::size_t from = ant.path.vertices[i - 1];
                std::size_t to = ant.path.vertices[i];
                local_pheromone(from, to) += delta;
                local_pheromone(to, from) += delta;
            }
        }

        UpdateGlobalPheromone(local_pheromone);
    }

    return best;
}
