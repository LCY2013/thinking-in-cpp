//
// Created by fufeng on 2024/1/29.
//

#ifndef THINKING_IN_CPP_OBSERVER_H
#define THINKING_IN_CPP_OBSERVER_H

class Observer {
public:
    Observer() {;}
    virtual ~Observer() {;}

    virtual void Update(void* arg) = 0;
};

#endif //THINKING_IN_CPP_OBSERVER_H
