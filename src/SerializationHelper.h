#ifndef NBM_TRACER_SERIALIZATION_HELPER_H
#define NBM_TRACER_SERIALIZATION_HELPER_H

#include "Tasks/Task.h"

namespace SerializationHelper{
//    TaskTypes readTaskType(uint8_t *ptr);

    void appendInt(std::vector<uint8_t> &data, int num);
    int readInt(uint8_t *ptr);
}

#endif