//
// Created by agmui on 5/13/25.
//

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