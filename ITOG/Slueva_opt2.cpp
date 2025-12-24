#include "Slueva_opt2.h"
#include "alg_api.h"
#include <vector>
#include <numeric>
#include <algorithm>
#include <random>
#include <chrono>

using namespace std;
using namespace std::chrono;

int calculateDistance(const vector<vector<int>>& dist, const vector<int>& path) {
    int total = 0;
    for(size_t i=0;i<path.size()-1;i++)
        total += dist[path[i]][path[i+1]];
    total += dist[path.back()][path.front()];
    return total;
}

vector<int> twoOptAlgorithm(const vector<vector<int>>& dist, vector<int> path) {
    int n = path.size();
    bool improved = true;
    while(improved) {
        improved = false;
        for(int i=0;i<n-1 && !improved;i++) {
            for(int j=i+1;j<n && !improved;j++) {
                int curr = dist[path[i]][path[i+1]] + dist[path[j]][path[(j+1)%n]];
                int prop = dist[path[i]][path[j]] + dist[path[i+1]][path[(j+1)%n]];
                if(prop<curr) {
                    reverse(path.begin()+i+1, path.begin()+j+1);
                    improved = true;
                }
            }
        }
    }
    return path;
}

vector<int> createRandomPath(int n) {
    vector<int> path(n);
    iota(path.begin(), path.end(), 0);
    shuffle(path.begin(), path.end(), mt19937(random_device{}()));
    return path;
}

Result runTwoOpt(const vector<vector<double>>& dist_double) { // менять не надо!!!
    int n = dist_double.size();
    vector<vector<int>> dist(n, vector<int>(n));
    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
            dist[i][j] = static_cast<int>(dist_double[i][j]);

    auto start = high_resolution_clock::now();
    vector<int> initialPath = createRandomPath(n);
    vector<int> bestPath = twoOptAlgorithm(dist, initialPath);
    auto stop = high_resolution_clock::now();
    int distance = calculateDistance(dist, bestPath);

    return {bestPath, static_cast<double>(distance), duration_cast<microseconds>(stop-start).count()};
}

// Регистрация
static bool registered = [](){
    registerAlgorithm("2-Opt", runTwoOpt);
    return true;
}();