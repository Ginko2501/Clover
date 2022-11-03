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
        point3 p;
        double t;
};

class scatter_record : public nghd {
    public:
        scatter_record() {}
        scatter_record(hit_record& hit_rec) {
            origin = hit_rec.origin;
            normal = hit_rec.normal;
            obj = hit_rec.obj;
        }

    public:
        ray r_out;
        double brdf;
        double pdf;
};

#endif