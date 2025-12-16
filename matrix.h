#pragma once
#include <vector>
#include <cstddef>

/*
    Класс Matrix — базовая квадратная матрица N×N.
    Используется:
    - как матрица расстояний
    - как матрица феромонов
*/
template<typename T>
class Matrix {
public:
    Matrix() = default;

    // Создание матрицы N×N, заполненной нулями
    explicit Matrix(std::size_t n) : data_(n, std::vector<T>(n, 0)) {}

    // Доступ к элементу (i, j)
    T& operator()(std::size_t i, std::size_t j) {
        return data_[i][j];
    }

    // Константный доступ
    T operator()(std::size_t i, std::size_t j) const {
        return data_[i][j];
    }

    // Количество строк (и столбцов)
    std::size_t getRows() const {
        return data_.size();
    }

protected:
    std::vector<std::vector<T>> data_;
};
