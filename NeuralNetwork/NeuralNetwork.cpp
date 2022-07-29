#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <cassert>
#include <cmath>
#include "Matrix.h"

using namespace std;
using ld = long double;

namespace Dataset
{
    enum class IRISSpecies
    {
        IRIS_SETOSA,
        IRIS_VERSICOLOR,
        IRIS_VIRGINICA
    };

    struct IRISEntry
    {
        ld sepal_length = 0;
        ld sepal_width = 0;
        ld petal_length = 0;
        ld petal_width = 0;
        IRISSpecies species = static_cast<IRISSpecies>(0);

        vector<ld> x;
        vector<ld> y;

        friend std::istream& operator>> (std::istream&, IRISEntry&);
        friend std::ostream& operator<< (std::ostream&, IRISEntry&);
    };

    std::istream& operator>> (std::istream& in, IRISEntry& iris)
    {
        in >> iris.sepal_length;
        in.ignore();
        in >> iris.sepal_width;
        in.ignore();
        in >> iris.petal_length;
        in.ignore();
        in >> iris.petal_width;
        in.ignore();
        string s;
        in >> s;
        if (s == "Iris-setosa")
            iris.species = IRISSpecies::IRIS_SETOSA;
        else if (s == "Iris-versicolor")
            iris.species = IRISSpecies::IRIS_VERSICOLOR;
        else
            iris.species = IRISSpecies::IRIS_VIRGINICA;

        iris.x = { iris.sepal_length, iris.sepal_width, iris.petal_length, iris.petal_width };
        iris.y = { 0,0,0 };
        iris.y[(int)iris.species] = 1;

        return in;
    }

    std::ostream& operator<< (std::ostream& out, IRISEntry& iris)
    {
        out << iris.sepal_length << "\t" << iris.sepal_width << "\t"
            << iris.petal_length << "\t" << iris.petal_width << "\t"
            << (int)iris.species;

        return out;
    }

    vector<IRISEntry> load_data()
    {
        vector<IRISEntry> vec;
        fstream file("IRIS.csv", ios::in);
        if (!file.is_open())
            return vec;

        string line;
        for (int i = 0; getline(file >> ws, line); i++)
        {
            if (i == 0)
                continue;

            IRISEntry e;
            stringstream sstr(line);
            sstr >> e;
            vec.push_back(e);
        }
        return vec;
    }
}

class NeuralNetwork
{
    vector<Matrix> W;
    vector<Matrix> b;
    size_t layer_count;

    static ld sigma(ld x)
    {
        return 1 / (1 + exp(-x));
    }

    static ld sigma_derivative(ld x)
    {
        return sigma(x) * (1 - sigma(x));
    }

    static Matrix sigma_derivative(vector<ld>&& z)
    {
        for (auto& zi : z)
            zi = sigma_derivative(zi);
        return Matrix::diagonalize(z);
    }

    pair<vector<Matrix>, vector<Matrix>> forward(const vector<ld>& x)
    {
        vector<Matrix> Z, X;
        Z.push_back(Matrix::Identity(0));
        X.push_back(x);

        for (size_t i = 1; i < layer_count; ++i)
        {
            Matrix new_z = std::move(W[i] * X.back());
            new_z += b[i];
            Z.push_back(std::move(new_z));
            X.push_back(Z.back().apply(sigma));
        }

        return { X, Z };
    }

public:
    NeuralNetwork(const vector<size_t>& layer_sizes)
        : layer_count{ layer_sizes.size() }
    {
        assert(layer_sizes.size() > 1);

        // To satisfy the equation X(n) = W(n)X(n-1) + b(n)
        W.push_back(Matrix::Identity(0));
        b.push_back(Matrix::Identity(0));
        for (int i = 1; i < layer_sizes.size(); ++i)
        {
            W.push_back(std::move(Matrix(layer_sizes[i], layer_sizes[i - 1])));
            b.push_back(std::move(Matrix(layer_sizes[i], 1)));
        }
    }

    void train(const vector<ld>& x, const vector<ld>& y, ld alpha)
    {
        assert(x.size() == W[1].size().second);
        assert(y.size() == b.back().size().first);

        // forward pass
        auto res = forward(x);
        vector<Matrix> X = std::move(res.first);
        vector<Matrix> Z = std::move(res.second);

        // backward pass, compute deltas
        vector<Matrix> delW, delb;
        delW.resize(W.size());
        delb.resize(b.size());

        Matrix delXi = X.back() - Matrix(y);
        for (size_t i = layer_count - 1; i > 0; --i)
        {
            Matrix delZi = sigma_derivative(Z[i].vec()) * delXi;
            delb[i] = delZi;
            delW[i] = delZi * X[i - 1].transpose();
            
            if (i == 1)
                continue;

            delXi = W[i].transpose() * delZi;
        }

        // Update parameters
        for (int i = 1; i < layer_count; ++i)
        {
            W[i] -= delW[i] * alpha;
            b[i] -= delb[i] * alpha;
        }
    }

    vector<ld> evaluate(const vector<ld>& x)
    {
        Matrix X(x);
        for (int i = 1; i < layer_count; ++i)
        {
            Matrix Z = W[i] * X;
            Z += b[i];
            X = Z.apply(sigma);
        }
        return X.vec();
    }
};

int main()
{
    auto res = Dataset::load_data();
    auto nn = NeuralNetwork({ 4,3 });
    const int epochs = 10000;

    for (int epoch = 1; epoch <= epochs; ++epoch)
    {
        for (auto& data : res)
            nn.train(data.x, data.y, 0.1);

        if (epoch % 10 != 0)
            continue;

        ld error = 0;

        for (auto& data : res)
        {
            auto res = nn.evaluate(data.x);
            auto temp = Matrix(data.y) - Matrix(res);
            error += ((0.5 * temp.transpose()) * temp).vec()[0];
        }
        cout << "After epoch " << epoch << "/" << epochs << ", error: " << error << endl;
    }

}