#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <cassert>
#include "Matrix.h"


using namespace std;
using ld = long double;

enum class IRISSpecies
{
    IRIS_SETOSA,
    IRIS_VERSICOLOR,
    IRIS_VIRGINICA
};

struct IRISEntry
{
    ld sepal_length;
    ld sepal_width;
    ld petal_length;
    ld petal_width;
    IRISSpecies species;

    friend std::istream& operator>> (std::istream&, IRISEntry&);
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

    return in;
}

void load_data(vector<IRISEntry>& vec)
{
    fstream file("IRIS.csv", ios::in);
    if (!file.is_open())
        return;

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
}

int main()
{
    Matrix m1({ {1,2,3},{4,5,6} });
    auto m2 = m1.transpose();
    cout << m1 << endl;
}