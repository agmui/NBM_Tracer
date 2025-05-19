//
// Created by agmui on 5/16/25.
//

#include "RenderPixel.h"


Result &RenderPixel::doTask() {
    t.init();
    printf("rendering pixel: %d %d\n", msg.x,msg.y);
    auto p = t.renderPixel(msg.x, msg.y);
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
    msg.x = serializedData.at(1);
    msg.y = serializedData.at(2);
}
