#include <iostream>
#include "numbers.dat"

using namespace std;
const size_t sqrtSize = 317;
const size_t arraySize = 100000;

int next(int& i, int* q)
{
    i++;
    while (!q[i])
        i++;
}

void create_sieve(int *q)
{
    int i, j;
    q[1] = 0;
    for (i = 2; i < sqrtSize; next(i, q))
    {
        for (j = i*i; j < arraySize; j += i)
            q[j] = 0;
    }
    return ;
}

void filling(int *q)
{
    for (int i = 0; i <= arraySize; i++)
        q[i] = 1;
    return ;
}

void answer(int border[][2], int how, int *q)
{
    int j;
    for (int i = 0; i != how; i++)
    {
        if (border[i][0] > border[i][1] || border[i][0] > arraySize)
        {
            cout << 0 << endl;
            continue;
        }
        int counts = 0;
        for (j = 0; Data[j] < border[i][0]; j++);
        if (Data[j] > border[i][0])
        {
            cout << 0 << endl;
            continue;
        }
        for (; Data[j] <= border[i][1]; j++)
            if (q[Data[j]])
                counts++;
        if (Data[j-1] == border[i][1])
            cout << counts << endl;
        else
            cout << 0 << endl;
    }
    return ;
}

int main(int argc, char* argv[])
{
    if (argc == 1 || !(argc&1))
        return -1;
    int how = (argc-1) >> 1;
    int border[how][2];
    for (int i = 1; i < argc; ++i)
    {
        border[(i-1)>>1][(i-1)&1] = atoi(argv[i]);
    }


    int i, j;
    int *q = new int[arraySize+1];
    filling(q);
    create_sieve(q);
    answer(border, how, q);

    delete []q;
    return 0;
}
