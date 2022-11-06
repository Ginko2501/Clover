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
        auto brdf = mat->brdf(r, r_out, hit_rec);
        auto pdf = mat->pdf(r_out, hit_rec);
        
        r = r_out;
        L += beta * mat->emit;
        beta *= mat->reflectance * cosine * brdf / pdf;
        // beta *= mat->reflectance * cosine * brdf / pdf / (1-1.0/bounces); // Russia Roulette
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
    ray rL;
    point3 pL;
    material* matL;
    hit_record hitL;

    // PDFs
    double pdfLL; // light ray as light pdf
    double pdfLS; // light ray as scatter pdf
    double pdfSL; // scatter ray as light pdf
    double pdfSS; // sactter ray as scatter pdf

    hit_record hit_rec;
    for(int i=0; i<bounces; i++) {
        if(!world.hit(r, epsilon, infinity, hit_rec)) {break;}

        material* mat = hit_rec.obj->mat;
        if(mat->name == "Light") {
            // stop tracing if hits light
            L += beta * mat->emit;
            break;
        }

        // Russian Roulette
        // if (random_double() >= 1 - 1.0/bounces){return L;}

        // indirect light
        auto rS = mat->scatter_sample(r, hit_rec);
        auto cosine = dot(rS.dir, hit_rec.normal);
        auto brdfS = mat->brdf(r, rS, hit_rec);

        // direct light
        pL = lights.light_sample();
        rL = ray(hit_rec.p, unit_vector(pL - hit_rec.p));

        // check if hit anything
        if(world.hit(rL, epsilon, infinity, hitL)){
            bool flag = true;

            // check if hits light
            matL = hitL.obj->mat;
            if(matL->name != "Light") {flag=false;}

            // check if hits the sampled point
            if(pL != hitL.p) {flag=false;}

            // check if the direct light is on the right side of the surface
            auto cosineL = dot(hit_rec.normal, rL.dir); // cosine term at light source
            if(cosineL <= 0) {flag=false;}

            if(flag){
                auto cosineS = - dot(hitL.normal, rL.dir); // cosine term at scatter surface
                auto dist_sq = (pL - hit_rec.p).length_squared();
                auto brdfL = matL->brdf(rL, rL, hitL); 

                pdfLL = hitL.obj->pdf(rL);
                pdfLS = mat->pdf(rL, hit_rec);

                // L += beta * mat->reflectance * matL->emit * brdfL * 
                //      cosineS * cosineL / dist_sq / pdfLL;
                L += beta * mat->reflectance * matL->emit * brdfL * 
                     cosineS * cosineL / dist_sq / pdfLL / (pdfLL + pdfLS);
            }
        }

        pdfSL = hitL.obj->pdf(rS);
        pdfSS = mat->pdf(rS, hit_rec);

        r = rS;
        //L += beta * mat->emit;
        L += beta * mat->emit / (pdfSL + pdfSS);
        beta *= mat->reflectance * cosine * brdfS / pdfSS;
        //beta *= mat->reflectance * cosine * brdfS / pdfSS / (1-1.0/bounces);
    }
    return L;
}

#endif