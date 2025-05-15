#ifndef NBM_TRACER_MINE_BIT_COIN_H
#define NBM_TRACER_MINE_BIT_COIN_H

#include "Task.h"
#include "Results/BitCoinResult.h"

class MineBitCoin : public Task {
public:
    MineBitCoin(int id, int num1, int num2) : id(id), num1(num1), num2(num2), result(0,0) {}
    ~MineBitCoin() {}

    size_t size() override;
    Result &doTask() override;

private:
    int id;
    int num1;
    int num2;
    BitCoinResult result;
};

#endif