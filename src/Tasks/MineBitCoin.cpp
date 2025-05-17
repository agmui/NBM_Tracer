//
// Created by agmui on 5/13/25.
//

#include "MineBitCoin.h"

Result& MineBitCoin::doTask() {
    bitCoinResult.id = msg.id;
    bitCoinResult.sum = msg.num1 + msg.num2;
    return bitCoinResult;
}

std::vector<uint8_t> MineBitCoin::serialize() {
    std::vector<uint8_t> data;
    data.reserve(sizeof(msg_frame)+1); //TODO: optimise

    data.push_back(static_cast<uint8_t>(getType()));
//    data.push_back(static_cast<uint8_t>(ResultTypes::BitCoinResult_));
//    SerializationHelper::appendInt(data, id);
//    SerializationHelper::appendInt(data, num1);
//    SerializationHelper::appendInt(data, num2);

    data.insert(data.begin()+1, (uint8_t *) &msg, (uint8_t *) &msg + sizeof(msg_frame));
    printf("data: %d\n", data.at(0));
    return data;
}

