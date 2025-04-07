#pragma once

#include "math/math.hpp"
#include "utils/constants.hpp"
#include "utils/primitive_types.hpp"
#include "reflected/reflection/dual_vector.hpp"
#include "reflected/reflection/proxy_class.hpp"

#include <print>

namespace sim {

struct FluidProperties {
  math::scalar particles_per_meter;
  math::scalar smoothing;
  math::scalar smoothing_pow_2;
  math::scalar smoothing_pow_6;
  math::scalar smoothing_pow_9;
  math::scalar mass;
  math::scalar f45_pi_smooth_6;
  math::scalar mass_pressure_05;
  math::scalar mass_goo;
  math::scalar transform_density_constant;
};


struct Particle {
  DEFINE_PROXY(id, position, hv, velocity, acceleration, density);

  u32 id {};
  math::vec3 position;
  math::vec3 hv;
  math::vec3 velocity;
  math::vec3 acceleration {gravity};
  math::scalar density {};

};

constexpr FluidProperties fluid_properties(math::scalar const ppm) {
  auto const smoothing = mul_rad / ppm;
  auto const mass      = density / pow(ppm, 3);

  FluidProperties fluid_properties {
    .particles_per_meter        = ppm,
    .smoothing                  = smoothing,
    .smoothing_pow_2            = std::pow(smoothing, 2),
    .smoothing_pow_6            = std::pow(smoothing, 6),
    .smoothing_pow_9            = std::pow(smoothing, 9),
    .mass                       = mass,
    .f45_pi_smooth_6            = {},
    .mass_pressure_05           = mass * pressure * 0.5,
    .mass_goo                   = mass * goo,
    .transform_density_constant = {}
  };

  fluid_properties.f45_pi_smooth_6            = 45 / (std::numbers::pi * fluid_properties.smoothing_pow_6);
  fluid_properties.transform_density_constant = (315.0 / (pi_times_64 * fluid_properties.smoothing_pow_9)) * mass;

  // std::println("Particles per meter: {}", ppm);
  // std::println("Smoothing length: {}", smoothing);
  // std::println("Particles Mass: {}", mass);

  return fluid_properties;
}

inline math::scalar densityIncrement(FluidProperties const& particles_params, math::scalar const squared_distance) {
  return std::pow(particles_params.smoothing_pow_2 - squared_distance, 3);
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
auto accelerationIncrement(
    FluidProperties const& params, auto particle_i, auto particle_j,
    math::scalar const squared_distance
) {

  math::scalar const distance = squared_distance > min_distance ? std::sqrt(squared_distance) : min_distance_sqrt;
  math::vec3 const left       = (particle_i.position() - particle_j.position()) * params.mass_pressure_05 *
                          (std::pow(params.smoothing - distance, 2) / distance) *
                          (particle_i.density() + particle_j.density() - density_times_2);

  math::vec3 const right         = (particle_j.velocity() - particle_i.velocity()) * params.mass_goo;
  math::scalar const denominator = particle_i.density() * particle_j.density();
  return ((left + right) * params.f45_pi_smooth_6 / denominator);
}

/**
 * Calcula un incremento en las densidades de dos partículas si su distancia
 * al cuadrado es menor que el suavizado al cuadrado proporcionado en los parámetros de partículas.
 * @param particles_params Parámetros de partículas que incluyen información sobre el suavizado.
 * @param particle_i Primera partícula.
 * @param particle_j Segunda partícula.
 */
inline void incrementDensities(FluidProperties const& particles_params, auto particle_i, auto particle_j) {
  if (math::scalar const squared_distance = squaredDistance(particle_i.position(), particle_j.position());
      squared_distance < particles_params.smoothing_pow_2) {
    // Incremento de densidades basado en la distancia
    math::scalar const density_increment = densityIncrement(particles_params, squared_distance);
    particle_i.density() += density_increment;
    particle_j.density() += density_increment;
  }
}

/**
 * Esta función calcula un incremento en las aceleraciones de dos partículas si su distancia
 * al cuadrado es menor que el suavizado al cuadrado proporcionado en los parámetros de partículas.
 * @param properties Parámetros de partículas que incluyen información sobre el suavizado.
 * @param particle_i Primera partícula.
 * @param particle_j Segunda partícula.
 */
inline void incrementAccelerations(FluidProperties const& properties, auto particle_i, auto particle_j) {
  if (auto const squared_distance = squaredDistance(particle_i.position(), particle_j.position());
      squared_distance < properties.smoothing_pow_2) {
    math::vec3 const incr = accelerationIncrement(properties, particle_i, particle_j, squared_distance);
    particle_i.acceleration() += incr;
    particle_j.acceleration() -= incr;
  }
}

} // namespace sim
