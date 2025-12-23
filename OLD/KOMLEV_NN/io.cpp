#include "common/io.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <limits>

namespace common {

static void require(bool cond, const std::string& msg) {
    if (!cond) throw std::runtime_error(msg);
}

TSPInstance read_distance_matrix_from_istream(std::istream& in) {
    TSPInstance inst;
    int n = 0;
    require(static_cast<bool>(in >> n), "Failed to read n.");
    require(n > 0, "n must be > 0.");

    inst.dist.assign(n, std::vector<double>(n, 0.0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            double v;
            require(static_cast<bool>(in >> v), "Failed to read distance matrix element.");
            inst.dist[i][j] = v;
        }
    }
    return inst;
}

TSPInstance read_distance_matrix_from_file(const std::string& path) {
    std::ifstream f(path);
    if (!f.is_open()) throw std::runtime_error("Cannot open file: " + path);
    return read_distance_matrix_from_istream(f);
}

void write_tour(std::ostream& out,
                const std::vector<int>& tour,
                double length,
                bool one_based) {
    out.setf(std::ios::fixed);
    out.precision(10);

    out << "length: " << length << "\n";
    out << "tour: ";
    for (size_t i = 0; i < tour.size(); ++i) {
        int v = tour[i] + (one_based ? 1 : 0);
        out << v << (i + 1 < tour.size() ? ' ' : '\n');
    }
}

} // namespace common
