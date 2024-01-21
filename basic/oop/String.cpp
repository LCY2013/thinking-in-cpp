//
// Created by fufeng on 2024/1/21.
//

#include "String.h"

String::String(const char *str)
{
    if (str == NULL)
    {
        m_data = new char[1];
        if (m_data != NULL)
        {
            *m_data = '\0';
        }
        else
        {
            exit(-1);
        }
    }
    else
    {
        int len = strlen(str);
        m_data = new char[len + 1];
        if (m_data != NULL)
        {
            strcpy(m_data, str);
        }
        else
        {
            exit(-1);
        }
    }
}

String::String(const String &other)
{
    int len = strlen(other.m_data);
    m_data = new char[len + 1];
    if (m_data != NULL)
    {
        strcpy(m_data, other.m_data);
    }
    else
    {
        exit(-1);
    }
}

String::String(String&& other)
{
    if (other.m_data != NULL)
    {
        m_data = other.m_data;
        other.m_data = NULL;
    }
}


String& String::operator= (const String &other)
{
    if (this == &other)
    {
        return *this;
    }
    delete[ ] m_data;
    int len = strlen(other.m_data);
    m_data = new char[len + 1];
    if (m_data != NULL)
    {
        strcpy(m_data, other.m_data);
    }
    else
    {
        exit(-1);
    }

    return *this;
}

String& String::operator=(String&& rhs)noexcept
{
    if(this != &rhs)
    {
        delete[] m_data;
        m_data = rhs.m_data;
        rhs.m_data = NULL;
    }
    return *this;
}

String::~String(void)
{
    if (m_data != NULL)
    {
        delete[] m_data;
    }
}

ostream& operator<<(ostream& os, const String &c)
{
    os << c.m_data;
    return os;
}
