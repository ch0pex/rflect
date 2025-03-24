#pragma once

#include "math/vector.hpp"

namespace sim {
  // Sim constants
  constexpr double mul_rad   = 1.695;    // r
  constexpr double density   = 1000.0;   // p
  constexpr double pressure  = 3.0;      // p_s
  constexpr double collision = 30000.0;  // s_c
  constexpr double damping   = 128.0;    // d_v
  constexpr double goo       = 0.4;      // NU
  constexpr double particle_size    = 0.0002;   // d_r
  constexpr double time_step = 0.001;    // DELTA*t
  constexpr double squared_time_step = time_step * time_step;
  constexpr double min_distance = 0.000000000001; //10^-12
  constexpr double min_collision_diff =  0.0000000001; //10^-10
  constexpr math::vec3 gravity = {0.0, -9.8, 0.0};

  // File constants
  constexpr size_t header_size = 8;
  constexpr size_t particle_components = 9;

  // Grid limits
  constexpr math::vec3 bottom_limit = {-0.065, -0.08, -0.065};
  constexpr math::vec3 top_limit = {0.065, 0.1, 0.065};

  enum Limits {
    cx0,
    cy0,
    cz0,
    cxn,
    cyn,
    czn
  };


}  // namespace sim
