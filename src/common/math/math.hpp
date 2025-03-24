#ifndef FLUID_MATH_HPP
#define FLUID_MATH_HPP

#include <cmath>

#include "math/vector.hpp"
#include "utils/constants.hpp"
#include "particle.hpp"


namespace sim::math {
  double DensityIncrement(const ParticlesData& particles_params, double squared_distance);
  vec3d AccelerationIncrement(const ParticlesData& params, const Particle& particle_i, const Particle& particle_j, double squared_distance);

}


#endif  // FLUID_MATH_HPP
