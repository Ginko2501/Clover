#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "../clover.h"

using std::shared_ptr;
using std::make_shared;

#define min(a,b) a<b?a:b

class hittable_list : public hittable {
    public:
        hittable_list() {}
        hittable_list(shared_ptr<hittable> object) { add(object);}

        inline void clear() { objects.clear(); }

        inline void add(shared_ptr<hittable> object) {objects.push_back(object);}

        //inline shared_ptr<hittable> get(int x) { return objects.at(x);}

        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) override;

        virtual double dist(const point3& p, hittable*& obj) override;

        virtual point3 sample(double& pdf) override;

    public:
        std::vector<shared_ptr<hittable>> objects;
};

bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_record& rec) {
    hit_record temp_rec;
    int hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto& object : objects) {
        auto tmp = object->hit(r, t_min, closest_so_far, temp_rec);
        if (tmp) {
            hit_anything = tmp;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

double hittable_list::dist(const point3& p, hittable*& obj) {
    double min_d = infinity;
    hittable* min_obj;
    hittable* tmp_obj;
    for(const auto& object: objects){
        auto dist = object->dist(p, tmp_obj);
        if(dist < min_d){
            min_d = dist;
            min_obj = tmp_obj;
        }
    }
    obj = min_obj;
    return min_d;
}

point3 hittable_list::sample(double& pdf) {
    auto n = objects.size();
    for(const auto& object: objects){
        if(random_double() < 1/n)
            return object->sample(pdf);
        n--;
    }
}

#endif