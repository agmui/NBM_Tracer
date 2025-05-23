#ifndef NBM_TRACER_MINEBITCOIN_H
#define NBM_TRACER_MINEBITCOIN_H

#include <cstring>
#include <cstdio>

#include "Task.h"
#include "Results/BitCoinResult.h"
#include "SerializationHelper.h"

class MineBitCoin : public Task {
public:
    // TODO: move type
    MineBitCoin(int id, int num1, int num2) : msg(id, num1, num2), bitCoinResult(0, 0){
    }

//    MineBitCoin(vector<uint8_t> serializedData) : bitCoinResult(0, 0) {
//        memcpy(&msg, serializedData.data()+1, sizeof(msg_frame)); //TODO: this is jank
//    }

    ~MineBitCoin() {}

    std::vector<uint8_t> serialize() override;

    void deserialize(vector<uint8_t> serializedData) override;

    size_t msgSize() override {
        return sizeof(msg_frame);
    };

    Result & doTask() override;

    unique_ptr<Result> getResult() override { return make_unique<BitCoinResult>(bitCoinResult); }


    void fillResults(vector<uint8_t>& serializedData) override{
        bitCoinResult.fill(serializedData);
    }


    uint8_t getTaskIndex() override { return taskIndex; }
    static uint8_t taskIndex;  //TODO: find better system
private:
    struct __attribute__((__packed__)) msg_frame {
        int id; // TODO: remove because Task already has this
        int num1;
        int num2;
    } msg;
    BitCoinResult bitCoinResult;
};

#endif // NBM_TRACER_MINEBITCOIN_H