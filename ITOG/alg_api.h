#pragma once
#include <vector>
#include <string>
#include <functional>

struct Result {
    std::vector<int> tour;
    double distance;
    long long microseconds;
};

using AlgoFunc = std::function<Result(
    const std::vector<std::vector<double>>&)>;

struct AlgoEntry {
    std::string name;
    AlgoFunc func;
};

std::vector<AlgoEntry>& getAlgorithms();
void registerAlgorithm(const std::string& name, AlgoFunc func);