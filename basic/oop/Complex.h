//
// Created by fufeng on 2024/1/1.
//

#ifndef THINKING_IN_CPP_COMPLEX_H
#define THINKING_IN_CPP_COMPLEX_H

# pragma once

class Complex {
public:
    Complex();

    Complex(double real, double imag);

    virtual ~Complex();

    double GetReal() const;

    void SetReal(double real);

    double GetImag() const;

    void SetImag(double imag);

    //运算符重载
    Complex operator+(const Complex &rhs);
    Complex operator-(const Complex &rhs);
    Complex operator*(const Complex &rhs);
    Complex operator/(const Complex &rhs);
    Complex& operator=(const Complex &rhs);
    Complex operator==(const Complex &rhs);
    Complex operator!=(const Complex &rhs);
    Complex operator>(const Complex &rhs);
    Complex operator<(const Complex &rhs);
    Complex operator>=(const Complex &rhs);
    Complex operator<=(const Complex &rhs);
    Complex operator!();
    Complex operator~();
    Complex operator++();
    Complex operator--();
    Complex operator++(int);
    Complex operator--(int);
    Complex operator+(double rhs);
    Complex operator-(double rhs);
    Complex operator*(double rhs);
    Complex operator/(double rhs);
    Complex operator=(double rhs);
    Complex operator==(double rhs);
    Complex operator!=(double rhs);
    Complex operator>(double rhs);
    Complex operator<(double rhs);
    Complex operator>=(double rhs);
    Complex operator<=(double rhs);
    Complex operator+(int rhs);
    Complex operator-(int rhs);
    Complex operator*(int rhs);
    Complex operator/(int rhs);

private:
    double _real; // 复数实部
    double _imag; // 复数虚部
};


#endif //THINKING_IN_CPP_COMPLEX_H
