#ifndef NBM_TRACER_MINEBITCOIN_H
#define NBM_TRACER_MINEBITCOIN_H

#include "Task.h"
#include "Results/BitCoinResult.h"
#include "SerializationHelper.h"

class MineBitCoin : public Task
{
public:
    // TODO: move type
    MineBitCoin(int id, int num1, int num2) : id(id), num1(num1), num2(num2)
    {
        type = MineBitCoin_;
        resultType = ResultTypes::BitCoinResult_;
    }

    MineBitCoin(vector<uint8_t> serializedData)
    {
        type = MineBitCoin_;
        resultType = SerializationHelper::readResultType(serializedData.data() + 1);
        id = SerializationHelper::readInt(serializedData.data() + 2);
        num1 = SerializationHelper::readInt(serializedData.data() + 6);
        num2 = SerializationHelper::readInt(serializedData.data() + 10);
    }

    ~MineBitCoin() {}

    std::vector<uint8_t> serialize() override;

    size_t size() override
    {
        return sizeof(*this);
    };

    void doTask(unique_ptr<Result> const &result) override;

private:
    int id; // TODO: remove because Task already has this
    int num1;
    int num2;
};

#endif // NBM_TRACER_MINEBITCOIN_H