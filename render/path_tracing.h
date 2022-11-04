#ifndef PATH_TRACING_H
#define PATH_TRACING_H

#include "../clover.h"

// iterative approach
// does not calculate direct light
color path_trace(ray& r, hittable_list& world, hittable_list& lights, int bounces) {
    color L = color(0, 0, 0);
    color beta = color(1, 1, 1);

    for(int i=0; i<bounces; i++) {
        hit_record hit_rec;
        if(!world.hit(r, epsilon, infinity, hit_rec)) { break; }

        material* mat = hit_rec.obj->mat;
        if(mat->name == "Light") {
            L += beta * mat->emit;
            break;
        }

        // // Russian Roulette
        // if (random_double() >= 1 - 1.0/bounces) {return L;}

        sample s = hit_rec;
        mat->scatter_sample(r, s);
        auto cosine = dot(s.r_out.dir, s.normal);
        
        r = s.r_out;
        L += beta * mat->emit;
        beta *= mat->reflectance * cosine * s.brdf / s.pdf;
        // // Russia Roulette
        // beta *= mat->reflectance * cosine * s.brdf / s.pdf / (1-1.0/bounces);
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
    sample s;
    for(int i=0;; i++) {
        if(!world.hit(r, epsilon, infinity, hit_rec)) {break;}

        material* mat = hit_rec.obj->mat;
        if(mat->name == "Light") {
            // stop tracing if hits light
            L += beta * mat->emit;
            break;
        }

        // direct light
        color L_direct = color(0, 0, 0);
        for(int j=0; j<1; j++) {
            sampled = lights.sample(sample_pdf);
            sample_r = ray(hit_rec.p, unit_vector(sampled-hit_rec.p));

            hit_record sample_rec;
            if(!world.hit(sample_r, epsilon, infinity, sample_rec)) {continue;}

            // check if hits the sampled point
            if(sampled != sample_rec.origin) {continue;}

            sample_mat = sample_rec.obj->mat;
            if(sample_mat->name != "Light") {continue;}

            auto cosine_L = - dot(sample_rec.normal, sample_r.dir); // cosine term at light source
            auto cosine_r = dot(hit_rec.normal, sample_r.dir); // cosine term at hittable object

            // check if the direct light is on the right side of the surface
            if(cosine_r <= 0) {continue;}

            L_direct = beta * mat->reflectance * sample_mat->emit *
                       cosine_r * cosine_L / (sample_rec.origin-hit_rec.p).length_squared() * sample_pdf / pi;
        }

        // Russian Roulette
        if (random_double() >= 1 - 1.0/bounces){return L;}

        // indirect light
        s = hit_rec;
        mat->scatter_sample(r, s);
        auto cosine = dot(s.r_out.dir, hit_rec.normal);

        r = s.r_out;
        if(L_direct == color(0,0,0)) {
            L += beta * mat->emit;
            // beta *= mat->reflectance * cosine * s.brdf / s.pdf;
            // Russia Roulette
            beta *= mat->reflectance * cosine * s.brdf / s.pdf / (1-1.0/bounces); // Russia Roulette
        } else {
            L += 0.5 * L_direct;
            L += 0.5 * beta * mat->emit;
            // beta *= mat->reflectance * cosine * s.brdf / s.pdf;
            // Russia Roulette
            beta *= 0.5 * mat->reflectance * cosine * s.brdf / s.pdf / (1-1.0/bounces); // Russia Roulette
        }
 
    }
    return L;
}

#endif