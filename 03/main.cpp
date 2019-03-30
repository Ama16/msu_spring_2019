#include <iostream>

using namespace std;

class Matrix
{
public:
    class array_
    {
    public:
        int* Array;
        int cols;
        int& operator[](int index)
        {
            if (index >= cols)
                throw std::out_of_range("");
            return Array[index];
        }
        const int& operator[](int index) const
        {
            if (index >= cols)
                throw std::out_of_range("");
            return Array[index];
        }
        array_& operator*=(int num)
        {
            for (int i = 0; i != cols; i++)
            {
                Array[i] *= num;
            }
            return *this;
        }


        bool operator==(const array_& other) const
        {
            for (int i = 0; i != cols; i++)
                if (Array[i] != other.Array[i])
                    return false;
            return true;
        }

        bool operator!=(const array_& other) const
        {
            return !(*this == other);
        }
    };



private:
    int rows;
    int cols;
    array_* tab;
public:
    Matrix(int row, int col)
    {
        rows = row;
        cols = col;
        tab = new array_[row];
        for (int i = 0; i != row; i++)
        {
            tab[i].Array = new int[col];
            tab[i].cols = cols;
        }
    }
    ~Matrix()
    {
        for (int i = 0; i != rows; i++)
            delete[] tab[i].Array;
        delete[] tab;
    }
    int getRows() const
    {
        return rows;
    }
    int getColumns() const
    {
        return cols;
    }
    Matrix& operator*=(int num)
    {
        for (int i = 0; i != rows; i++)
        {
            tab[i]*=num;
        }
        return *this;
    }


    array_& operator[](int index)
    {
        if (index >= rows)
            throw std::out_of_range("");
        return tab[index];
    }
    const array_& operator[](int index) const
    {
        if (index >= rows)
            throw std::out_of_range("");
        return tab[index];
    }

    bool operator==(const Matrix& other) const
    {
        if ((rows != other.rows) || (cols != other.cols))
            return false;
        for (int i = 0; i != rows; i++)
            if (tab[i] != other.tab[i])
                return false;
        return true;
    }

    bool operator!=(const Matrix& other) const
    {
        return !(*this == other);
    }

};
