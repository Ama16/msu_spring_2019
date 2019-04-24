#include <iostream>
#include <atomic>
#include <thread>
using namespace std;
atomic<bool> flag(0);
const int ping_pong = 500000;

void ping()
{
    int i = 0;
    while (i < ping_pong)
    {
        if (!flag)
        {
            cout << "ping\n";
            i++;
            flag = true;
        }
    }
}

void pong()
{
    int i = 0;
    while (i < ping_pong)
    {
        if (flag)
        {
            cout << "pong\n";
            i++;
            flag = false;
        }
    }
}


int main()
{
    thread t1(ping);
    thread t2(pong);
    t1.join();
    t2.join();
    return 0;
}
