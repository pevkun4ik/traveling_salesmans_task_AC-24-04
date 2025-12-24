#pragma once
#include "matrix.h"

/*
    graph(i, j) - расстояние между городами i и j
*/
template<typename T>
class Graph : public Matrix<T> {
public:
    explicit Graph(std::size_t n) : Matrix<T>(n) {}


    std::size_t getVertexesCount() const {
        return this->getRows();
    }

    // Суммарный вес графа (нужен для Q)
    T getGraphWeight() const {
        T sum = 0;
        for (std::size_t i = 0; i < this->getRows(); ++i)
            for (std::size_t j = 0; j < this->getRows(); ++j)
                sum += (*this)(i, j);
        return sum;
    }
};
