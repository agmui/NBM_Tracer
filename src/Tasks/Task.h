//
// Created by agmui on 5/13/25.
//

#ifndef NBM_TRACER_TASK_H
#define NBM_TRACER_TASK_H


#include <cstddef>

#include "Results/Result.h"

class Task {

public:
    Task() {}
    ~Task() {}

    virtual size_t size() = 0;
    virtual Result &doTask() = 0;

    int getId() { return id; }
    void setId(int newID) { id = newID; }

private:
    int id;
};


#endif //NBM_TRACER_TASK_H
