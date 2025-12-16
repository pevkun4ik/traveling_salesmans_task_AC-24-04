#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <queue>
#include <iomanip>
#include <chrono>

using namespace std;
using namespace std::chrono;

struct Node {
    int cost;
    vector<vector<int>> matrix;
    vector<pair<int, int>> path;
    int vertex;
    int level;
};

class Compare {
public:
    bool operator()(const Node& a, const Node& b) {
        return a.cost > b.cost;
    }
};

vector<vector<int>> copyMatrix(const vector<vector<int>>& matrix) {
    int n = matrix.size();
    vector<vector<int>> newMatrix(n, vector<int>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            newMatrix[i][j] = matrix[i][j];
        }
    }
    return newMatrix;
}

int reduceMatrix(vector<vector<int>>& matrix) {
    int n = matrix.size();
    int cost = 0;
    
    for (int i = 0; i < n; i++) {
        int minVal = INT_MAX;
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] < minVal) minVal = matrix[i][j];
        }
        if (minVal != INT_MAX && minVal > 0) {
            cost += minVal;
            for (int j = 0; j < n; j++) {
                if (matrix[i][j] != INT_MAX) matrix[i][j] -= minVal;
            }
        }
    }
    
    for (int j = 0; j < n; j++) {
        int minVal = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (matrix[i][j] < minVal) minVal = matrix[i][j];
        }
        if (minVal != INT_MAX && minVal > 0) {
            cost += minVal;
            for (int i = 0; i < n; i++) {
                if (matrix[i][j] != INT_MAX) matrix[i][j] -= minVal;
            }
        }
    }
    
    return cost;
}

Node createNode(vector<vector<int>>& matrix, vector<pair<int, int>>& path, int level, int i, int j) {
    Node node;
    node.level = level;
    node.vertex = j;
    node.path = path;
    
    if (i != -1 && j != -1) node.path.push_back({i, j});
    
    node.matrix = copyMatrix(matrix);
    
    if (level != 0) {
        int n = matrix.size();
        for (int k = 0; k < n; k++) {
            node.matrix[i][k] = INT_MAX;
            node.matrix[k][j] = INT_MAX;
        }
        node.matrix[j][0] = INT_MAX;
    }
    
    node.cost = reduceMatrix(node.matrix);
    return node;
}

pair<int, vector<pair<int, int>>> solveTSP(vector<vector<int>>& costMatrix) {
    int n = costMatrix.size();
    priority_queue<Node, vector<Node>, Compare> pq;
    vector<pair<int, int>> initialPath;
    
    Node root = createNode(costMatrix, initialPath, 0, -1, 0);
    root.cost = reduceMatrix(root.matrix);
    pq.push(root);
    
    int bestCost = INT_MAX;
    vector<pair<int, int>> bestPath;
    
    while (!pq.empty()) {
        Node current = pq.top();
        pq.pop();
        
        if (current.cost >= bestCost) continue;
        
        if (current.level == n - 1) {
            current.path.push_back({current.vertex, 0});
            int totalCost = current.cost;
            
            if (totalCost < bestCost) {
                bestCost = totalCost;
                bestPath = current.path;
            }
            continue;
        }
        
        for (int j = 0; j < n; j++) {
            if (current.matrix[current.vertex][j] != INT_MAX) {
                Node child = createNode(current.matrix, current.path,
                                      current.level + 1, current.vertex, j);
                child.cost += current.cost + current.matrix[current.vertex][j];
                
                if (child.cost < bestCost) pq.push(child);
            }
        }
    }
    
    return {bestCost, bestPath};
}

int main() {
    // Матрица расстояний 7x7
    vector<vector<int>> costMatrix = {
        {0,  8, 10,  3, 12,  3, 15},
        {12, 0,  4,  9, 11, 13, 14},
        {9, 12,  0, 12, 12, 10, 13},
        {13, 15, 18,  0, 17, 11, 19},
        {18, 19, 11, 14,  0, 14,  5},
        {12, 20,  6, 12, 14,  0,  6},
        {11, 20, 11, 10,  8,  1,  0}
    };
    
    // Замер времени
    auto start = high_resolution_clock::now();
    
    // Решение задачи
    auto result = solveTSP(costMatrix);
    int minDistance = result.first;
    vector<pair<int, int>> path = result.second;
    
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    
    // Вывод матрицы расстояний
    cout << "Distance matrix:" << endl;
    int n = costMatrix.size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << setw(5) << costMatrix[i][j];
        }
        cout << endl;
    }
    cout << endl;
    
    // Формирование маршрута
    vector<int> route;
    if (!path.empty()) {
        // Начинаем с первого города первого ребра
        route.push_back(path[0].first);
        
        // Добавляем конечные точки всех ребер
        for (const auto& edge : path) {
            route.push_back(edge.second);
        }
    }
    
    // Вывод лучшего пути
    cout << "Best path: ";
    for (size_t i = 0; i < route.size(); i++) {
        cout << route[i];
        if (i != route.size() - 1) {
            cout << " ";
        }
    }
    
    // Вывод расстояния и времени
    cout << "\nDistance: " << minDistance << endl;
    cout << "Time: " << duration.count() << " microseconds" << endl;
    
    return 0;
}
