#include "../clover.h"

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
const double step_size = 5e-6;

// initialize center material with random blue color
auto center = lambertian(color(0.1, 0.2, 0.3));
vector<int> loss_record;
vector<color> color_record;
vector<color> gradient_record;

extern vec3 __enzyme__autodiff(void*, vec3);

void render() {
    // update world scene
    world.clear();
    hello_world_IR(world, lights, cam, center);

    // render
    freopen("I.ppm", "w", stdout);
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    for (int j = image_height-1; j >= 0; --j) {
        //std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + 1.2*random_double()) / (image_width-1);
                auto v = (j + 1.2*random_double()) / (image_height-1);
                ray r = cam.get_ray(u, v);
                //pixel_color += sphere_trace_analytic(r, world);
                //pixel_color += sphere_trace_voxel(r, world, SDF, 3);
                pixel_color += path_trace(r, world, lights, 3);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }
    std::cerr << "Rendered once.\n";
}



int main() {
    // world scene
    hello_world_IR(world, lights, cam, center);

    // initalize
    loss_record.clear();
    color_record.clear();
    gradient_record.clear();

    // compute initial status
    render();
    auto loss_m = loss();
    auto gradient = compute_gradient();

    loss_record.push_back(loss_m);
    color_record.push_back(center.reflectance);
    gradient_record.push_back(gradient);

    std::cerr<<"lost: "<<loss_m<<"\n"; 
    std::cerr<<"gradient: "<<gradient<<"\n";
    std::cerr<<"current color: "<<center.reflectance<<"\n\n";

    // gradient descent
    while(gradient.length() >= 15) {
        center.reflectance = center.reflectance - step_size * gradient;
        gradient = compute_gradient();
        render();
        loss_m = loss();
        std::cerr<<"lost: "<<loss_m<<"\n"; 
        std::cerr<<"gradient: "<<gradient<<"\n";
        std::cerr<<"current color: "<<center.reflectance<<"\n\n";
    }
    return 0;
}