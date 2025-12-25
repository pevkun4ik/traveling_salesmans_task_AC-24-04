#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <random>
#include <fstream>
#include <string>

#include "alg_api.h"

// алгоритмы
#include "Leonidova_ant.h"
#include "Slueva_opt2.h"
#include "Volkov_bb.h"

using namespace std;

/* ---------- генерация случайной матрицы ---------- */
vector<vector<double>> generateMatrix(int n) {
    vector<vector<double>> m(n, vector<double>(n, 0));

    mt19937 gen(random_device{}());
    uniform_int_distribution<> d(1, 20);

    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            m[i][j] = m[j][i] = d(gen);

    return m;
}

/* ---------- чтение матрицы из файла ---------- */
vector<vector<double>> loadMatrix(const string& filename) {
    ifstream f(filename);

    int n;
    f >> n;

    vector<vector<double>> m(n, vector<double>(n));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            f >> m[i][j];

    return m;
}

int main(int argc, char* argv[]) {

    vector<vector<double>> dist;

    // --- выбор источника данных ---
    if (argc == 2) {
        string arg = argv[1];

        if (isdigit(arg[0])) {
            dist = generateMatrix(stoi(arg));
        } else {
            dist = loadMatrix(arg);
        }
    } else {
        dist = generateMatrix(10);
    }

    // --- вывод матрицы ---
    cout << "Distance matrix:\n";
    for (auto& r : dist) {
        for (double v : r)
            cout << setw(4) << v << " ";
        cout << "\n";
    }
    cout << "\n";

    // --- запуск алгоритмов ---
    auto& algos = getAlgorithms();

    for (auto& a : algos) {
        Result r = a.func(dist);

        cout << a.name << ":\n";
        cout << "  Distance: " << r.distance << "\n";
        cout << "  Time: " << r.microseconds << " us\n";
        cout << "  Tour: ";
        for (int v : r.tour) cout << v << " ";
        cout << "\n\n";
    }

    return 0;
}