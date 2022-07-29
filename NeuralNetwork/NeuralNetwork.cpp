#include <iostream>
#include <vector>

using namespace std;

template <class T>
class Matrix
{
private:
    const int rows, columns;
    vector<vector<T>> mat;
public:
    Matrix(int rows, int columns) : rows{ rows }, columns{ columns }
    {
        mat.resize(rows, vector<T>(columns, 0));
    }


};

int main()
{
    std::cout << "Hello World!\n";
}