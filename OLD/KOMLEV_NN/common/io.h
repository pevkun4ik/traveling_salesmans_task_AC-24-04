#pragma once
#include <string>
#include <vector>

namespace common {

struct TSPInstance {
    // dist[i][j] is the distance/cost from i to j (double; can be asymmetric).
    std::vector<std::vector<double>> dist;
};

// Reads a square distance matrix.
// File format (whitespace separated):
//   n
//   d00 d01 ... d0(n-1)
//   d10 d11 ... d1(n-1)
//   ...
//   d(n-1)0 ... d(n-1)(n-1)
//
// Notes:
// - You may set diagonal to 0.
// - For disallowed edges, you may use a very large number (e.g., 1e18).
TSPInstance read_distance_matrix_from_istream(std::istream& in);

// Convenience wrapper: read from file path (throws on errors).
TSPInstance read_distance_matrix_from_file(const std::string& path);

// Writes a tour to stdout in a consistent format:
//   length: <value>
//   tour: i0 i1 ... i(n-1) i0
// (tour is 0-based indices by default; you may request 1-based in caller)
void write_tour(std::ostream& out,
                const std::vector<int>& tour, // must include return-to-start (last == first)
                double length,
                bool one_based=false);

} // namespace common
