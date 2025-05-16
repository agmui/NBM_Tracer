#ifndef NBM_TRACER_TASKFACTORY_H
#define NBM_TRACER_TASKFACTORY_H

#include "Task.h"
#include "MineBitCoin.h"

namespace TaskFactory{
    vector<uint8_t> serialize(Task &task);
    unique_ptr<Task> deserialize(vector<uint8_t> &serializedTask);

}


#endif //NBM_TRACER_TASKFACTORY_H
