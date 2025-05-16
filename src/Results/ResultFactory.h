#ifndef NBM_TRACER_RESULTFACTORY_H
#define NBM_TRACER_RESULTFACTORY_H

#include "Result.h"
#include "BitCoinResult.h"

namespace ResultFactory{
    vector<uint8_t> serialize(Result &result);
    unique_ptr<Result> deserialize(vector<uint8_t> &serializedResult);
}


#endif
