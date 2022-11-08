#ifndef SANITY_CHECK_H
#define SANITY_CHECK_H

#include "../clover.h"

void sanity_check(hittable_list& world, hittable_list& lights, camera& cam) {
    world.clear();
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5, &material_center));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100, &material_center));

    lights.clear();

    cam = camera(point3(0,0,0), point3(0,0,-1), vec3(0,1,0), 90, 16.0/9.0);
}

#endif