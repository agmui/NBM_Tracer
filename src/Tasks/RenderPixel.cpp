//
// Created by agmui on 5/16/25.
//

#include "RenderPixel.h"


Result &RenderPixel::doTask() {
    t.init();
    printf("rendering pixel: %d %d\n", x,y);
    auto p = t.renderPixel(x, y);
    printf("finished rendering: final pixel value: %f %f %f\n", p.r, p.g, p.b);
    renderPixelResult.p = p;
//    renderPixelResult.id = id;
    return renderPixelResult;
}

vector<uint8_t> RenderPixel::serialize() {
    return {static_cast<uint8_t>(getType()),static_cast<unsigned char>(x),static_cast<unsigned char>(y)}; // TODO:
}


TaskTypes RenderPixel::getType() {
    return RenderPixel_;
}
