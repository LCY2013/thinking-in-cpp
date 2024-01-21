//
// Created by fufeng on 2024/1/1.
//

#include "Complex.h"

Complex::Complex()
{
    _real = 0.0;
    _image = 0.0;
    //cout << "Complex::Complex()" << endl;
}

Complex::Complex(double r, double i)
{
    _real = r;
    _image = i;
    //cout << "Complex::Complex(double r, double i)" << endl;
}

Complex::Complex(const Complex& c)
{
    _real = c._real;
    _image = c._image;
    //cout << "Complex::Complex(const Complex& c)" << endl;
}

Complex& Complex::operator= (const Complex& c)
{
    if (this != &c)
    {
        _real = c._real;
        _image = c._image;
    }
    return *this;
}

Complex::~Complex()
{
    _real = _image = 0.0;
    //cout << "Complex::~Complex()" << endl;
}

Complex Complex::operator+ (const Complex& c) const
{
    //Complex tmp;
    //tmp._real = _real + x._real;
    //tmp._image = _image + x._image;
    //return tmp;

    return Complex(_real + c._real, _image + c._image);
}


Complex& Complex::operator+= (const Complex& c)
{
    _real += c._real;
    _image += c._image;

    return *this;
}

Complex Complex::operator-(const Complex &c) const
{
    return Complex(_real - c._real, _image - c._image);
}

Complex& Complex::operator-=(const Complex &c)
{
    _real -= c._real;
    _image -= c._image;

    return *this;
}

Complex Complex::operator*(const Complex &c) const
{
    return Complex(_real*c._real - _image*c._image, _real*c._image + _image*c._real);
}

Complex& Complex::operator*=(const Complex &c)
{
    Complex tmp(*this);
    _real = tmp._real*c._real - _image*c._image;
    _image = tmp._real*c._image + tmp._image*c._real;
    return *this;
}

Complex Complex::operator/(const Complex &c) const
{
    double t = c._real*c._real + c._image*c._image;
    return Complex((_real*c._real - _image*(-c._image)) / t, (_real*(-c._image) + _image*c._real) / t);
}

Complex& Complex::operator/=(const Complex &c)
{
    Complex tmp(*this);
    double t = c._real*c._real + c._image*c._image;
    _real = (tmp._real*c._real - tmp._image*(-c._image)) / t;
    _image = (tmp._real*(-c._image) + tmp._image*c._real) / t;
    return *this;
}

bool Complex::operator==(const Complex& c) const
{
    return (_real == c._real) && (_image == c._image);
}

bool Complex::operator!=(const Complex& c) const
{
    return !( (_real == c._real) && (_image == c._image) );
}

bool Complex::operator>(const Complex &c)  const
{
    return (_real > c._real) && (_image > c._image);
}

bool Complex::operator>=(const Complex &c) const
{
    return (_real >= c._real) && (_image >= c._image);
}

bool Complex::operator<(const Complex &c) const
{
    return (_real < c._real) && (_image < c._image);
}

bool Complex::operator<=(const Complex &c) const
{
    return (_real <= c._real) && (_image <= c._image);
}


Complex& Complex::operator++ ()
{
    _real++;
    _image++;
    return *this;
}

Complex Complex::operator++ (int)
{
    //Complex tmp(*this);
    //_real++;
    //_image++;
    //return tmp;
    return Complex(_real++, _image++);
}

Complex& Complex::operator--()
{
    _real--;
    _image--;
    return *this;
}

Complex Complex::operator--(int)
{
    return Complex(_real--, _image--);
}

ostream& operator<<(ostream& os, const Complex &x)
{
    os << "real value is  " << x._real << "  image value is " << x._image;
    return os;
}

istream& operator >> (istream& is, Complex &x)
{
    is >> x._real >> x._image;
    return is;
}


