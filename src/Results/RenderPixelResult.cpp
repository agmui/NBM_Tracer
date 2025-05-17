//
// Created by agmui on 5/16/25.
//

#include "RenderPixelResult.h"


vector<uint8_t> RenderPixelResult::serialize() {
    vector<uint8_t> data; //TODO: optimise
    data.resize(sizeof(pixel));
    memcpy(data.data(),&p,  sizeof(pixel));
    return data;
}

void RenderPixelResult::fill(vector<uint8_t> serializedData) {
    memcpy( &p,serializedData.data(),  sizeof(pixel));
}
