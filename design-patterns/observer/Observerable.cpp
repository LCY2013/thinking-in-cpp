//
// Created by fufeng on 2024/1/29.
//

#include "Observerable.h"

Observerable::Observerable() : _isChanged(false) {
}

Observerable::~Observerable() {
}

void Observerable::Attach(Observer *ob) {
    if (ob == NULL) {
        return;
    }
    auto it = _observers.begin();
    for (; it != _observers.end(); it++) {
        if (*it == ob) {
            return;
        }
    }
    _observers.push_back(ob);
}

void Observerable::Detach(Observer *ob) {
    if (ob == NULL || _observers.empty()) {
        return;
    }
    _observers.remove(ob);
}

void Observerable::SetChange(string data) {
    _isChanged = true;
    Notify(((void*)data.c_str()));
}

void Observerable::Notify(void *arg) {
    if (!_isChanged) {
        return;
    }
    _isChanged = false;

    auto it = _observers.begin();
    for (; it != _observers.begin(); ++it) {
        (*it)->Update(arg);
    }

}
