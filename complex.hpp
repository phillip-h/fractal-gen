#ifndef __PML_COMPLEX___
#define __PML_COMPLEX___

#include <cmath>
#include <string>

const std::string I_SYMBOL = "\u03B9";

class complex
{
public:
    double a;
    double b;

    complex(double a = 0, double b = 0)
    {
        this->a = a;
        this->b = b;
    }

    complex(const complex& other)
    {
        this->a = other.a;
        this->b = other.b;
    }

    double abs()
    {
        return sqrt(a * a + b * b);
    }

    std::string to_str()
    {
        return std::to_string(a) + " + " + std::to_string(b) + I_SYMBOL;
    }

    complex scale(const double scale)
    {
        a *= scale;
        b *= scale;
        return *this;
    }

    complex conjugate()
    {
        return complex(a, -b);
    }

    complex& operator=(const complex& rhs)
    {
        if (this == &rhs)
            return *this;
        a = rhs.a;
        b = rhs.b;
        return *this;
    }

    complex& operator+=(const complex& rhs)
    {
        a += rhs.a;
        b += rhs.b;
        return *this;
    }

    const complex operator+(const complex& rhs) const
    {
        complex other = *this;
        other += rhs;
        return other;
    }

    complex& operator-=(const complex& rhs)
    {
        a -= rhs.a;
        b -= rhs.b;
        return *this;
    }

    const complex operator-(const complex& rhs) const
    {
        complex other = *this;
        other -= rhs;
        return other;
    }

    complex& operator*=(const complex& rhs)
    {
        double tmp_a = (a * rhs.a) - (b * rhs.b);
        double tmp_b = (b * rhs.a) + (a * rhs.b);
        a = tmp_a;
        b = tmp_b;
        return *this;
    }

    const complex operator*(const complex& rhs) const
    {
        complex other = *this;
        other *= rhs;
        return other;
    }

    bool operator==(const complex& rhs)
    {
        return a == rhs.a && b == rhs.b;
    }

    bool operator!=(const complex& rhs)
    {
        return !(*this == rhs);
    }

};

#endif
