#include "common/io.h"
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <limits>
#include <algorithm>

static void usage(const char* prog) {
    std::cerr
        << "Usage:\n"
        << "  " << prog << " --input <path> [--start <idx>] [--one-based-output]\n"
        << "  " << prog << " < input.txt\n\n"
        << "Input format:\n"
        << "  n\\n"
        << "  n lines with n numbers (distance matrix)\n\n"
        << "Notes:\n"
        << "- Indices for --start are 0-based.\n"
        << "- Output tour always includes return to start (last vertex == first).\n";
}

static double tour_length(const std::vector<std::vector<double>>& dist,
                          const std::vector<int>& tour) {
    double len = 0.0;
    for (size_t i = 1; i < tour.size(); ++i) {
        len += dist[tour[i-1]][tour[i]];
    }
    return len;
}

// Nearest Neighbor heuristic for TSP:
// start -> repeatedly go to the nearest unvisited vertex -> return to start.
static std::vector<int> nearest_neighbor_tour(const std::vector<std::vector<double>>& dist, int start) {
    const int n = static_cast<int>(dist.size());
    std::vector<int> tour;
    tour.reserve(n + 1);

    std::vector<char> used(n, 0);
    int cur = start;
    used[cur] = 1;
    tour.push_back(cur);

    for (int step = 1; step < n; ++step) {
        double best_d = std::numeric_limits<double>::infinity();
        int best_j = -1;

        for (int j = 0; j < n; ++j) {
            if (used[j]) continue;
            double d = dist[cur][j];
            // If there are NaNs or negative costs, we still allow them,
            // but infinities effectively disable edges.
            if (d < best_d) {
                best_d = d;
                best_j = j;
            }
        }

        if (best_j == -1 || !std::isfinite(best_d)) {
            throw std::runtime_error("No feasible next vertex found (disconnected graph or INF edges).");
        }

        cur = best_j;
        used[cur] = 1;
        tour.push_back(cur);
    }

    tour.push_back(start); // return
    return tour;
}

int main(int argc, char** argv) {
    try {
        std::string input_path;
        int start = 0;
        bool one_based_out = false;

        for (int i = 1; i < argc; ++i) {
            std::string a = argv[i];
            if (a == "--help" || a == "-h") {
                usage(argv[0]);
                return 0;
            } else if (a == "--input" && i + 1 < argc) {
                input_path = argv[++i];
            } else if (a == "--start" && i + 1 < argc) {
                start = std::stoi(argv[++i]);
            } else if (a == "--one-based-output") {
                one_based_out = true;
            } else {
                std::cerr << "Unknown argument: " << a << "\n";
                usage(argv[0]);
                return 2;
            }
        }

        common::TSPInstance inst;
        if (!input_path.empty()) {
            inst = common::read_distance_matrix_from_file(input_path);
        } else {
            inst = common::read_distance_matrix_from_istream(std::cin);
        }

        const int n = static_cast<int>(inst.dist.size());
        if (start < 0 || start >= n) throw std::runtime_error("start out of range.");

        auto tour = nearest_neighbor_tour(inst.dist, start);
        double len = tour_length(inst.dist, tour);

        common::write_tour(std::cout, tour, len, one_based_out);
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}
