#include "ant.h"
#include <algorithm>
#include <cmath>
#include <random>
#include <chrono>

// Инициализация муравья в стартовой вершине
Ant::Ant(std::size_t start)
    : start_location(start),
    current_location(start) {
}

// Генерация случайного числа [0, 1]
double Ant::getRandomChoice() {
    static std::default_random_engine gen(
        std::chrono::system_clock::now().time_since_epoch().count()
    );
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(gen);
}

// Получение всех доступных (непосещённых) соседей
std::vector<std::size_t> Ant::getNeighborVertexes(const Graph<double>& g) {
    std::vector<std::size_t> result;

    for (std::size_t v = 0; v < g.getVertexesCount(); ++v) {
        bool edge_exists = g(current_location, v) != 0;
        bool not_visited =
            std::find(visited.begin(), visited.end(), v) == visited.end();

        if (edge_exists && not_visited)
            result.push_back(v);
    }
    return result;
}

void Ant::MakeChoice(const Graph<double>& g,
    const Matrix<double>& pheromone,
    double alpha,
    double beta) {

    // Первый шаг — добавляем стартовую вершину
    if (path.vertices.empty()) {
        path.vertices.push_back(current_location);
        visited.push_back(current_location);
    }

    auto neighbors = getNeighborVertexes(g);

    // Если соседей нет — возвращаемся в старт
    if (neighbors.empty()) {
        can_continue = false;
        path.vertices.push_back(start_location);
        path.distance += g(current_location, start_location);
        return;
    }

    /*
        Расчёт "желания" перейти в вершину j:

        desire_j = τ_ij^α · (1 / d_ij)^β
    */
    std::vector<double> wish;
    double sum = 0.0;

    for (auto v : neighbors) {
        double tau = pheromone(current_location, v);
        double eta = 1.0 / g(current_location, v);
        double value = std::pow(tau, alpha) * std::pow(eta, beta);
        wish.push_back(value);
        sum += value;
    }

    // Кумулятивные вероятности
    std::vector<double> cumulative(wish.size());
    double acc = 0.0;

    for (std::size_t i = 0; i < wish.size(); ++i) {
        acc += wish[i] / sum;
        cumulative[i] = acc;
    }

    // Рулетка
    double r = getRandomChoice();
    std::size_t next = neighbors.back();

    for (std::size_t i = 0; i < cumulative.size(); ++i) {
        if (r <= cumulative[i]) {
            next = neighbors[i];
            break;
        }
    }

    // Переход в выбранную вершину
    path.vertices.push_back(next);
    path.distance += g(current_location, next);
    visited.push_back(next);
    current_location = next;
}
