#pragma once

#include <utils/constants.hpp>


#include "math/math.hpp"

#include <utils/primitive_types.hpp>

namespace sim {

struct ParticlesData {
  explicit ParticlesData(math::scalar ppm);
  math::scalar particles_per_meter;
  math::scalar smoothing;
  math::scalar smoothing_pow_2;
  math::scalar smoothing_pow_6;
  math::scalar smoothing_pow_9;
  math::scalar mass;
  math::scalar f45_pi_smooth_6;
  math::scalar mass_pressure_05;
  math::scalar mass_goo;
};

struct Particle {
  Particle(size_t _id, math::vec3 _position, math::vec3 _hv, math::vec3 _velocity);

  Particle(Particle const&);

  void transformDensity(ParticlesData const& particles_params);

  static void incrementDensities(ParticlesData const& particles_params, Particle& particle_i, Particle& particle_j);

  static void incrementAccelerations(ParticlesData const& particles_params, Particle& particle_i, Particle& particle_j);

  u64 id;
  math::vec3 position;
  math::vec3 hv;
  math::vec3 velocity;
  math::vec3 acceleration;
  math::scalar density;
};

inline math::scalar densityIncrement(ParticlesData const& particles_params, math::scalar const squared_distance) {
  return pow(particles_params.smoothing_pow_2 - squared_distance, 3);
}

/**
 * Esta función calcula el incremento de aceleración entre dos partículas, `particle_i` y `particle_j`,
 * en función de los parámetros especificados en `params`. El incremento de aceleración se calcula teniendo en cuenta
 * la distancia al cuadrado entre las partículas.
 *
 * @param params Los parámetros de las partículas y la simulación.
 * @param particle_i La primera partícula para la que se calculará el incremento de aceleración.
 * @param particle_j La segunda partícula para la que se calculará el incremento de aceleración.
 * @param squared_distance La distancia al cuadrado entre las partículas.
 * @return Un vector 3D que representa el incremento de aceleración entre las partículas.
 */
inline auto accelerationIncrement(
    ParticlesData const& params, Particle const& particle_i, Particle const& particle_j,
    math::scalar const squared_distance
) {

  double const distance = sqrt(std::max(squared_distance, min_distance));
  math::vec3 const left = (particle_i.position - particle_j.position) * params.mass_pressure_05 *
                          (std::pow(params.smoothing - distance, 2) / distance) *
                          (particle_i.density + particle_j.density - 2 * density);

  math::vec3 const right         = (particle_j.velocity - particle_i.velocity) * params.mass_goo;
  math::scalar const denominator = particle_i.density * particle_j.density;
  return ((left + right) * params.f45_pi_smooth_6 / denominator);
}

} // namespace sim
