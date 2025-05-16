#ifndef NBM_TRACER_BIT_COIN_RESULT_H
#define NBM_TRACER_BIT_COIN_RESULT_H

#include "SerializationHelper.h"
#include "Result.h"

class BitCoinResult : public Result
{
public:
    BitCoinResult(int id, int sum) : sum(sum) {
        this->id = id;
    }
    BitCoinResult(vector<uint8_t> serializedData)
    {
        resultType = BitCoinResult_;
        id = SerializationHelper::readInt(serializedData.data() + 1);
        sum = SerializationHelper::readInt(serializedData.data() + 5);
    }

    ~BitCoinResult() {}

    vector<uint8_t> serialize() override;

    size_t size() override
    {
        return sizeof(*this);
    }

    int sum;

private:
};

#endif