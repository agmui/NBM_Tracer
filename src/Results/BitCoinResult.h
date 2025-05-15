#ifndef NBM_TRACER_BIT_COIN_RESULT_H
#define NBM_TRACER_BIT_COIN_RESULT_H

#include "Result.h"

class BitCoinResult : public Result {
public:
    BitCoinResult(int id, int sum) : id(id), sum(sum) {}
    ~BitCoinResult() {}

    size_t size() override {
        return sizeof(*this);
    }

private:
    int id;
    int sum;
};

#endif