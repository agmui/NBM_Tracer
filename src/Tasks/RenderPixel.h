//
// Created by agmui on 5/16/25.
//

#ifndef NBM_TRACER_RENDERPIXEL_H
#define NBM_TRACER_RENDERPIXEL_H


#include <utility>

#include "Task.h"
#include "TracerImplementations/TracerInterface.h"
#include "TracerImplementations/MuianRayTracer.h"
#include "Results/RenderPixelResult.h"

class RenderPixel : public Task {
public:
    RenderPixel(int x, int y, shared_ptr<TracerInterface> tracer) : tracer(std::move(tracer)), msg(x,y), renderPixelResult(0, {-1,-1,-1}) {}
//    RenderPixel(int x, int y) : x(x), y(y), renderPixelResult(0, {-1,-1,-1}) {}
//    RenderPixel(vector<uint8_t> serializedData) : renderPixelResult(0, {-1,-1,-1}) {
//        x = serializedData.at(1);
//        y = serializedData.at(2);
//    }

    size_t msgSize() override {
        return sizeof(msg_frame);
    };

    Result &doTask() override;

    vector<uint8_t> serialize() override;

    unique_ptr<Result> getResult() override {
        renderPixelResult.x = msg.x;//TODO: move this to a more appropriate place
        renderPixelResult.y = msg.y;
        return make_unique<RenderPixelResult>(renderPixelResult);
    }

    void fillResults(vector<uint8_t>& serializedData) override{
        renderPixelResult.fill(serializedData);
    }

    void deserialize(vector<uint8_t> serializedData) override;

    uint8_t getTaskIndex() override { return taskIndex; }
    static uint8_t taskIndex;  //TODO: find better system
private:
    shared_ptr<TracerInterface> tracer;
    struct msg_frame{
        int x;
        int y;
    }msg;
    bool randBefore = false;
    RenderPixelResult renderPixelResult;

};


#endif //NBM_TRACER_RENDERPIXEL_H
