//
// Created by agmui on 5/13/25.
//

#ifndef NBM_TRACER_TRACERINTERFACE_H
#define NBM_TRACER_TRACERINTERFACE_H

struct pixel {
    float r;
    float g;
    float b;
};

class TracerInterface {
public:
    TracerInterface() = default;
    ~TracerInterface() = default;
    virtual void init() = 0;

    virtual pixel renderPixel(int x, int y) = 0;
};


#endif //NBM_TRACER_TRACERINTERFACE_H
