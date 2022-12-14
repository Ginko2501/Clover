#ifndef NEIGHBORHOOD_H
#define NEIGHBORHOOD_H

#include "../clover.h"

class hittable;
class material;

class neighborhood {
    public:
        neighborhood() {}

    public:
        point3 origin;
        vec3 normal;
        hittable* obj;
};

class hit_record : public neighborhood {
    public:
        point3 p;
        double t;
};

// class scatter_record : public nghd {
//     public:
//         scatter_record() {}
//         scatter_record(hit_record& hit_rec) {
//             origin = hit_rec.origin;
//             normal = hit_rec.normal;
//             obj = hit_rec.obj;
//         }

//     public:
//         ray r_out;
//         double brdf;
//         double pdf;
// };

// class sample : public nghd {
//     public:
//         sample() {}
//         sample(hit_record& hit_rec) {
//             origin = hit_rec.origin;
//             normal = hit_rec.normal;
//             obj = hit_rec.obj;
//         }
    
//     public:
//         ray r_out;
//         double pdf;
//         double brdf;
// };

#endif