#ifndef PATH_TRACING_RECURSIVE_H
#define PATH_TRACING_RECURSIVE_H

#include "../clover.h"

// Russian Roulette approach
// expectedly compute all bounces, but with high variance
color path_trace_recursive(ray& r, hittable_list& world, hittable_list& lights, int bounces) {
    hit_record hit_rec;
    if(world.hit(r, epsilon, infinity, hit_rec)){
        material* mat = hit_rec.obj->mat;
        if(mat->name == "Light") {
            return mat->emit;
        }

        // Russian Roulette
        if (random_double() >= 1 - 1.0/bounces)
            return color(0, 0, 0); 

        auto r_out = mat->scatter_sample(r, hit_rec);
        auto cosine = dot(hit_rec.normal, r_out.dir);
        auto brdf = mat->brdf(r, r_out, hit_rec);
        auto pdf = mat->pdf(r_out, hit_rec);

        return mat->emit + 
               mat->reflectance * path_trace_recursive(r_out, world, lights, bounces) *
               cosine * brdf / pdf / (1-1.0/bounces);
    }

    return color(0, 0, 0);
}

// limited-bounce approach
// compute limited bounces
// color path_trace_recursive(ray& r, hittable_list& world, int depth) {
//     if(depth >= 50) {return color(0, 0, 0);}

//     hit_record hit_rec;
//     if(world.hit(r, epsilon, infinity, hit_rec)){
//         material* mat = hit_rec.obj->mat;
//         if(mat->name == "Light") {
//             return mat->emit;
//         }

//         scatter_record s_rec;
//         mat->scatter(r, hit_rec, s_rec);
//         auto n = hit_rec.obj->normal(hit_rec.p);
//         if(dot(n, r.dir)>0) n=-n;
//         auto cosine = dot(s_rec.r_out.dir, n);

//         return mat->emit + 
//                mat->reflectance * cosine * path_trace_recursive(s_rec.r_out, world, depth+1) / s_rec.pdf / 0.5;
//     }

//     return color(0, 0, 0);
// }

#endif