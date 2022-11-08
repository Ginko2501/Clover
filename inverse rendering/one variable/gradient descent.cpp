#include "../../clover.h"

// Globals
// declare as global variables so that no need to make a copy for every recursive call
camera cam;
hittable_list world;
hittable_list lights;
voxel_grid SDF;

// Image
const auto aspect_ratio = 1.0;
const int image_width = 256;
const int image_height = static_cast<int>(image_width / aspect_ratio);
const int samples_per_pixel = 200;

// initialize center material with random blue color
auto center = lambertian(color(0.1, 0.2, 0.3));

void render() {
    // update world scene
    world.clear();
    hello_world_IR(world, lights, cam, center);

    // render
    freopen("I.ppm", "w", stdout);
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
                pixel_color += path_trace_directL(r, world, lights, 20);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }
    std::cerr << "\nDone.\n";
}

double compute_derivative(int x) {
    // compute loss(x-)
    center.reflectance.e[x] -= 0.01;
    render();
    auto loss_l = loss();
    // std::cerr<<center.reflectance<<"\n";
    // std::cerr<<loss_l<<"\n";

    // compute loss(x+)
    center.reflectance.e[2] += 2 * 0.01;
    render();
    auto loss_r = loss();
    // std::cerr<<center.reflectance<<"\n";
    // std::cerr<<loss_r<<"\n";

    center.reflectance.e[2] -= 0.01;
    return 0.5 * (loss_r - loss_l) / 0.01;
}

double compute_gradient() {
    // compute loss(b-)
    center.reflectance.e[2] -= 0.01;
    world.clear();
    hello_world_IR(world, lights, cam, center);
    render();
    auto loss_l = loss();
    // std::cerr<<center.reflectance<<"\n";
    std::cerr<<loss_l<<"\n";

    // compute loss(b+)
    center.reflectance.e[2] += 2 * 0.01;
    world.clear();
    hello_world_IR(world, lights, cam, center);
    render();
    auto loss_r = loss();
    // std::cerr<<center.reflectance<<"\n";
    std::cerr<<loss_r<<"\n";

    center.reflectance.e[2] -= 0.01;
    return 0.5 * (loss_r - loss_l) / 0.01;
}

int main() {
    // world scene
    hello_world_IR(world, lights, cam, center);
    //cornell_box(world, lights, cam);

    // Voxel_Grid
    //SDF.init(world);

    auto step_size = 1e-4;
    auto loss_m = loss();
    auto gradient = compute_gradient();
    std::cerr<<gradient<<"\n\n";
    while(fabs(gradient) >= 10) {
        center.reflectance.e[2] -= step_size * gradient;
        gradient = compute_gradient();
        render();
        loss_m = loss();
        std::cerr<<"lost: "<<loss_m<<"\n"; 
        std::cerr<<"gradient: "<<gradient<<"\n";
        std::cerr<<"current blue: "<<center.reflectance.e[2]<<"\n\n";
    }

    render();
    std::cerr<<"loss is "<<loss()<<"\n";
    std::cerr<<"current blue: "<<center.reflectance.e[2]<<"\n\n";

    return 0;
}