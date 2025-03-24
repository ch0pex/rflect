#include "particle.hpp"

#include <cmath>

#include "utils/constants.hpp"
#include "math/math.hpp"

namespace sim {
  /**
   * Constructor de ParticlesData a partir de ppm se calcula la masa de la particula el
   * smoothing y sus respectivas potencias. De esta forma ganamos mucho rendimiento ahorrando tener que calcular
   * h^2, h^6 y h^9 en cada iteracion.
   * @param ppm
   */
  ParticlesData::ParticlesData(const double ppm)
      : particles_per_meter(ppm), smoothing(MUL_RAD / ppm),
        smoothing_pow_2(pow(smoothing, 2)),
        smoothing_pow_6(pow(smoothing_pow_2, 3)),
        smoothing_pow_9(pow(smoothing, 9)),
        mass(DENSITY / pow(ppm, 3)),
        f45_pi_smooth_6(45 / (PI * smoothing_pow_6)),
        mass_pressure_05(mass * PRESSURE * 0.5),
        mass_goo(GOO * mass)

  { }

  /**
   * Constructor de particle se pasa por parametro el ide la posicion, hvy velocidad.
   * La acelearcion y la densidad se inicializan a 0
   * @param other
   */
  Particle::Particle(size_t _id, vec3d& _position, vec3d& _hv, vec3d& _velocity)
    : id(_id),
      position(_position),
      hv(_hv),
      velocity(_velocity),
      acceleration(GRAVITY),
      density(0) { }
  /**
   * Constructor copia de particle, cuando una particula se copia la aceleracion y la gravedad se pone a 0
   * @param other
   */
  Particle::Particle(sim::Particle const & other)
    : id(other.id),
      position(other.position),
      hv(other.hv),
      velocity(other.velocity),
      acceleration(GRAVITY),
      density(0) { }


  void Particle::TransformDenisty(const ParticlesData& particles_params) {
    density = (density + particles_params.smoothing_pow_6) *
              (315.0 /(64.0 * PI * particles_params.smoothing_pow_9)) * particles_params.mass;
  }

  /**
   * Calcula un incremento en las densidades de dos partículas si su distancia
   * al cuadrado es menor que el suavizado al cuadrado proporcionado en los parámetros de partículas.
   * @param particles_params Parámetros de partículas que incluyen información sobre el suavizado.
   * @param particle_i Primera partícula.
   * @param particle_j Segunda partícula.
   */
  void Particle::IncrementDensities(const ParticlesData& particles_params, Particle& particle_i, Particle& particle_j) {
    const double squared_distance = vec3d::SquaredDistance(particle_i.position, particle_j.position);
    if (squared_distance < particles_params.smoothing_pow_2){
      // Incremento de densidades basado en la distancia
      const double density_increment = math::DensityIncrement(particles_params, squared_distance);
      particle_i.density += density_increment;
      particle_j.density += density_increment;
    }
  }

  /**
   * Esta función calcula un incremento en las aceleraciones de dos partículas si su distancia
   * al cuadrado es menor que el suavizado al cuadrado proporcionado en los parámetros de partículas.
   * @param particles_params Parámetros de partículas que incluyen información sobre el suavizado.
   * @param particle_i Primera partícula.
   * @param particle_j Segunda partícula.
   */
  void Particle::IncrementAccelerations(const ParticlesData & particles_params, Particle & particle_i, Particle & particle_j) {
    const double squared_distance = vec3d::SquaredDistance(particle_i.position, particle_j.position);
    if (squared_distance < particles_params.smoothing_pow_2){
      const vec3d accleration_increment = math::AccelerationIncrement(particles_params, particle_i, particle_j, squared_distance);
      particle_i.acceleration += accleration_increment;
      particle_j.acceleration -= accleration_increment;
    }
  }
}  // namespace sim
