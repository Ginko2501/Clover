#include "clover.h"

// declare global variable so that no need to make a copy for every recursive call
hittable_list world;
hittable_list lights;
voxel_grid SDF;

int main() {
    freopen("./images/image.ppm", "w", stdout);

    // Image
    const auto aspect_ratio = 1.0;
    const int image_width = 512;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 2000;

    // Camera
    camera cam;
    //sanity_check(world, lights, cam);
    //hello_world(world, lights, cam);
    //hello_world_IR(world, lights, cam, material_center);
    cornell_box(world, lights, cam);

    // Voxel_Grid
    //SDF.init(world);
    
    // check single ray trace
    // auto u = 0.773971;
    // auto v = 0.997264;
    // ray r = cam.get_ray(u, v);
    // for(int i=0;i<200;i++)
    //     std::cerr<<path_trace_iterative2(r, world, lights, 3)<<"...\n";
    // std::cerr<<"finished\n";
    // return 0;

    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + 1.2*random_double()) / (image_width-1);
                auto v = (j + 1.2*random_double()) / (image_height-1);
                ray r = cam.get_ray(u, v);
                //pixel_color += sphere_trace_analytic(r, world);
                //pixel_color += sphere_trace_voxel(r, world, SDF, 3);
                pixel_color += path_trace_directL(r, world, lights, 5);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";
    return 0;
}