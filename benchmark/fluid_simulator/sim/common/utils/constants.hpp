#pragma once

#include "math/vector.hpp"

namespace sim {
// Sim constants
constexpr math::scalar mul_rad            = 1.695; // r
constexpr math::scalar density            = 1000.0; // p
constexpr math::scalar pressure           = 3.0; // p_s
constexpr math::scalar collision          = 30000.0; // s_c
constexpr math::scalar damping            = 128.0; // d_v
constexpr math::scalar goo                = 0.4; // NU
constexpr math::scalar particle_size      = 0.0002; // d_r
constexpr math::scalar time_step          = 0.001; // DELTA*t
constexpr math::scalar squared_time_step  = time_step * time_step;
constexpr math::scalar min_distance       = 0.000000000001; // 10^-12
constexpr math::scalar min_collision_diff = 0.0000000001; // 10^-10
constexpr math::vec3 gravity              = {0.0, -9.8, 0.0};
constexpr math::scalar pi_times_64        = 64.0 * std::numbers::pi;
constexpr math::scalar density_times_2    = 2.0 * density; // p
math::scalar const min_distance_sqrt      = std::sqrt(min_distance); // 10^-12

// File constants
constexpr size_t header_size         = 8;
constexpr size_t particle_components = 9;

// Grid limits
constexpr math::vec3 bottom_limit = {-0.065, -0.08, -0.065};
constexpr math::vec3 top_limit    = {0.065, 0.1, 0.065};

enum Limits { cx0, cy0, cz0, cxn, cyn, czn };


} // namespace sim
