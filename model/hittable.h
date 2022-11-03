#ifndef HITTABLE_H
#define HITTABLE_H

#include "../clover.h"

class material;
class hit_record;

class hittable {
    public:
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) = 0;
    
        virtual double dist(const point3& p, hittable*& obj) = 0;

        virtual vec3 normal(const point3& p) {return vec3(0, 0, 0);}

        virtual point3 sample(double& pdf) {return point3(infinity, infinity, infinity);}
    
    public:
        //int id;
        //std::string name;
        point3 origin;
        material* mat;
};



#endif