#ifndef SAMPLER_H
#define SAMPLER_H

#include "point.h"
#include <vector>

class PixelSampler {

    public:
    PixelSampler(int samplesPerPixel = 1) : samplesPerPixel{samplesPerPixel} {};
    Point2f Get2D();
    void virtual StartPixel() = 0;
    virtual ~PixelSampler() {};
    int samplesPerPixel;

    protected:
    std::vector<Point2f> samples;
    int sampleOffset;
    

};

class StratifiedSampler : public PixelSampler {
    public: // uhh this is a bit evil, please only use perfect squares for now thanks
    StratifiedSampler(int samplesPerPixel = 1);
    void virtual StartPixel();

    private:
    int samplesPerEdge;
};


#endif