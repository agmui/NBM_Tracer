//
// Created by agmui on 5/13/25.
//

#ifndef NBM_TRACER_RESULT_H
#define NBM_TRACER_RESULT_H

#include <cstddef>
#include <vector>
#include <cstdint>

using namespace std;

enum ResultTypes
{
    BitCoinResult_
};

class Result
{

public:
    virtual size_t size() = 0;

    virtual vector<uint8_t> serialize() = 0;

    virtual void fill(vector<uint8_t> serializedData) = 0;

    virtual void printResult() = 0;
    int id;
};

#endif // NBM_TRACER_RESULT_H
