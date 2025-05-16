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

    int id;
    ResultTypes resultType = BitCoinResult_;
};

#endif // NBM_TRACER_RESULT_H
