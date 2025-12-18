#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <numeric>
#include <random>
#include <climits>
#include <queue>
#include <iomanip>
#include <string>

// ----------- ANT COLONY ALGORITHM -----------
// Предполагается, что у тебя есть aco.h и классы Graph и AntColonyOptimization
#include "aco.h" // твой файл библиотеки

struct Result {
    std::vector<int> tour;
    double distance;
    long long microseconds;
};

Result runAntAlgorithm(const std::vector<std::vector<double>>& dist) {
    Graph<double> graph(dist.size());
    for (size_t i = 0; i < dist.size(); ++i)
        for (size_t j = 0; j < dist.size(); ++j)
            graph(i, j) = dist[i][j];

    auto start = std::chrono::high_resolution_clock::now();
    AntColonyOptimization aco(graph);
    AntPath result = aco.SolveSalesmansProblem();
    auto end = std::chrono::high_resolution_clock::now();

    long long time_us = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    // Преобразуем std::size_t в int
    std::vector<int> tour_int(result.vertices.begin(), result.vertices.end());

    return {tour_int, result.distance, time_us};
}

// ----------- 2-OPT ALGORITHM -----------
int calculateDistance(const std::vector<std::vector<int>>& dist, const std::vector<int>& path) {
    int total = dist[path.back()][path.front()];
    for (size_t i = 0; i < path.size() - 1; i++) {
        total += dist[path[i]][path[i + 1]];
    }
    return total;
}

std::vector<int> twoOpt(const std::vector<std::vector<int>>& dist, std::vector<int> path) {
    int n = path.size();
    if (n <= 3) return path;
    bool improved = true;
    while (improved) {
        improved = false;
        for (int i = 0; i < n - 1 && !improved; i++) {
            for (int j = i + 1; j < n && !improved; j++) {
                int current = dist[path[i]][path[i + 1]] + dist[path[j]][path[(j + 1) % n]];
                int proposed = dist[path[i]][path[j]] + dist[path[i + 1]][path[(j + 1) % n]];
                if (proposed < current) {
                    std::reverse(path.begin() + i + 1, path.begin() + j + 1);
                    improved = true;
                }
            }
        }
    }
    return path;
}

std::vector<int> createRandomPath(int n) {
    std::vector<int> path(n);
    std::iota(path.begin(), path.end(), 0);
    std::shuffle(path.begin(), path.end(), std::mt19937(std::random_device{}()));
    return path;
}

Result runTwoOpt(const std::vector<std::vector<double>>& dist_double) {
    int n = dist_double.size();
    std::vector<std::vector<int>> dist(n, std::vector<int>(n));
    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
            dist[i][j] = static_cast<int>(dist_double[i][j]);

    auto start = std::chrono::high_resolution_clock::now();
    std::vector<int> initialPath = createRandomPath(n);
    std::vector<int> bestPath = twoOpt(dist, initialPath);
    auto end = std::chrono::high_resolution_clock::now();

    int distance = calculateDistance(dist, bestPath);
    long long time_us = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    return {bestPath, static_cast<double>(distance), time_us};
}

// ----------- BRANCH & BOUND ALGORITHM -----------
struct Node {
    int cost;
    std::vector<std::vector<int>> matrix;
    std::vector<std::pair<int,int>> path;
    int vertex;
    int level;
};

class Compare {
public:
    bool operator()(const Node& a, const Node& b) {
        return a.cost > b.cost;
    }
};

std::vector<std::vector<int>> copyMatrix(const std::vector<std::vector<int>>& matrix) {
    int n = matrix.size();
    std::vector<std::vector<int>> newMatrix(n, std::vector<int>(n));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            newMatrix[i][j] = matrix[i][j];
    return newMatrix;
}

int reduceMatrix(std::vector<std::vector<int>>& matrix) {
    int n = matrix.size();
    int cost = 0;
    for(int i=0;i<n;i++){
        int minVal = INT_MAX;
        for(int j=0;j<n;j++) if(matrix[i][j]<minVal) minVal=matrix[i][j];
        if(minVal!=INT_MAX && minVal>0) {
            cost+=minVal;
            for(int j=0;j<n;j++) if(matrix[i][j]!=INT_MAX) matrix[i][j]-=minVal;
        }
    }
    for(int j=0;j<n;j++){
        int minVal=INT_MAX;
        for(int i=0;i<n;i++) if(matrix[i][j]<minVal) minVal=matrix[i][j];
        if(minVal!=INT_MAX && minVal>0){
            cost+=minVal;
            for(int i=0;i<n;i++) if(matrix[i][j]!=INT_MAX) matrix[i][j]-=minVal;
        }
    }
    return cost;
}

