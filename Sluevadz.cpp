#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <iomanip>
#include <cstdio>
#include <numeric>

using namespace std;
using namespace std::chrono;

int calculateDistance(const vector<vector<int>>& dist, const vector<int>& path) {
    int total = dist[path.back()][path.front()];
    for (size_t i = 0; i < path.size() - 1; i++) {
        total += dist[path[i]][path[i + 1]];
    }
    return total;
}

vector<int> twoOpt(const vector<vector<int>>& dist, vector<int> path) {
    int n = path.size();
    if (n <= 3) return path;
    bool improved = true;
    while (improved) {
        improved = false;
        for (int i = 0; i < n - 1 && !improved; i++) {
            for (int j = i + 1; j < n && !improved; j++) {
                int current = dist[path[i]][path[i + 1]] + 
                             dist[path[j]][path[(j + 1) % n]];
                int proposed = dist[path[i]][path[j]] + 
                              dist[path[i + 1]][path[(j + 1) % n]];
                if (proposed < current) {
                    reverse(path.begin() + i + 1, path.begin() + j + 1);
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

int main() {
    bool useFixedMatrix = true;
    vector<vector<int>> distanceMatrix;
    int nCities = 7;
    if (useFixedMatrix) {
        distanceMatrix = {
            {0, 8, 10, 3, 12, 3, 15},
            {12, 0, 4, 9, 11, 13, 14},
            {9, 12, 0, 12, 12, 10, 13},
            {13, 15, 18, 0, 17, 11, 19},
            {18, 19, 11, 14, 0, 14, 5},
            {12, 20, 6, 12, 14, 0, 6},
            {11, 20, 11, 10, 8, 1, 0}
        };
    } else {
        distanceMatrix.assign(nCities, vector<int>(nCities, 0));
        mt19937 gen(random_device{}());
        uniform_int_distribution<> dist(1, 20);
        for (int i = 0; i < nCities; ++i)
            for (int j = i + 1; j < nCities; ++j)
                distanceMatrix[i][j] = distanceMatrix[j][i] = dist(gen);
    }
     printf("Distance matrix:\n");
    for (int i = 0; i < nCities; ++i) {
        for (int j = 0; j < nCities; ++j) {
            printf("%4d", distanceMatrix[i][j]);
        }
        putchar('\n');
    }
    putchar('\n');
    vector<int> initialPath = createRandomPath(nCities);
    auto start = high_resolution_clock::now();
    vector<int> bestPath = twoOpt(distanceMatrix, initialPath);
    auto stop = high_resolution_clock::now();
    cout << "Best path: ";
    for (size_t i = 0; i < bestPath.size(); i++)
        cout << bestPath[i] << (i < bestPath.size() - 1 ? " " : "");
    cout << " " << bestPath[0] << endl;
    cout << "Distance: " << calculateDistance(distanceMatrix, bestPath) << endl;
    cout << "Time: " << duration_cast<microseconds>(stop - start).count() << " microseconds" << endl;
    return 0;
}