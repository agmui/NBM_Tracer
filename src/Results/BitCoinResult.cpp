//
// Created by agmui on 5/13/25.
//

#include "BitCoinResult.h"

vector<uint8_t> BitCoinResult::serialize()
{
    std::vector<uint8_t> data;

    data.push_back(static_cast<uint8_t>(ResultTypes::BitCoinResult_));
    SerializationHelper::appendInt(data, id);
    SerializationHelper::appendInt(data, sum);

    return data;
}
