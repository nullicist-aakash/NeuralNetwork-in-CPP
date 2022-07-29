#include "Matrix.h"
#include <algorithm>
#include <iterator>
#include <cassert>

using namespace std;

Matrix::Matrix(size_t rows, size_t columns) : rows{ rows }, columns{ columns }
{
    mat.resize(rows, vector<ld>(columns, 0));
}

Matrix::Matrix(const std::pair<size_t, size_t>& pr) : Matrix(pr.first, pr.second)
{

}

Matrix::Matrix(const vector<vector<ld>>& mat) : mat{ mat }, rows{ mat.size() }, columns{ mat[0].size() }
{

}

Matrix::Matrix(const vector<ld>& col_vector) : Matrix(col_vector.size(), 1)
{
    for (int i = 0; i < rows; ++i)
        this->mat[i][0] = col_vector[i];
}


// Copy constructor
Matrix::Matrix(const Matrix& mat) : rows{ mat.rows }, columns{ mat.columns }
{
    this->mat = mat.mat;
}

// Move constructor
Matrix::Matrix(Matrix&& mat) noexcept :
    rows{ std::move(mat.rows) },
    columns{ std::move(mat.columns) }
{
    this->mat = std::move(mat.mat);
}

// Copy Assignment
Matrix& Matrix::operator=(const Matrix& mat)
{
    if (&mat == this)
        return *this;

    this->rows = mat.rows;
    this->columns = mat.columns;
    this->mat = mat.mat;
    return *this;
}

// Move Assignment
Matrix& Matrix::operator=(Matrix&& mat) noexcept
{
    if (&mat == this)
        return *this;

    this->rows = std::move(mat.rows);
    this->columns = std::move(mat.columns);
    this->mat = std::move(mat.mat);
    return *this;
}

Matrix operator+(const Matrix& lhs, const Matrix& rhs)
{
    Matrix temp = lhs;
    temp += rhs;
    return temp;
}

Matrix operator-(const Matrix& lhs, const Matrix& rhs)
{
    Matrix temp = lhs;
    temp -= rhs;
    return temp;
}

Matrix operator*(const Matrix::ld& lhs, const Matrix& rhs)
{
    Matrix temp = rhs;
    temp *= lhs;
    return temp;
}

Matrix operator*(const Matrix& lhs, const Matrix::ld& rhs)
{
    Matrix temp = lhs;
    temp *= rhs;
    return temp;
}

Matrix operator*(const Matrix& lhs, const Matrix& rhs)
{
    Matrix temp = lhs;
    temp *= rhs;
    return temp;
}

Matrix& Matrix::operator+=(const Matrix& rhs)
{
    assert(this->size() == rhs.size());

    for (int i = 0; i < this->rows; ++i)
        for (int j = 0; j < this->columns; ++j)
            this->mat[i][j] += rhs.mat[i][j];

    return *this;
}

Matrix& Matrix::operator-=(const Matrix& rhs)
{
    assert(this->size() == rhs.size());

    for (int i = 0; i < this->rows; ++i)
        for (int j = 0; j < this->columns; ++j)
            this->mat[i][j] -= rhs.mat[i][j];

    return *this;
}

Matrix& Matrix::operator*=(const Matrix& rhs)
{
    auto sz1 = this->size();
    auto sz2 = rhs.size();
    assert(sz1.second == sz2.first);
    Matrix m(sz1.first, sz2.second);

    for (int i = 0; i < sz1.first; ++i)
        for (int j = 0; j < sz2.second; ++j)
            for (int k = 0; k < sz1.second; ++k)
                m.mat[i][j] += this->mat[i][k] * rhs.mat[k][j];

    return *this = std::move(m);
}

Matrix& Matrix::operator*=(const Matrix::ld& constant)
{
    for (int i = 0; i < this->rows; ++i)
        for (int j = 0; j < this->columns; ++j)
            this->mat[i][j] *= constant;

    return *this;
}

std::ostream& operator<< (std::ostream& out, const Matrix& mat)
{
    string outer_separator = "";
    out << "{ ";
    for (auto& row : mat.mat)
    {
        out << outer_separator << "{ ";
        string inner_separator = "";
        for (auto& col : row)
        {
            out << inner_separator << col;
            inner_separator = ", ";
        }
        outer_separator = " }, ";
    }
    out << " } }";
    return out;
}

Matrix::ld Matrix::get(int row, int column) const
{
    return this->mat.at(row).at(column);
}

std::pair<size_t, size_t> Matrix::size() const
{
    return { this->rows, this->columns };
}

Matrix Matrix::Identity(const int& size)
{
    Matrix m(size, size);
    for (int i = 0; i < size; ++i)
        m.mat[i][i] = 1;
    return m;
}

Matrix Matrix::diagonalize(const std::vector<ld>& vec)
{
    Matrix m(vec.size(), vec.size());
    for (int i = 0; i < vec.size(); ++i)
        m.mat[i][i] = vec[i];

    return m;
}

Matrix Matrix::transpose() const
{
    Matrix m(this->columns, this->rows);
    for (int i = 0; i < this->columns; ++i)
        for (int j = 0; j < this->rows; ++j)
            m.mat[i][j] = this->mat[j][i];
    return m;
}


Matrix Matrix::apply(const function<Matrix::ld(Matrix::ld)>& func) const
{
    Matrix temp = *this;
    for (auto& x : temp.mat)
        for (auto& y : x)
            y = func(y);
    return temp;
}

vector<Matrix::ld> Matrix::vec() const
{
    vector<Matrix::ld> ans;
    for (auto& x : this->transpose().mat)
        for (auto& y : x)
            ans.push_back(y);

    return ans;
}