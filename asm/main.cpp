#include <iostream>

using namespace std;

int Binom(int n, int k)
{
    if (k > n) return 0;
    if( k==0 || n==k)      return 1;
    else return Binom(n-1,k)+Binom(n-1,k-1);
}

int main()
{
    int n, k;
    cin >> n >> k;
    int how = 0, all = 0, nowz = 0, need, now;
    int nn = n;
    while (nn)
    {
        how++;
        nn >>= 1;
    }




    int i = how - 2;
    while (i >= 0)
    {
        now = (n >> i) & 1;
        if (now == 1)
        {
            need = k - nowz - 1;
            all += Binom(i, need);
        }
        else
        {
            nowz++;
        }
        i--;
    }



    for (i = how-2; i >= k; i--)
        all += Binom(i, k);


    now = 0;
    while (n)
    {
        need = n&1;
        if (need == 0)
            now++;
        n >>= 1;
    }

    if (now == k)
        all++;

    cout << all;
}
