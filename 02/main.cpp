#include <iostream>
#include <cmath>

using namespace std;

void prob(const string& q, const int len, int& i)
{
    i++;
    while (i < len && q[i] == ' ')
        i++;
    return ;
}

bool is_digit(const char& a)
{
    if (a >= '0' && a <= '9')
        return 1;
    return 0;
}

int get_num(bool &er, const string& q, const int len, int& i)
{
    bool sig = 0;
    while (i < len && q[i] == '-')
    {
        prob(q, len, i);
        sig = !sig;
    }
    if (i >= len || !is_digit(q[i]))
    {
        er = 0;
        return -1;
    }
    int a = 0;
    while (i < len && is_digit(q[i]))
    {
        a = a*10 + q[i] - '0';
        prob(q, len, i);
    }
    return sig ? -a : a;
}

int mul(bool &er, const string& q, const int len, int& i)
{
    int a = get_num(er, q, len, i);
    while (i < len && (q[i] == '*' || q[i] == '/'))
    {
        char now = q[i];
        prob(q, len, i);
        if (now == '*')
            a *= get_num(er, q, len, i);
        else
        {
            int b = get_num(er, q, len, i);
            if (b)
                a /= b;
            else
            {
                er = 0;
                return -1;
            }
        }
    }
    return a;
}

int viras(bool &er, const string& q, const int len)
{
    int i = 0;
    while (i < len && q[i] == ' ')
        i++;
    int a = mul(er, q, len, i);
    while (i < len && er)
    {
        char now = q[i];
        prob(q, len, i);
        if (now == '+')
            a += mul(er, q, len, i);
        else
        {
            if (now == '-')
                a -= mul(er, q, len, i);
            else
            {
                er = 0;
                return -1;
            }
        }
    }
    return a;
}

int main(int argc, char* argv[])
{
    string q;
    if (argc > 2)
    {
        cout << "error";
        return 1;
    }
    for (int j = 1; j < argc; ++j)
    {
        q += argv[j];
    }

    bool er = true;
    int a = viras(er, q, q.length());

    if (er)
        cout << a;
    else
    {
        cout << "error";
        return 1;
    }
    return 0;
}
