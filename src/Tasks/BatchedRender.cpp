//
// Created by agmui on 5/19/25.
//

#include "BatchedRender.h"


Result &BatchedRender::doTask() {
    if (!randBefore) {
        randBefore = true;
        tracer.init();
    }
    printf("rendering block: %d %d\n", msg.x, msg.y);
    Buffer<pixel> output(msg.width, msg.height);
    for (int y = 0; y < msg.height; ++y) {
        for (int x = 0; x < msg.width; ++x) {
            output.at(x, y) = tracer.renderPixel(msg.x+x, msg.y+y);
        }
    }
    batchedRenderResult.output = output;
    printf("output: %f %f %f\n",
           batchedRenderResult.output.at(0,0).r,
           batchedRenderResult.output.at(0,0).g,
           batchedRenderResult.output.at(0,0).b);
    printf("finished rendering:\n");
    return batchedRenderResult;
}

vector<uint8_t> BatchedRender::serialize() {
    vector<uint8_t> v;
    v.resize(msgSize());
    memcpy(v.data(), &msg, msgSize());
    return v;
}

void BatchedRender::deserialize(vector<uint8_t> serializedData) {
    memcpy(&msg, serializedData.data(), msgSize());
}

unique_ptr<Result> BatchedRender::getResult() {
    batchedRenderResult.x = msg.x;
    batchedRenderResult.y = msg.y;
    return make_unique<BatchedRenderResult>(batchedRenderResult);
}

void BatchedRender::fillResults(vector<uint8_t> &serializedData) {
    return batchedRenderResult.fill(serializedData);
}
