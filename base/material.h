#ifndef MATERIAL_H
#define MATERIAL_H

#include "../clover.h"

struct hit_record;

// struct sample {
//     ray r_out;
//     color reflectance;
//     double pdf;
//     double brdf;
// };

class material {
    public:
        virtual ray scatter_sample(ray& r_in, hit_record& rec) {}

        virtual double pdf(point3& p, ray& r_out) {return infinity;}

        virtual double brdf(point3& p, ray& r_in, ray& r_out) {return 1;}
    
    public:
        std::string name;
        color emit;
        color reflectance;
};

class light : public material {
    public:
        light() {
            name = "Light";
            emit = color(5, 5, 5);
        }
        light(const color& c) {
            name = "Light";
            emit = c;
        }
};

class lambertian : public material {
    public:
        lambertian() {name = "Lambertian";}
        lambertian(const color& a) {
            name = "Lambertian";
            emit = color(0, 0, 0);
            reflectance = a;
        }
        lambertian(const color& a, const color& e) {
            name = "Lambertian";
            emit = e;
            reflectance = a;
        }

        virtual ray scatter_sample(ray& r_in, hit_record& rec) override {
            auto scatter_direction = rec.normal + random_unit_vector();

            // Catch degenerate scatter direction
            if (is_zero(scatter_direction))
                scatter_direction = rec.normal;

            ray r_out = ray(rec.origin, unit_vector(scatter_direction));
            return r_out;
        }

        virtual double pdf(point3& p, ray& r_out) {return 0.5 / pi;}

        virtual double brdf(point3& p, ray& r_in, ray& r_out) {return 1.0 / pi;}
};


// class metal : public material {
//     public:
//         metal(const color& a, double f) {
//             name = "Metal";
//             emit = color(0, 0, 0);
//             reflectance = a;
//             fuzz = f < 1 ? f : 1;
//         }

//         virtual void scatter_sample(ray& r_in, sample& s) override {
//             vec3 reflected = reflect(unit_vector(r_in.direction()), s.obj->normal(s.origin));
//             s.r_out = ray(s.origin, reflected + fuzz*random_in_unit_sphere());
//             //return (dot(scattered.direction(), rec.normal()) > 0);
//         }

//     public:
//         double fuzz;
// };

// do not declare as material
auto material_light = light();
auto material_light_dim = light(color(1, 1, 1));
auto material_light_bright = light(color(15, 15, 15));

auto material_ground = lambertian(color(0.1, 0.1, 0.0));
auto material_center = lambertian(color(0.1, 0.2, 0.5));

#endif