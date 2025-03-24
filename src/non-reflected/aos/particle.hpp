#ifndef FLUID_PARTICLE_HPP
#define FLUID_PARTICLE_HPP

#include "math/vector.hpp"

#include "utils/constants.hpp"
#include "math/math.hpp"

namespace sim {

  struct ParticlesData {
    ParticlesData(double ppm);
    double particles_per_meter;
    double smoothing;
    double smoothing_pow_2;
    double smoothing_pow_6;
    double smoothing_pow_9;
    double mass;
    double f45_pi_smooth_6;
    double mass_pressure_05;
    double mass_goo;
  };

  struct Particle {
      Particle(size_t _id, math::vec3 & _position, math::vec3 & _hv, math::vec3 & _velocity);

      Particle(Particle const &);

      void TransformDenisty(const ParticlesData& particles_params);

      static void IncrementDensities(const ParticlesData& particles_params, Particle& particle_i, Particle& particle_j);

      static void IncrementAccelerations(const ParticlesData& particles_params, Particle& particle_i, Particle& particle_j);

      size_t id;

      math::vec3 position;

      math::vec3 hv;

      math::vec3 velocity;

      math::vec3 acceleration;

      double density;
  };


  inline double densityIncrement(const ParticlesData& particles_params, const double squared_distance) {
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
  inline math::vec3 accelerationIncrement(ParticlesData const & params, Particle const & particle_i, Particle const & particle_j, double squared_distance) {
    double const distance = sqrt(std::max(squared_distance, MINIMUN_DISTANCE));
    math::vec3 const left = (particle_i.position - particle_j.position) *
                       params.mass_pressure_05 *
                       (std::pow(params.smoothing - distance, 2) / distance) *
                       (particle_i.density + particle_j.density - 2 * DENSITY);

    math::vec3 const right = (particle_j.velocity - particle_i.velocity) * params.mass_goo;
    double const denominator = particle_i.density * particle_j.density;
    return ((left + right) * params.f45_pi_smooth_6 / denominator);
  }

}  // namespace sim

#endif  // FLUID_PARTICLE_HPP
