#include <iostream>
#include <cmath>
#include "numbers.dat"

using namespace std;
const size_t arraySize = 100000;

bool is_prime(int a)
{
    if (a == 2)
        return 1;
    if (a%2 == 0 || a < 2)
        return 0;
    int sqrt_a = sqrt(a);
    for (int i = 3; i <= sqrt_a; i += 2)
        if (a%i == 0)
            return 0;
    return 1;
}

void print_answer(int a, int b)
{
    if (a > b || b > arraySize)
    {
        cout << 0 << endl;
        return;
    }
    int i;
    for (i = 0; Data[i] < a; i++);
    if (Data[i] != a)
    {
        cout << 0 << endl;
        return;
    }
    int c = 0;
    for (; Data[i] <= b; i++)
    {
        c += is_prime(Data[i]);
    }
    if (Data[i-1] != b)
    {
        cout << 0 << endl;
        return;
    }
    cout << c << endl;
    return ;
}

int main(int argc, char* argv[])
{
    if (argc == 1 || !(argc&1))
        return -1;
    int how = (argc-1) >> 1;
    for (int j = 1; j <= how; ++j)
    {
        int a = atoi(argv[2*(j-1)+1]);
        int b = atoi(argv[2*j]);
        print_answer(a, b);
    }
    return 0;
}
