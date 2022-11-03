#ifndef SPHERE_TRACING_VOXEL_H
#define SPHERE_TRACING_VOXEL_H

#include "../clover.h"

// iterative approach
color sphere_trace_voxel(ray r, hittable_list& world, voxel_grid& SDF, int bounces){
    color L = color(0, 0, 0);
    color beta = color(1, 1, 1);

    for(int i=0; i<bounces; i++) {
        hittable* obj;
        double dist = world.dist(r.orig, obj);
               dist = SDF.at(r.orig);

        // sphere tracing until hits something
        while(dist > epsilon){
            if(dist > 100) {return L;}
            r.orig = r.orig + (dist-epsilon/2) * r.dir;
            dist = world.dist(r.orig, obj);
            dist = SDF.at(r.orig);
        }

        material* mat = obj->mat;
        if(mat->name == "Light") {
            // stop tracing if hits light
            L += beta * mat->emit;
            break;
        }

        // // Russian Roulette
        // if (random_double() >= 1 - 1.0/bounces) {break;}

        // temporary hit record 
        hit_record hit_rec;
        hit_rec.p = r.orig + dist * r.dir;
        hit_rec.obj = obj;

        // compute the new ray scattered
        scatter_record s_rec;
        mat->scatter(r, hit_rec, s_rec);
        s_rec.r_out.orig += 2 * epsilon * s_rec.r_out.dir; // times 2 to so that r_out not "hitting" the object

        auto n = hit_rec.obj->normal(hit_rec.p);
             n = dot(n, r.dir)>0 ? -n:n;
        auto cosine = dot(s_rec.r_out.dir, n);
        if(cosine<0){std::cerr<<"cosine<0\n";}

        r = s_rec.r_out;
        L += beta * mat->emit;
        beta *= mat->reflectance * cosine / s_rec.pdf;
        // Russia Roulette
        // beta *= mat->reflectance * cosine / s_rec.pdf / (1-1.0/bounces);
    }
        
    return L;
}

// recursive approach
color sphere_trace_voxel_recursive(ray r, hittable_list& world, voxel_grid& SDF){
    hittable* obj;
    double dist = world.dist(r.orig, obj);
           dist = SDF.at(r.orig);

    while(dist > epsilon){
        if(dist > 100) {
            return color(0,0,0);
        }
        r.orig = r.orig + (dist-epsilon/2) * r.dir;
        dist = world.dist(r.orig, obj);
        dist = SDF.at(r.orig);
    }
    //std::cerr<<world.dist(r.orig, obj)<<" "<<SDF.at(r.orig)<<"\n";

    material* mat = obj->mat;
    if(mat->name == "Light") {
        //std::cerr<<"light\n";
        std::cerr<<world.dist(r.orig, obj)<<"\n";
        return mat->emit;
    }
        
    // Russian Roulette
    // expected 20 recursions
    if (random_double() >= 0.9) {
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
    //std::cerr<<cosine<<"\n";
    //auto cosine = dot(s_rec.r_out.dir, hit_rec.obj->normal(hit_rec.p));

    return mat->emit + 
           mat->reflectance * cosine * sphere_trace_voxel_recursive(s_rec.r_out, world, SDF) / s_rec.pdf / 0.9;
    
}

#endif