#pragma once

#include "math/math.hpp"
#include "utils/constants.hpp"
#include "utils/primitive_types.hpp"

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
  u32 id {0};
  math::vec3 position;
  math::vec3 hv;
  math::vec3 velocity;
  math::vec3 acceleration {gravity};
  math::scalar density {0};
};

void transformDensity(FluidProperties const& particles_params, auto&& particle) {
  auto [_, _, _, _, _, density] = particle;
  density = (density + particles_params.smoothing_pow_6) * particles_params.transform_density_constant;
}

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
 */
auto accelerationIncrement(
    FluidProperties const& params, auto&& particle_i, auto&& particle_j, math::scalar const squared_distance
) {

  auto [_, position_i, _, velocity_i, _, density_i] = particle_i;
  auto [_, position_j, _, velocity_j, _, density_j] = particle_j;

  math::scalar const distance = squared_distance > min_distance ? std::sqrt(squared_distance) : min_distance_sqrt;
  math::vec3 const left       = (position_i - position_j) * params.mass_pressure_05 *
                          (std::pow(params.smoothing - distance, 2) / distance) *
                          (density_i + density_j - density_times_2);

  math::vec3 const right         = (velocity_j - velocity_i) * params.mass_goo;
  math::scalar const denominator = density_i * density_j;
  return ((left + right) * params.f45_pi_smooth_6 / denominator);
}

/**
 * Calcula un incremento en las densidades de dos partículas si su distancia
 * al cuadrado es menor que el suavizado al cuadrado proporcionado en los parámetros de partículas.
 */
void incrementDensities(FluidProperties const& properties, auto&& particle_i, auto&& particle_j) {
  auto [_, position_i, _, velocity_i, _, density_i] = particle_i;
  auto [_, position_j, _, velocity_j, _, density_j] = particle_j;

  if (math::scalar const squared_distance = squaredDistance(position_i, position_j);
      squared_distance < properties.smoothing_pow_2) {
    // Incremento de densidades basado en la distancia
    math::scalar const density_increment = densityIncrement(properties, squared_distance);
    density_i += density_increment;
    density_j += density_increment;
  }
}

/**
 * Esta función calcula un incremento en las aceleraciones de dos partículas si su distancia
 * al cuadrado es menor que el suavizado al cuadrado proporcionado en los parámetros de partículas.
 */
void incrementAccelerations(FluidProperties const& properties, auto&& particle_i, auto&& particle_j) {
  auto [_, position_i, _, _, acceleration_i, _] = particle_i;
  auto [_, position_j, _, _, acceleration_j, _] = particle_j;

  if (auto const squared_distance = squaredDistance(position_i, position_j);
      squared_distance < properties.smoothing_pow_2) {
    math::vec3 const incr = accelerationIncrement(properties, particle_i, particle_j, squared_distance);
    acceleration_i += incr;
    acceleration_j -= incr;
  }
}

} // namespace sim
