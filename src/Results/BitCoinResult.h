#ifndef NBM_TRACER_BIT_COIN_RESULT_H
#define NBM_TRACER_BIT_COIN_RESULT_H

#include <stdio.h>

#include "SerializationHelper.h"
#include "Result.h"

class BitCoinResult : public Result
{
public:
    BitCoinResult(int id, int sum) : sum(sum) {
        this->id = id;
    }
    void fill(vector<uint8_t> serializedData) override
    {
        id = SerializationHelper::readInt(serializedData.data() + 1);
        sum = SerializationHelper::readInt(serializedData.data() + 5);
    }

    ~BitCoinResult() = default;

    vector<uint8_t> serialize() override;

    size_t size() override
    {
        return sizeof(*this);
    }

    void printResult() override{
        printf("sum: %d\n", sum);
    }

    int sum;

private:
};

#endif