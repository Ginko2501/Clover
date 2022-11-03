#ifndef CORNELL_BOX_H
#define CORNELL_BOX_H

#include "../clover.h"

void cornell_box(hittable_list& world, hittable_list& lights, camera& cam) {
    // front face
    // world.add(make_shared<rectangle>(point3(5, 5, 0),
    //                                 vec3(5, 0, 0),
    //                                 vec3(0, 5, 0),
    //                                 &lambertian_white));

    // back face
    world.add(make_shared<rectangle>(point3(5, 5, -10),
                                    vec3(5, 0, 0),
                                    vec3(0, 5, 0),
                                    &lambertian_white));
    
    // bottom face
    world.add(make_shared<rectangle>(point3(5, 0, -5),
                                    vec3(5, 0, 0),
                                    vec3(0, 0, 5),
                                    &lambertian_white));

    // top face
    world.add(make_shared<rectangle>(point3(5, 10, -5),
                                    vec3(5, 0, 0),
                                    vec3(0, 0, 5),
                                    &lambertian_white)); 

    // right face  
    world.add(make_shared<rectangle>(point3(10, 5, -5),
                                    vec3(0, 5, 0),
                                    vec3(0, 0, 5),
                                    &lambertian_green)); 

    // left face                      
    world.add(make_shared<rectangle>(point3(0, 5, -5),
                                    vec3(0, 5, 0),
                                    vec3(0, 0, 5),
                                    &lambertian_red));
    
    // light source
    shared_ptr<rectangle> top_light = make_shared<rectangle>(point3(5, 10-0.5*epsilon, -5),
                                                  vec3(1, 0, 0),
                                                  vec3(0, 0, 1),
                                                  &material_light);
    world.add(top_light);
    lights.add(top_light);

    cam = camera(point3(5,5,2), point3(5,5,-5), vec3(0,1,0), 90, 1.0);
}

#endif