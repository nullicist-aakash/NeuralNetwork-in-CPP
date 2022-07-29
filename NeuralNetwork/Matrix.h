#pragma once
#include <vector>
#include <ostream>
#include <utility>

class Matrix
{
public:
    using ld = long double;
    Matrix(size_t, size_t);
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

    std::pair<size_t, size_t> size() const;

    static Matrix Identity(const int&);
    static Matrix diagonalize(const std::vector<ld>&);
    Matrix transpose() const;
private:
    size_t rows, columns;
    std::vector<std::vector<ld>> mat;
};
