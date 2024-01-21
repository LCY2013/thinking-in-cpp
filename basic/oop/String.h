//
// Created by fufeng on 2024/1/21.
//

#ifndef THINKING_IN_CPP_STRING_H
#define THINKING_IN_CPP_STRING_H

#pragma once

#include <iostream>

using namespace std;

class String {
public:
    String(const char *str = NULL);

    String(const String &other);

    String(String &&other);

    ~String(void);

    String &operator=(const String &other);

    String &operator=(String &&other) noexcept;

    friend ostream &operator<<(ostream &os, const String &str);

private:
    char *m_data;
};


#endif //THINKING_IN_CPP_STRING_H
