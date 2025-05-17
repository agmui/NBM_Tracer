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
    MineBitCoin(int id, int num1, int num2) : msg(id, num1, num2), bitCoinResult(0, 0) {
    }

    MineBitCoin(vector<uint8_t> serializedData) : bitCoinResult(0, 0) {
//        resultType = SerializationHelper::readResultType(serializedData.data() + 1);
//        msg.id = SerializationHelper::readInt(serializedData.data() + 2);
//        msg.num1 = SerializationHelper::readInt(serializedData.data() + 6);
//        msg.num2 = SerializationHelper::readInt(serializedData.data() + 10);
        memcpy(&msg, serializedData.data()+1, sizeof(msg_frame)); //TODO: this is jank
    }

    ~MineBitCoin() {}

    std::vector<uint8_t> serialize() override;

    size_t size() override {
        return sizeof(*this);
    };

    Result & doTask() override;

    unique_ptr<Result> getResult() override { return make_unique<BitCoinResult>(bitCoinResult); }

    TaskTypes getType() override { return TaskTypes::MineBitCoin_; }

    void fillResults(vector<uint8_t>& serializedData) override{
        bitCoinResult.fill(serializedData);
    }

private:
    struct __attribute__((__packed__)) msg_frame {
        int id; // TODO: remove because Task already has this
        int num1;
        int num2;
    } msg;
    BitCoinResult bitCoinResult;
};

#endif // NBM_TRACER_MINEBITCOIN_H