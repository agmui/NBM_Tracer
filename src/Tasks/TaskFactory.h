//
// Created by agmui on 5/15/25.
//

#ifndef NBM_TRACER_TASKFACTORY_H
#define NBM_TRACER_TASKFACTORY_H

#include <memory>

#include "Task.h"
#include "MineBitCoin.h"
#include "RenderPixel.h"

namespace TaskFactory{
    vector<uint8_t> serialize(Task &task);
    unique_ptr<Task> deserialize(vector<uint8_t> serializedTask);

}


#endif //NBM_TRACER_TASKFACTORY_H
