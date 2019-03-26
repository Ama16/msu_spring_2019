#include <iostream>
#include <cmath>

using namespace std;

void space(const string& input, const int len, int& now)
{
    now++;
    while (now < len && input[now] == ' ')
        now++;
    return ;
}

bool is_digit(const char& a)
{
    if (a >= '0' && a <= '9')
        return 1;
    return 0;
}

int get_num(bool &error, const string& input, const int len, int& now)
{
    bool sign = 0;
    while (now < len && input[now] == '-')
    {
        space(input, len, now);
        sign = !sign;
    }
    if (now >= len || !is_digit(input[now]))
    {
        error = 0;
        return -1;
    }
    int num = 0;
    while (now < len && is_digit(input[now]))
    {
        num = num*10 + input[now] - '0';
        space(input, len, now);
    }
    return sign ? -num : num;
}

int mul(bool &error, const string& input, const int len, int& now)
{
    int a = get_num(error, input, len, now);
    while (now < len && (input[now] == '*' || input[now] == '/'))
    {
        char token = input[now];
        space(input, len, now);
        if (token == '*')
            a *= get_num(error, input, len, now);
        else
        {
            int b = get_num(error, input, len, now);
            if (b)
                a /= b;
            else
            {
                error = 0;
                return -1;
            }
        }
    }
    return a;
}

int viras(bool &error, const string& input, const int len)
{
    int now = 0;
    while (now < len && input[now] == ' ')
        now++;
    int a = mul(error, input, len, now);
    while (now < len && error)
    {
        char token = input[now];
        space(input, len, now);
        if (token == '+')
            a += mul(error, input, len, now);
        else
        {
            if (token == '-')
                a -= mul(error, input, len, now);
            else
            {
                error = 0;
                return -1;
            }
        }
    }
    return a;
}

int main(int argc, char* argv[])
{
    string input;
    if (argc > 2)
    {
        cout << "error";
        return 1;
    }
    for (int j = 1; j < argc; ++j)
    {
        input += argv[j];
    }

    bool error = true;
    int a = viras(error, input, input.length());

    if (error)
        cout << a;
    else
    {
        cout << "error";
        return 1;
    }
    return 0;
}
