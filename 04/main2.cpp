#include <iostream>
#include <sstream>
using namespace std;

enum class Error
{
    NoError,
    CorruptedArchive
};


class Serializer
{
    static constexpr char Separator = ' ';
public:
    explicit Serializer(std::ostream& out)
        : out_(out)
    {
    }

    template <class T>
    Error save(T& object)
    {
        return object.serialize(*this);
    }

    Error process(bool val)
    {
        if (val)
            out_ << "true" << Separator;
        else
            out_ << "false" << Separator;
        return Error::NoError;
    }

    Error process(uint64_t&& val)
    {
        out_ << val << Separator;
        return Error::NoError;
    }

    template <class... ArgsT>
    Error process(bool val, ArgsT... args)
    {
        if (val)
            out_ << "true" << Separator;
        else
            out_ << "false" << Separator;
        return process(args...);
    }

    template <class... ArgsT>
    Error process(uint64_t val, ArgsT... args)
    {
        out_ << val << Separator;
        return process(args...);
    }

    template <class... ArgsT>
    Error operator()(ArgsT... args)
    {
        return process(args...);
    }
private:
    std::ostream& out_;
};




class Deserializer
{
public:
    explicit Deserializer(std::istream& in)
        : in_(in)
    {
    }


    bool is_digit(string value)
    {
        for (int i = 0; i < value.length(); i++)
        {
            if (value[i] < '0' || value[i] > '9')
                return false;
        }
        return true;
    }

    uint64_t convert(string value)
    {
        uint64_t a = 0;
        for (int i = 0; i < value.length(); i++)
        {
            a = a*10 + value[i] - '0';
        }
        return a;
    }

    template <class T>
    Error load(T& object)
    {
        if (load(object.a) == Error::NoError && load(object.b) == Error::NoError && load(object.c) == Error::NoError)
            return Error::NoError;
        return Error::CorruptedArchive;
    }


    Error load(uint64_t& value)
    {
        std::string text;
        in_ >> text;

        if (is_digit(text))
            value = convert(text);
        else
            return Error::CorruptedArchive;

        return Error::NoError;
    }

    Error load(bool& value)
    {
        std::string text;
        in_ >> text;

        if (text == "true")
            value = true;
        else if (text == "false")
            value = false;
        else
            return Error::CorruptedArchive;

        return Error::NoError;
    }
private:
    std::istream& in_;
};
