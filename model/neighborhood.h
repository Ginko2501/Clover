#ifndef NEIGHBORHOOD_H
#define NEIGHBORHOOD_H

#include "../clover.h"

class hittable;
class material;

class neighborhood {
    public:
        nghd() {}

    public:
        point3 origin;
        vec3 normal;
        hittable* obj;
};

using nghd = neighborhood;

class hit_record : public nghd {
    public:
        //point3 p;
        double t;
};

class scatter_record : public nghd {
    public:
        ray r_out;
        double brdf;
        double pdf;
};

#endif