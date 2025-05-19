//
// Created by agmui on 5/13/25.
//

#include "BitCoinResult.h"

vector<uint8_t> BitCoinResult::serialize()
{
    std::vector<uint8_t> data;

    SerializationHelper::appendInt(data, id);
    SerializationHelper::appendInt(data, sum);

    return data;
}
