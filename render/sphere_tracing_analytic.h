#ifndef SPHERE_TRACING_ANALYTIC_H
#define SPHERE_TRACING_ANALYTIC_H

#include "../clover.h"

color sphere_trace_analytic(ray& r, hittable_list& world){
    //std::cerr<<r.orig<<"\n";

    hittable* obj;
    double dist = world.dist(r.orig, obj);

    while(dist > epsilon){
        //std::cerr<<r.orig<<" "<<dist<<" "<<obj->mat->name<<"\n";
        if(dist > 100) {
            return color(0,0,0);
        }
        r.orig = r.orig + (dist-(1e-5)) * r.dir;
        dist = world.dist(r.orig, obj);
    }
    //std::cerr<<"reached here"<<obj->mat->name<<"\n";

    material* mat = obj->mat;
    if(mat->name == "Light") {
        return mat->emit;
    }
        
    // Russian Roulette
    // expected 20 recursions
    if (random_double() >= 0.8) {
        return color(0, 0, 0);
    }

    // temporary hit record 
    hit_record hit_rec;
    hit_rec.p = r.orig + dist * r.dir;
    hit_rec.obj = obj;

    scatter_record s_rec;
    mat->scatter(r, hit_rec, s_rec);
    s_rec.r_out.orig += 2 * epsilon * s_rec.r_out.dir;

    auto n = hit_rec.obj->normal(hit_rec.p);
    if(dot(n, r.dir)>0) n=-n;
    auto cosine = dot(s_rec.r_out.dir, n);

    //auto cosine = dot(s_rec.r_out.dir, hit_rec.obj->normal(hit_rec.p));

    return mat->emit + 
           mat->reflectance * cosine * sphere_trace_analytic(s_rec.r_out, world) / s_rec.pdf / 0.8;
    
}

#endif