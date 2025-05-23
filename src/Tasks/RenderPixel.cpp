//
// Created by agmui on 5/16/25.
//

#include "RenderPixel.h"

uint8_t RenderPixel::taskIndex;  //TODO: find better system
Result &RenderPixel::doTask() {
    if(!randBefore){
        randBefore = true;
        tracer->init();
    }
    printf("rendering pixel: %d %d\n", msg.x,msg.y);
    auto p = tracer->renderPixel(msg.x, msg.y);
    printf("finished rendering: final pixel value: %f %f %f\n", p.r, p.g, p.b);
    renderPixelResult.p = p;
//    renderPixelResult.id = id;
    return renderPixelResult;
}

vector<uint8_t> RenderPixel::serialize() {
    vector<uint8_t> v;
    v.resize(msgSize());
    memcpy(v.data(), &msg, msgSize());
    return v;
}



void RenderPixel::deserialize(vector<uint8_t> serializedData) {
    memcpy(&msg, serializedData.data(), msgSize());
}
