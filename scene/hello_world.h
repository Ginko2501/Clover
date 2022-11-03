#ifndef HELLO_WORLD_H
#define HELLO_WORLD_H

#include "../clover.h"

void hello_world(hittable_list& world, hittable_list& lights, camera& cam) {
    // ground
    world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, &material_ground));

    // center ball
    world.add(make_shared<sphere>(point3( 0, 0.0, -1.0), 0.5, &material_center));

    // top rectangle light
    shared_ptr<rectangle> top_light = make_shared<rectangle>(point3(0, 1.5, -1),
                                                             vec3(1, 0, 0),
                                                             vec3(0, 0, 0.5),
                                                             &material_light_bright);
    world.add(top_light);
    lights.add(top_light);

    cam = camera(point3(0,0,0.5), point3(0,0.5,-1), vec3(0,1,0), 90, 1.0);
}

#endif