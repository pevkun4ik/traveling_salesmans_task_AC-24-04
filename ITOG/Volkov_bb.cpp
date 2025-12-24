#include "Volkov_bb.h"
#include <vector>
#include <chrono>
#include <climits>
#include <cfloat>

using namespace std;
using namespace std::chrono;

static double bestDistance;
static vector<int> bestTour;

/* Рекурсивный обход с отсечением */
void dfs(
    const vector<vector<double>>& dist,
    vector<int>& currentTour,
    vector<bool>& used,
    double currentDist
) {
    int n = dist.size();

    // ОТСЕЧЕНИЕ
    if (currentDist >= bestDistance)
        return;

    // Если маршрут полный
    if ((int)currentTour.size() == n) {
        double total = currentDist + dist[currentTour.back()][currentTour[0]];
        if (total < bestDistance) {
            bestDistance = total;
            bestTour = currentTour;
        }
        return;
    }

    // Перебор следующей вершины
    for (int i = 0; i < n; i++) {
        if (!used[i]) {
            used[i] = true;
            currentTour.push_back(i);

            double add = dist[currentTour[currentTour.size() - 2]][i];
            dfs(dist, currentTour, used, currentDist + add);

            currentTour.pop_back();
            used[i] = false;
        }
    }
}

Result runBranchBound(const vector<vector<double>>& dist) {
    auto start = high_resolution_clock::now();

    int n = dist.size();
    bestDistance = DBL_MAX;
    bestTour.clear();

    vector<bool> used(n, false);
    vector<int> currentTour;

    // стартуем с вершины 0
    currentTour.push_back(0);
    used[0] = true;

    dfs(dist, currentTour, used, 0.0);

    // замыкаем цикл
    bestTour.push_back(0);

    auto stop = high_resolution_clock::now();
    return {
        bestTour,
        bestDistance,
        duration_cast<microseconds>(stop - start).count()
    };
}

/* Регистрация */
static bool registered = []() {
    registerAlgorithm("Branch & Bound", runBranchBound);
    return true;
}();