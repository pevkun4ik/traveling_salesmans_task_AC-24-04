#pragma once
#include <vector>
#include <cstddef>

/*
    Класс Matrix — базовая квадратная матрица N×N.
    Используется: как матрица расстояний, как матрица феромонов
*/
template<typename T>
class Matrix {
public:
    Matrix() = default;

    explicit Matrix(std::size_t n) : data_(n, std::vector<T>(n, 0)) {}

    T& operator()(std::size_t i, std::size_t j) {
        return data_[i][j];
    }

    T operator()(std::size_t i, std::size_t j) const {
        return data_[i][j];
    }

    std::size_t getRows() const {
        return data_.size();
    }

protected:
    std::vector<std::vector<T>> data_;
};

