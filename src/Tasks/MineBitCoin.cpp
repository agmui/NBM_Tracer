//
// Created by agmui on 5/13/25.
//

#include "MineBitCoin.h"

size_t MineBitCoin::size() {
    return sizeof(*this);
}

Result &MineBitCoin::doTask() {
    return result;
}