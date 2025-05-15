//
// Created by agmui on 5/15/25.
//

#ifndef NBM_TRACER_TASKFACTORY_H
#define NBM_TRACER_TASKFACTORY_H

#include "Task.h"
#include "MineBitCoin.h"

namespace TaskFactory{
    static vector<uint8_t> serialize(Task &task);
    static unique_ptr<Task> deserialize(vector<uint8_t> serializedTask);

}


#endif //NBM_TRACER_TASKFACTORY_H
