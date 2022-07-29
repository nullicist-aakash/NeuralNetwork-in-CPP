#pragma once
#include <vector>
#include <ostream>
#include <utility>
#include <functional>
#include <iostream>

class Matrix
{
public:
    using ld = long double;
    Matrix(size_t = 0, size_t = 0);
    Matrix(const std::pair<size_t, size_t>&);
    Matrix(const std::vector<std::vector<ld>>& mat);
    Matrix(const std::vector<ld>& col_vector);
    
    Matrix(const Matrix& mat);                  // Copy constructor
    Matrix(Matrix&& mat) noexcept;              // Move constructor
    Matrix& operator=(const Matrix& mat);       // Copy Assignment
    Matrix& operator=(Matrix&& mat) noexcept;   // Move Assignment

    // Operators
    friend Matrix operator+(const Matrix&, const Matrix&);
    friend Matrix operator-(const Matrix&, const Matrix&);
    friend Matrix operator*(const ld&, const Matrix&);
    friend Matrix operator*(const Matrix&, const ld&);
    friend Matrix operator*(const Matrix&, const Matrix&);
    Matrix& operator+=(const Matrix&);
    Matrix& operator-=(const Matrix&);
    Matrix& operator*=(const Matrix&);
    Matrix& operator*=(const ld&);
    friend std::ostream& operator<< (std::ostream&, const Matrix&);

    static Matrix Identity(const int&);
    static Matrix diagonalize(const std::vector<ld>&);

    ld get(int, int) const;
    std::pair<size_t, size_t> size() const;

    Matrix transpose() const;
    Matrix apply(const std::function<ld(ld)>&) const;
    std::vector<ld> vec() const;

private:
    size_t rows, columns;
    std::vector<std::vector<ld>> mat;
};
