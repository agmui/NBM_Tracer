//
// Created by agmui on 5/13/25.
//

#ifndef NBM_TRACER_TASK_H
#define NBM_TRACER_TASK_H


#include <cstddef>
#include <cstdint>
#include <vector>
#include <bits/stdc++.h>

#include "Results/Result.h"

using namespace std;


enum TaskTypes {
    MineBitCoin_
};

class Task {

public:
    Task() = default;

    virtual ~Task() = default;

    virtual size_t size() = 0;

    virtual void doTask(unique_ptr<Result> const &result) = 0;

    virtual std::vector<uint8_t> serialize() = 0;

    int getId() { return id; }

    void setId(int newID) { id = newID; }

    TaskTypes type; //TODO: find a better place for this
    ResultTypes resultType;
private:
    // Note: has to be public
    int id;
};


#endif //NBM_TRACER_TASK_H
