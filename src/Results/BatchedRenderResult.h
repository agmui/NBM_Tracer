//
// Created by agmui on 5/19/25.
//

#ifndef NBM_TRACER_BATCHEDRENDERRESULT_H
#define NBM_TRACER_BATCHEDRENDERRESULT_H

#include <memory>
#include <muian_raytracer/include/Buffer.h>
#include "TracerImplementations/TracerInterface.h"
#include "Result.h"

class BatchedRenderResult : public Result{
public:
    BatchedRenderResult(int x, int y, int width, int height): x(x), y(y), output( width,height ){}
    size_t size() override;

    vector<uint8_t> serialize() override;

    void fill(vector<uint8_t> serializedData) override;

    void printResult() override;

    int x; //TODO: make private
    int y;
    Buffer<pixel> output;
private:
};


#endif //NBM_TRACER_BATCHEDRENDERRESULT_H
