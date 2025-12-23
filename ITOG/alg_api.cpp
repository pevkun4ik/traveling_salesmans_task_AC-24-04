#include "alg_api.h"

std::vector<AlgoEntry>& getAlgorithms() {
    static std::vector<AlgoEntry> algos;
    return algos;
}

void registerAlgorithm(const std::string& name, AlgoFunc func) {
    getAlgorithms().push_back({name, func});
}