Node createNode(std::vector<std::vector<int>>& matrix, std::vector<std::pair<int,int>>& path, int level, int i, int j){
    Node node;
    node.level=level;
    node.vertex=j;
    node.path=path;
    if(i!=-1 && j!=-1) node.path.push_back({i,j});
    node.matrix=copyMatrix(matrix);
    if(level!=0){
        int n = matrix.size();
        for(int k=0;k<n;k++){
            node.matrix[i][k]=INT_MAX;
            node.matrix[k][j]=INT_MAX;
        }
        node.matrix[j][0]=INT_MAX;
    }
    node.cost = reduceMatrix(node.matrix);
    return node;
}

std::pair<int,std::vector<std::pair<int,int>>> solveTSP(std::vector<std::vector<int>>& costMatrix){
    int n = costMatrix.size();
    std::priority_queue<Node,std::vector<Node>,Compare> pq;
    std::vector<std::pair<int,int>> initialPath;
    Node root = createNode(costMatrix, initialPath, 0, -1, 0);
    root.cost = reduceMatrix(root.matrix);
    pq.push(root);
    int bestCost = INT_MAX;
    std::vector<std::pair<int,int>> bestPath;

    while(!pq.empty()){
        Node current = pq.top(); pq.pop();
        if(current.cost>=bestCost) continue;
        if(current.level==n-1){
            current.path.push_back({current.vertex,0});
            int totalCost = current.cost;
            if(totalCost<bestCost){ bestCost=totalCost; bestPath=current.path;}
            continue;
        }
        for(int j=0;j<n;j++){
            if(current.matrix[current.vertex][j]!=INT_MAX){
                Node child=createNode(current.matrix,current.path,current.level+1,current.vertex,j);
                child.cost+=current.cost+current.matrix[current.vertex][j];
                if(child.cost<bestCost) pq.push(child);
            }
        }
    }
    return {bestCost,bestPath};
}

Result runBranchAndBound(const std::vector<std::vector<double>>& dist_double){
    int n = dist_double.size();
    std::vector<std::vector<int>> dist(n,std::vector<int>(n));
    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
            dist[i][j]=static_cast<int>(dist_double[i][j]);
    auto start = std::chrono::high_resolution_clock::now();
    auto res=solveTSP(dist);
    auto stop = std::chrono::high_resolution_clock::now();
    std::vector<int> route;
    if(!res.second.empty()){
        route.push_back(res.second[0].first);
        for(auto &e: res.second) route.push_back(e.second);
    }
    long long time_us = std::chrono::duration_cast<std::chrono::microseconds>(stop-start).count();
    return {route, static_cast<double>(res.first), time_us};
}

// -------------------- MAIN --------------------

int main() {
    std::vector<std::vector<double>> dist = {
        {0, 8, 10, 3, 12, 3, 15},
        {12, 0, 4, 9, 11, 13, 14},
        {9, 12, 0, 12, 12, 10, 13},
        {13, 15, 18, 0, 17, 11, 19},
        {18, 19, 11, 14, 0, 14, 5},
        {12, 20, 6, 12, 14, 0, 6},
        {11, 20, 11, 10, 8, 1, 0}
    };

    // Вывод входной матрицы
    std::cout << "Input distance matrix:\n";
    for(const auto& row : dist){
        for(double d : row)
            std::cout << std::setw(4) << d << " ";
        std::cout << "\n";
    }
    std::cout << "\n";

    struct AlgResult { std::string name; Result res; };
    std::vector<AlgResult> results;
    results.push_back({"Ant Colony", runAntAlgorithm(dist)});
    results.push_back({"2-Opt", runTwoOpt(dist)});
    results.push_back({"Branch & Bound", runBranchAndBound(dist)});

    // Вывод результатов каждого алгоритма
    for(auto &r: results){
        std::cout << r.name << ":\n";
        std::cout << "  Distance: " << r.res.distance << "\n";
        std::cout << "  Time: " << r.res.microseconds << " us\n";
        std::cout << "  Tour: ";
        for(int v: r.res.tour) std::cout << v << " ";
        std::cout << "\n\n";
    }

    // ---------- Определяем лучший алгоритм по длине (с учётом времени при равенстве) ----------
    double minDistance = std::min_element(results.begin(), results.end(),
        [](auto &a, auto &b){ return a.res.distance < b.res.distance; })->res.distance;

    std::vector<AlgResult*> bestByLengthCandidates;
    for (auto &r : results)
        if (r.res.distance == minDistance)
            bestByLengthCandidates.push_back(&r);

    AlgResult* bestByLength = bestByLengthCandidates[0];
    for (auto *r : bestByLengthCandidates)
        if (r->res.microseconds < bestByLength->res.microseconds)
            bestByLength = r;

    std::cout << "Best by length (fastest if tie): " << bestByLength->name << "\n";

    // ---------- Самый быстрый алгоритм ----------
    auto bestTime = std::min_element(results.begin(), results.end(),
        [](auto &a, auto &b){ return a.res.microseconds < b.res.microseconds; });
    std::cout << "Fastest algorithm: " << bestTime->name << "\n";

    return 0;
}