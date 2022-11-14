#ifndef TWO_SPHERES_H
#define TWO_SPHERES_H

#include "../clover.h"

auto yellow = lambertian(color(0.7, 0.7, 0.4));
auto gray = lambertian(color(0.2, 0.2, 0.2));

void two_spheres(hittable_list& world, hittable_list& lights, camera& cam) {
    world.clear();
    world.add(make_shared<sphere>(point3(0, 0, 0), 0.5, &yellow));
    world.add(make_shared<sphere>(point3(0, -40, 0), 39.5, &gray));

    shared_ptr<sphere> sphere_light = make_shared<sphere>(point3(12, 10, 5), 5, &material_light);
    lights.clear();
    lights.add(sphere_light);
    world.add(sphere_light);

    cam = camera(point3(3,1.7,5), point3(0,0,0), vec3(0,1,0), 25, 16.0/9.0);
}

#endif