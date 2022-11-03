#ifndef HIT_RECORD_H
#define HIT_RECORD_H

#include "../clover.h"

class material;
class hittable;

struct hit_record {
        double t;
        point3 p;
        hittable* obj;
};


#endif