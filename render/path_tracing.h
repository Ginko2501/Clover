#ifndef PATH_TRACING_H
#define PATH_TRACING_H

#include "../clover.h"

// iterative approach
// does not calculate direct light
color path_trace(ray& r, hittable_list& world, hittable_list& lights, int bounces) {
    // L is accumulative light
    // beta is decaying factor
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

        auto r_out = mat->scatter_sample(r, hit_rec);
        auto cosine = dot(r_out.dir, hit_rec.normal);
        auto brdf = mat->brdf(hit_rec.p, r, r_out);
        auto pdf = mat->pdf(hit_rec.p, r_out);
        
        r = r_out;
        L += beta * mat->emit;
        beta *= mat->reflectance * cosine * brdf / pdf;
        // beta *= mat->reflectance * cosine * brdf / pdf / (1-1.0/bounces); // Russia Roulette
    }

    return L;
}

// // direct light approach 
// color path_trace_directL(ray& r, hittable_list& world, hittable_list& lights, int bounces) {
//     // L is accumulative light
//     // beta is decaying factor
//     color L = color(0, 0, 0);
//     color beta = color(1, 1, 1);

//     // variables for sampling at light source
//     ray rL;
//     point3 pL;
//     material* matL;
//     hit_record hitL;

//     hit_record hit_rec;
//     for(int i=0;; i++) {
//         if(!world.hit(r, epsilon, infinity, hit_rec)) {break;}

//         material* mat = hit_rec.obj->mat;
//         if(mat->name == "Light") {
//             // stop tracing if hits light
//             L += beta * mat->emit;
//             break;
//         }

//         // direct light
//         auto Ld = color(0, 0, 0);
//         for(int j=0; j<1; j++) {
//             pL = lights.light_sample();
//             rL = ray(hit_rec.p, unit_vector(pL - hit_rec.p));

//             // check if hit anything
//             if(!world.hit(rL, epsilon, infinity, hitL)) {continue;}

//             // check if hits light
//             matL = hitL.obj->mat;
//             if(matL->name != "Light") {continue;}

//             // check if hits the sampled point
//             if(pL != hitL.p) {continue;}

//             auto dist_sq = (pL - hit_rec.p).length_squared();
//             auto cosine  = - dot(hitL.normal, rL.dir);
//             auto cosineL =   dot(hit_rec.normal, rL.dir); // cosine term at hittable object
            
//             // check if the direct light is on the right side of the surface
//             if(cosineL <= 0) {continue;}
            
//             Ld = beta * mat->reflectance * matL->emit *
//                  cosine * cosineL / dist_sq / s.pdf;
//         }

//         // Russian Roulette
//         if (random_double() >= 1 - 1.0/bounces){return L;}

//         // indirect light
//         auto r_out = mat->scatter_sample(r, hit_rec);
//         auto cosine = dot(r_out.dir, hit_rec.normal);
//         auto brdf = mat->brdf(hit_rec.p, r, r_out);
//         auto pdf = mat->pdf(hit_rec.p, r_out);

//         r = r_out;

//         if(Ld == color(0,0,0)) {
//             L += beta * mat->emit;
//             // beta *= mat->reflectance * cosine * s.brdf / s.pdf;
//             // Russia Roulette
//             beta *= mat->reflectance * cosine * s.brdf / s.pdf / (1-1.0/bounces); // Russia Roulette
//         } else {
//             L += 0.5 * Ld;
//             L += 0.5 * beta * mat->emit;
//             // beta *= mat->reflectance * cosine * s.brdf / s.pdf;
//             // Russia Roulette
//             beta *= 0.5 * mat->reflectance * cosine * s.brdf / s.pdf / (1-1.0/bounces); // Russia Roulette
//         }
 
//     }
//     return L;
// }

#endif