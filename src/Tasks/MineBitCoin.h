#ifndef NBM_TRACER_MINEBITCOIN_H
#define NBM_TRACER_MINEBITCOIN_H

#include "Task.h"
#include "Results/BitCoinResult.h"

class MineBitCoin : public Task {
public:
    //TODO: move type
    MineBitCoin(int id, int num1, int num2) : id(id), num1(num1), num2(num2), result(0, 0) {type=MineBitCoin_;}
    MineBitCoin(vector<uint8_t> serializedData): result(0, 0){
        MineBitCoin* m = (MineBitCoin*)&serializedData[0];
        id = m->id; //TODO: use move or something
        num1 = m->num1;
        num2 = m->num2;
    }

    ~MineBitCoin() {}


    std::vector<uint8_t> serialize() override {
        return vector<uint8_t>(1, 2); //TODO:
    }

    size_t size() override {
        return sizeof(*this);
    };

    Result &doTask() override;

    Result *getResult() override {
        return &result;
    }

private:
    int id; //TODO: remove because Task already has this
    int num1;
    int num2;
    BitCoinResult result;
};

#endif //NBM_TRACER_MINEBITCOIN_H