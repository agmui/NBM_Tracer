//
// Created by agmui on 5/13/25.
//

#ifndef NBM_TRACER_TASK_H
#define NBM_TRACER_TASK_H


#include <cstddef>
#include <cstdint>
#include <vector>
#include <memory>

#include "Results/Result.h"

using namespace std;

class Task {

public:
    Task() = default;

    virtual ~Task() = default;

    virtual size_t msgSize() = 0; //TODO: maybe remove

    virtual Result &doTask() = 0;

    virtual std::vector<uint8_t> serialize() = 0;

    virtual void deserialize(vector<uint8_t> serializedData) = 0;

    int getId() { return id; }

    uint8_t getTaskIndex() { return taskIndex; }

    static void setTaskIndex(uint8_t index) { taskIndex = index; }

    virtual unique_ptr<Result> getResult() = 0;

    virtual void
    fillResults(vector<uint8_t> &serializedData) = 0; //TODO: should just call .getResults().fill() directly on results


private:
    // Note: has to be public
    int id;
    static uint8_t taskIndex;  //TODO: find better system
};


#endif //NBM_TRACER_TASK_H
