//
// Created by agmui on 5/19/25.
//

#include "BatchedRender.h"

uint8_t BatchedRender::taskIndex;  //TODO: find better system
void BatchedRender::multiThreadTrace(Buffer<pixel> &output, int i) {
    for (int y = msg.height*i/NUM_THREADS; y < msg.height*(i+1)/NUM_THREADS; ++y) {
        for (int x = 0; x < msg.width; ++x) {
            output.at(x, y) = tracer->renderPixel(msg.x+x, msg.y+y);
        }
    }
}

Result &BatchedRender::doTask() {
//    if (!ranBefore) { //TODO: make better system
//        ranBefore = true;
//        tracer->init();
//    }
    printf("rendering block: %d %d\n", msg.x, msg.y);

    Buffer<pixel> output(msg.width, msg.height);
    // multi threaded render
    thread threadArr[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; ++i) {
        threadArr[i] = thread(&BatchedRender::multiThreadTrace, this, ref(output), i);
    }
    for (auto & t : threadArr)
        t.join();

/*
    for (int y = 0; y < msg.height; ++y) {
        for (int x = 0; x < msg.width; ++x) {
            batchedRenderResult.output->at(x, y) = tracer->renderPixel(msg.x+x, msg.y+y);
        }
    }
*/

    batchedRenderResult.output = output; // TODO: does a copy
    printf("output: %f %f %f\n",
           batchedRenderResult.output.at(0,0).r,
           batchedRenderResult.output.at(0,0).g,
           batchedRenderResult.output.at(0,0).b);
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

