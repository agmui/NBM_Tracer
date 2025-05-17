//
// Created by agmui on 5/16/25.
//

#ifndef NBM_TRACER_RENDERPIXEL_H
#define NBM_TRACER_RENDERPIXEL_H


#include "Task.h"
#include "TracerImplementations/TracerInterface.h"
#include "TracerImplementations/MuianRayTracer.h"
#include "Results/RenderPixelResult.h"

class RenderPixel : public Task {
public:
    RenderPixel(int x, int y) : x(x), y(y), renderPixelResult(0, {-1,-1,-1}) {}
    RenderPixel(vector<uint8_t> serializedData) : renderPixelResult(0, {-1,-1,-1}) {
        x = serializedData.at(1);
        y = serializedData.at(2);
    }

    size_t size() override {
        return sizeof(*this);
    };

    Result &doTask() override;

    vector<uint8_t> serialize() override;

    unique_ptr<Result> getResult() {
        renderPixelResult.x = x;//TODO: move this to a more appropriate place
        renderPixelResult.y = y;
        return make_unique<RenderPixelResult>(renderPixelResult);
    }

    TaskTypes getType() override;
    void fillResults(vector<uint8_t>& serializedData) override{
        renderPixelResult.fill(serializedData);
    }

private:
    MuianRayTracer t;
    int x = 0;//TODO: wrap with struct
    int y = 0;
    RenderPixelResult renderPixelResult;

};


#endif //NBM_TRACER_RENDERPIXEL_H
