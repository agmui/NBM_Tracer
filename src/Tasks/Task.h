//
// Created by agmui on 5/13/25.
//

#ifndef NBM_TRACER_TASK_H
#define NBM_TRACER_TASK_H


#include <cstddef>

class Task {

public:
    Task() {}
    ~Task() {}

    virtual size_t size() = 0;

    int get_id() { return id; }
    void set_id(int newID) { id = newID; }

private:
    int id;
};


#endif //NBM_TRACER_TASK_H
