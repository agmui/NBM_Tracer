//
// Created by agmui on 5/19/25.
//

#ifndef NBM_TRACER_BATCHEDRENDER_H
#define NBM_TRACER_BATCHEDRENDER_H

#include "Task.h"
#include "TracerImplementations/MuianRayTracer.h"
#include "Results/BatchedRenderResult.h"

/**
 * Renders a block of pixels at once starting upper let corner at x,y
 * and width, height
 */
class BatchedRender : public Task {
public:
    BatchedRender(int x, int y, int width, int height, shared_ptr<TracerInterface> tracer) : tracer(std::move(tracer)), msg(x, y, width, height), batchedRenderResult(x,y,width,height){}

    size_t msgSize() override{ return sizeof(msg_frame); }

    Result &doTask() override;

    vector<uint8_t> serialize() override;

    void deserialize(vector<uint8_t> serializedData) override;

    unique_ptr<Result> getResult() override;

    void fillResults(vector<uint8_t> &serializedData) override;

private:
    shared_ptr<TracerInterface> tracer;
    struct msg_frame {
        int x;
        int y;
        int width;
        int height;
    } msg;
    bool ranBefore = false;
    BatchedRenderResult batchedRenderResult;

    void multiThreadTrace(Buffer<pixel> &output, int i);
};


#endif //NBM_TRACER_BATCHEDRENDER_H
