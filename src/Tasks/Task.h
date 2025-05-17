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

enum TaskTypes {
    MineBitCoin_,
    RenderPixel_
};

class Task {

public:
    Task() = default;

    virtual ~Task() = default;

    virtual size_t size() = 0; //TODO: maybe remove

    virtual Result& doTask() = 0;

    virtual std::vector<uint8_t> serialize() = 0;

    int getId() { return id; }

    void setId(int newID) { id = newID; }

    virtual unique_ptr<Result> getResult() = 0;

    virtual void fillResults(vector<uint8_t>& serializedData) = 0; //TODO: should just call .getResults().fill() directly on results



    virtual TaskTypes getType() = 0;

private:
    // Note: has to be public
    int id;
};


#endif //NBM_TRACER_TASK_H
