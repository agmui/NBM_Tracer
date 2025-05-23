//
// Created by agmui on 5/19/25.
//

#include <cstdio>
#include <cstring>
#include "BatchedRenderResult.h"
#include "SerializationHelper.h"

size_t BatchedRenderResult::size() {
    return sizeof(pixel) * output.getWidth() * output.getHeight();
}

vector<uint8_t> BatchedRenderResult::serialize() {
//    printf("start serialize():\n");
    vector<uint8_t> v;
    v.resize(size());
    memcpy(v.data(), &output.at(0,0), size());
    return v;
//    return &output.at(0,0); //TODO: try this
}

void BatchedRenderResult::fill(vector<uint8_t> serializedData) {
    memcpy(&output.at(0,0), serializedData.data(), size());
}

void BatchedRenderResult::printResult() {
    printf("x: %d y: %d pixel: %f %f %f\n", x, y,
           output.at(0,0).r, output.at(0,0).g, output.at(0,0).b);
}
