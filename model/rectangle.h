#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "../clover.h"
#define max(a,b) a>b?a:b

class rectangle : public hittable {
    public:
        rectangle() {}
        rectangle(const point3& o, const vec3& h, const vec3& v, material* m){
            origin = o;
            horizontal = h;
            vertical = v;
            mat = m;
            n = unit_vector(cross(horizontal, vertical));
        }

        virtual vec3 normal(const point3& p) override { return n;}

        bool in_range(const point3& p);

        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) override;

        virtual double dist(const point3& p, hittable*& obj) override;    

        virtual point3 sample(double& pdf) override;
    
    public:
        vec3 horizontal;
        vec3 vertical;
        vec3 n;
};

bool rectangle::in_range(const point3& p) {
    if(fabs(dot(p-origin, horizontal) / horizontal.length()) > horizontal.length())
        return false;
    
    if(fabs(dot(p-origin, vertical) / vertical.length()) > vertical.length())
        return false;
    
    return true;
}

bool rectangle::hit(const ray& r, double t_min, double t_max, hit_record& rec) {
    vec3 n = normal(r.orig);
    auto t = dot(origin-r.orig, n) / dot(r.dir, n);

    if(dot(r.dir, n) == 0) return false;
    if (t<t_min || t>t_max) return false;
    if (!in_range(r.at(t))) return false;

    rec.t = t;
    rec.obj = this;
    rec.origin = r.at(t);
    rec.normal = dot(n, r.dir)>0 ? -n:n;

    return true;
}

double rectangle::dist (const point3& p, hittable*& obj) {
    obj = this;

    vec3 v = p - origin;
    double x = clamp(dot(v, horizontal) / horizontal.length_squared(), -1, 1);
    double y = clamp(dot(v, vertical) / vertical.length_squared(), -1, 1);
    point3 nearest = origin + (x * horizontal) + (y * vertical);

    return (p - nearest).length();
}

point3 rectangle::sample(double& pdf) {
    double x = 2*random_double() - 1;
    double y = 2*random_double() - 1;
    pdf = 4 * cross(horizontal, vertical).length();
    return origin + x*horizontal + y*vertical;
}

#endif