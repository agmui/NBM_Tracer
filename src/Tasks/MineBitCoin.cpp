//
// Created by agmui on 5/13/25.
//

#include "MineBitCoin.h"

void MineBitCoin::doTask(unique_ptr<Result> const &result)
{
    BitCoinResult *bitCoinResult = static_cast<BitCoinResult *>(result.get());
    bitCoinResult->id = id;
    bitCoinResult->sum = num1 + num2;
    bitCoinResult->resultType = BitCoinResult_;
}

std::vector<uint8_t> MineBitCoin::serialize()
{
    std::vector<uint8_t> data;

    data.push_back(static_cast<uint8_t>(TaskTypes::MineBitCoin_));
    data.push_back(static_cast<uint8_t>(ResultTypes::BitCoinResult_));
    SerializationHelper::appendInt(data, id);
    SerializationHelper::appendInt(data, num1);
    SerializationHelper::appendInt(data, num2);

    return data;
}