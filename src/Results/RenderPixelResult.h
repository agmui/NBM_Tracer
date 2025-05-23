//
// Created by agmui on 5/16/25.
//

#ifndef NBM_TRACER_RENDERPIXELRESULT_H
#define NBM_TRACER_RENDERPIXELRESULT_H

#include <cstring>
#include <stdio.h>

#include "Result.h"
#include "TracerImplementations/TracerInterface.h"
#include "../TracerImplementations/TracerInterface.h"

class RenderPixelResult : public Result {
public:
    RenderPixelResult(int id, pixel p): p(p){
        this->id = id;
    }
    ~RenderPixelResult() = default;

    size_t size() override
    {
        return sizeof(pixel);
    }


    vector<uint8_t> serialize() override;

    void fill(vector<uint8_t> serializedData) override;

    void printResult() override{
        printf("pixel: %f %f %f\n", p.r, p.g, p.b);
    }

    int x;
    int y;
    pixel p;//TODO: make private
private:
};


#endif //NBM_TRACER_RENDERPIXELRESULT_H
