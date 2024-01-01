//
// Created by fufeng on 2024/1/1.
//

#include <iostream>
#include "Complex.h"

using namespace std;

Complex::Complex() {
    cout << "Complex()无参构造函数" << endl;
    _real = 0;
    _imag = 0;
}

Complex::Complex(double real, double imag) {
    this->_imag = imag;
    this->_real = real;
}

Complex::~Complex() {
    cout << "~Complex()析构函数" << endl;
}

double Complex::GetReal() const {
    return _real;
}

void Complex::SetReal(double real) {
    _real = real;
}

double Complex::GetImag() const {
    return _imag;
}

void Complex::SetImag(double imag) {
    _imag = imag;
}

Complex Complex::operator+(const Complex &rhs) {
    return Complex(_real + rhs.GetReal(), _imag + rhs.GetImag());
}

Complex Complex::operator-(const Complex &rhs) {
    return Complex(_real - rhs.GetReal(), _imag - rhs.GetImag());
}

Complex Complex::operator*(const Complex &rhs) {
    return Complex(_real * rhs.GetReal() - _imag * rhs.GetImag(),
                   _real * rhs.GetImag() + _imag * rhs.GetReal());
}

Complex Complex::operator/(const Complex &rhs) {
    double denominator = rhs.GetReal() * rhs.GetReal() + rhs.GetImag() * rhs.GetImag();
    return Complex((_real * rhs.GetReal() + _imag * rhs.GetImag()) / denominator,
                   (_imag * rhs.GetReal() - _real * rhs.GetImag()) / denominator);
}

Complex& Complex::operator=(const Complex &rhs) {
    if (this!= &rhs) {
        _real = rhs.GetReal();
        _imag = rhs.GetImag();
    }
    return *this;
}

ostream &operator<<(ostream &os, const Complex &rhs) {
    os << rhs.GetReal() << " + " << rhs.GetImag() << "i";
    return os;
}
istream &operator>>(istream &is, Complex &rhs) {
    double real, imag;
    is >> real >> imag;
    rhs = Complex(real, imag);
    return is;
}

bool operator==(const Complex &lhs, const Complex &rhs) {
    return lhs.GetReal() == rhs.GetReal() && lhs.GetImag() == rhs.GetImag();
}

bool operator!=(const Complex &lhs, const Complex &rhs) {
    return!(lhs == rhs);
}

bool operator<(const Complex &lhs, const Complex &rhs) {
    return lhs.GetReal() < rhs.GetReal() || (lhs.GetReal() == rhs.GetReal() && lhs.GetImag() < rhs.GetImag());
}

bool operator>(const Complex &lhs, const Complex &rhs) {
    return rhs < lhs;
}

bool operator<=(const Complex &lhs, const Complex &rhs) {
    return!(lhs > rhs);
}

bool operator>=(const Complex &lhs, const Complex &rhs) {
    return!(lhs < rhs);
}

Complex operator+(double real, const Complex &rhs) {
    return Complex(real + rhs.GetReal(), rhs.GetImag());
}

Complex operator-(double real, const Complex &rhs) {
    return Complex(real - rhs.GetReal(), -rhs.GetImag());
}

Complex operator*(double real, const Complex &rhs) {
    return Complex(real * rhs.GetReal(), real * rhs.GetImag());
}


