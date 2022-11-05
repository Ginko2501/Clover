#ifndef SPHERE_H
#define SPHERE_H

#include "../clover.h"

class sphere : public hittable {
    public:
        sphere() {}
        sphere(const point3& o, double r, material* m){
            origin = o;
            radius = r;
            mat = m;
            area = (4 * pi * r * r);
            //name = "Sphere";
        }

        virtual vec3 normal(const point3& p) override {
            return unit_vector(p - origin);
        }

        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) override;

        virtual double dist(const point3& p, hittable*& obj) override;

        virtual double pdf(ray& r) override;

        virtual point3 light_sample() override;
        
    public:
        double radius;
        double area;
        //int id;
        //std::string name;
};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) {
    vec3 oc = r.origin() - origin;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius*radius;

    auto discriminant = half_b*half_b - a*c;
    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    // Update hit record
    rec.t = root;
    rec.obj = this;
    rec.p = r.at(rec.t);
    rec.origin = r.at(rec.t);
    rec.normal = normal(rec.origin);
    rec.normal = dot(rec.normal, r.dir)>0 ? -rec.normal : rec.normal;

    return true;
}

double sphere::dist(const point3& p, hittable*& obj) {
    obj = this;
    return (p-origin).length() - radius;
}

double sphere::pdf(ray& r) {
    hit_record hit_rec;
    if(sphere::hit(r, epsilon, infinity, hit_rec)){
        return 1 / area;
    }
    return 0;
}

point3 sphere::light_sample() {
    return origin + radius * random_unit_vector();
}

#endif