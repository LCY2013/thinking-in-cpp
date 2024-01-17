//
// Created by fufeng on 2024/1/1.
//

#ifndef THINKING_IN_CPP_COMPLEX_H
#define THINKING_IN_CPP_COMPLEX_H

# pragma once

class Complex {
public:
    Complex(); //默认构造函数

    Complex(double real, double imag); // 构造函数

    Complex(const Complex &rhs); //拷贝构造函数

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

    // 前置自增自减
    Complex& operator++(); //前置自增
    Complex& operator--(); //前置自减
    Complex operator++(int); //后置自增
    Complex operator--(int); //后置自减
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
