//
// Created by fufeng on 2024/1/29.
//
#pragma once
#ifndef THINKING_IN_CPP_OBSERVERABLE_H
#define THINKING_IN_CPP_OBSERVERABLE_H

#include <iostream>
#include <string>
#include <list>
#include "Observer.h"
using namespace std;

class Observer;

class Observerable {
public:
    Observerable();
    virtual ~Observerable();

    void Attach(Observer* ob);
    void Detach(Observer* ob);

    int GetObserverCount() {
        return _observers.size();
    }

    void DetachAll() {
        _observers.clear();
    }

    virtual void GetSomeData(string str) {
        SetChange(str);
    }

protected:
    void SetChange(string data);

private:
    void Notify(void* arg);

private:
    bool _isChanged;
    list<Observer*> _observers;
};


#endif //THINKING_IN_CPP_OBSERVERABLE_H
