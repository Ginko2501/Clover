#ifndef PATH_TRACING_H
#define PATH_TRACING_H

#include "../clover.h"

// iterative approach
// does not calculate direct light
color path_trace(ray& r, hittable_list& world, int bounces) {
    color L = color(0, 0, 0);
    color beta = color(1, 1, 1);

    for(int i=0;; i++) {
        hit_record hit_rec;
        if(!world.hit(r, epsilon, infinity, hit_rec)) { break; }

        material* mat = hit_rec.obj->mat;
        if(mat->name == "Light") {
            L += beta * mat->emit;
            break;
        }

        // Russian Roulette
        if (random_double() >= 1 - 1.0/bounces) {return L;}

        scatter_record s_rec;
        mat->scatter(r, hit_rec, s_rec);

        auto n = hit_rec.obj->normal(hit_rec.origin);
             n = dot(n, r.dir)>0 ? -n:n;
        auto cosine = dot(s_rec.r_out.dir, n);

        r = s_rec.r_out;
        L += beta * mat->emit;
        // beta *= mat->reflectance * cosine * s_rec.brdf / s_rec.pdf;
        // Russia Roulette
        beta *= mat->reflectance * cosine * s_rec.brdf / s_rec.pdf / (1-1.0/bounces);
    }

    return L;
}

// direct light approach 
color path_trace_directL(ray& r, hittable_list& world, hittable_list& lights, int bounces) {
    // L is accumulative light
    // beta is decaying factor
    color L = color(0, 0, 0);
    color beta = color(1, 1, 1);

    // variables for sampling at light source
    point3 sampled;
    ray sample_r;
    double sample_pdf;
    material* sample_mat;

    hit_record hit_rec;
    scatter_record s_rec;
    for(int i=0;; i++) {
        if(!world.hit(r, epsilon, infinity, hit_rec)) {break;}

        material* mat = hit_rec.obj->mat;
        if(mat->name == "Light") {
            // stop tracing if hits light
            L += beta * mat->emit;
            break;
        }

        // compute surface normal and cosine
        auto n = hit_rec.obj->normal(hit_rec.origin);
             n = dot(n, r.dir)>0 ? -n:n;

        // direct light
        for(int j=0; j<1; j++) {
            sampled = lights.sample(sample_pdf);
            sample_r = ray(hit_rec.origin, unit_vector(sampled-hit_rec.origin));

            hit_record sample_rec;
            if(!world.hit(sample_r, epsilon, infinity, sample_rec)) {continue;}

            // check if hits the sampled point
            if(sampled != sample_rec.origin) {continue;}

            sample_mat = sample_rec.obj->mat;
            if(sample_mat->name != "Light") {continue;}

            auto n_L = sample_rec.obj->normal(sample_rec.origin); // normal at light source
                 n_L = dot(n_L, sample_r.dir)>0 ? -n_L:n_L;
            auto cosine_L = - dot(sample_r.dir, n_L); // cosine term at light source
            auto cosine_r = dot(n, sample_r.dir); // cosine term at hittable object

            // check if the direct light is on the right side of the surface
            if(cosine_r <= 0) {continue;}

            L += beta * mat->reflectance * sample_mat->emit *
                 cosine_r * cosine_L / (sample_rec.origin-hit_rec.origin).length_squared() * sample_pdf;
        }

        // Russian Roulette
        if (random_double() >= 1 - 1.0/bounces){return L;}

        // indirect light
        mat->scatter(r, hit_rec, s_rec);
        auto cosine = dot(s_rec.r_out.dir, n);

        r = s_rec.r_out;
        L += beta * mat->emit;
        // beta *= mat->reflectance * cosine * s_rec.brdf / s_rec.pdf;
        // Russia Roulette
        beta *= mat->reflectance * cosine * s_rec.brdf / s_rec.pdf / (1-1.0/bounces); // Russia Roulette
    }
    return L;
}

#endif