#pragma once
#include <vector>
#include "ant.h"

/*
    AntColonyOptimization Ч реализаци€ муравьиного алгоритма
*/
class AntColonyOptimization {
public:
    explicit AntColonyOptimization(const Graph<double>& graph);

    AntPath SolveSalesmansProblem();

private:
    const double kAlpha_ = 1.0;        // вли€ние феромона
    const double kBeta_ = 2.0;         // вли€ние рассто€ни€
    const double kPheromone0_ = 1.0;   // начальный феромон
    const double kEvaporation_ = 0.2;  // испарение феромона
    const std::size_t kIterations_ = 50; // число итераций

    Graph<double> graph_;
    double kQ_;                        // коэффициент обновлени€
    Matrix<double> pheromone_;         // матрица феромонов
    std::vector<Ant> ants_;

    void CreateAnts();
    void UpdateGlobalPheromone(const Matrix<double>& local_update);
};

