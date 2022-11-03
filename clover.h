#ifndef CLOVER_H
#define CLOVER_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <limits>
#include <memory>
#include <vector>
#include <string>
#include <cstring>

// basics
#include "base/math.h"
#include "base/vec3.h"
#include "base/ray.h"
#include "base/color.h"

// scene
#include "model/camera.h"
#include "model/neighborhood.h"
// #include "model/hit_record.h"
#include "model/hittable.h"
#include "model/hittable_list.h"
#include "model/sphere.h"
#include "model/rectangle.h"

// more advanced base elements
#include "base/material.h"
#include "base/voxel_grid.h"

// rendering methods
#include "render/path_tracing.h"
// #include "render/path_tracing_recursive.h"
// #include "render/sphere_tracing_analytic.h"
// #include "render/sphere_tracing_voxel.h"

// inverse rendering
#include "inverse rendering/loss.h"

// scene
#include "scene/hello_world.h"
#include "scene/hello_world_IR.h"
#include "scene/cornell_box.h"

// Usings
using std::shared_ptr;
using std::make_shared;
using std::vector;
using std::sqrt;
using std::string;

#endif