//
// Created by agmui on 5/13/25.
//

#include "MineBitCoin.h"

uint8_t Task::taskIndex; //TODO: this is jank
Result& MineBitCoin::doTask() {
    bitCoinResult.id = msg.id;
    bitCoinResult.sum = msg.num1 + msg.num2;
    return bitCoinResult;
}

std::vector<uint8_t> MineBitCoin::serialize() {
    std::vector<uint8_t> data;

    data.insert(data.begin()+1, (uint8_t *) &msg, (uint8_t *) &msg + sizeof(msg_frame));
    printf("data: %d\n", data.at(0));
    return data;
}

void MineBitCoin::deserialize(vector<uint8_t> serializedData) {
    memcpy(&msg, serializedData.data()+1, sizeof(msg_frame)); //TODO: this is jank
}

