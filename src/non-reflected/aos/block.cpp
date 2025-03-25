#include "block.hpp"

#include "math/math.hpp"

namespace sim {

  /**
   * Agrega una partícula al bloque.
   *
   * @param id Identificador único de la partícula.
   * @param position Posición de la partícula en coordenadas 3D.
   * @param hv Vector de características (hv) de la partícula.
   * @param velocity Vector de velocidad de la partícula.
   */
  void Block::AddParticle(Particle & particle) {
    particle.acceleration = gravity;
    particle.density = 0;
    particles_.push_back(particle);
  }

  std::vector<Particle> & Block::GetParticles() {
    return particles_;
  }

  /**
  * Calcula las densidades de las partículas en el bloque en función de la distancia entre las partículas y el radio de suavizado ('particles_params').
  *
  * Este método permite calcular y actualizar las densidades de las partículas en el bloque, teniendo en cuenta sus interacciones con otras partículas dentro del mismo bloque y con partículas de bloques adyacentes.
  *
  * @param particles_params Los parámetros de las partículas que incluyen el radio de suavizado.
  * @param adjacents Un vector que almacena los índices de bloques adyacentes.
  * @param blocks Un vector de bloques conteniendo partículas.
  * @param block_index El índice del bloque actual en el vector de bloques.
  */
  void Block::CalcDensities(FluidProperties const & particles_params, std::vector<size_t>& adjacents, std::vector<Block>& blocks) {
    for (size_t i = 0; i < particles_.size(); ++i) {
      for (size_t j = i + 1; j < particles_.size(); ++j) { // Evitamos repetir calculos entre particulas inicializando j=i+1
        incrementDensities(particles_params, particles_[i], particles_[j]);
      }
      for(auto& adjacent_index : adjacents) {
          for(auto& particle_j : blocks[adjacent_index].GetParticles()){
            incrementDensities(particles_params, particles_[i], particle_j);
          }
      }
      particles_[i].transformDensity(particles_params);
    }
  }

   /**
   * Calcula las aceleraciones de las partículas en el bloque en función de sus interacciones con otras partículas en el
   * mismo bloque y bloques adyacentes.
   *
   * Este método calcula y actualiza las aceleraciones de las partículas en el bloque, teniendo en cuenta sus
   * interacciones mutuas y con partículas de bloques adyacentes.
   *
   * @param particles_params Los parámetros de las partículas, incluyendo información relevante para el cálculo de
   * aceleraciones.
   * @param adjacents Un vector que almacena los índices de bloques adyacentes.
   * @param blocks Un vector de bloques conteniendo partículas.
   */
  void Block::CalcAccelerations(const FluidProperties& particles_params, std::vector<size_t>& adjacents, std::vector<Block>& blocks) {
    for (size_t i = 0; i < particles_.size(); ++i) {
      for (size_t j = i + 1; j < particles_.size(); ++j) {  // Evitamos repetir calculos entre particulas inicializando j=i+1
        incrementAccelerations(particles_params, particles_[i], particles_[j]);
      }
      for (auto & adjacent_index : adjacents) {
        for (auto & particle_j : blocks[adjacent_index].GetParticles()) {
          incrementAccelerations(particles_params, particles_[i], particle_j);
        }
      }
    }
  }

  /**
  * Procesa y gestiona las colisiones de las partículas dentro del bloque.
  *
  * Este método itera a través de todas las partículas en el bloque y verifica y maneja las colisiones en las
  * dimensiones X, Y y Z. Las colisiones se verifican con respecto a los límites especificados en el conjunto 'limits'.
  *
  * @param limits Un conjunto que almacena los límites del espacio en el que se encuentran las partículas, utilizados
  * para verificar colisiones.
  */
  void Block::ProcessCollisions(std::set<Limits>& limits) {
    for(auto& particle : particles_){
      CollisionsX(particle, limits);
      CollisionsY(particle, limits);
      CollisionsZ(particle, limits);
    }
  }

  /**
  * Mueve las partículas en el bloque en función de su velocidad, aceleración y el paso de tiempo.
  *
  * Este método itera a través de todas las partículas en el bloque y actualiza sus posiciones, velocidades y velocidad
  * de medio paso de tiempo ('hv') de acuerdo con las ecuaciones de movimiento. Utiliza un paso de tiempo definido ('TIME_STEP') y el cuadrado del paso de tiempo ('SQUARED_TIME_STEP').
  */
  void Block::MoveParticles() {
    for (auto& particle : particles_){
      particle.position += particle.hv * time_step + particle.acceleration * squared_time_step;
      particle.velocity = particle.hv + ((particle.acceleration * time_step) / 2);
      particle.hv += particle.acceleration * time_step;
    }
  }

  /**
  * Gestiona colisiones en la dirección X para una partícula específica dentro del bloque.
  * Este método verifica y gestiona las colisiones de una partícula en la dirección X con respecto a los límites
  * definidos en el conjunto 'limits'. Se calcula la posición potencial de la partícula en la dirección X en el próximo
  * paso de tiempo y se compara con los límites para determinar si hay colisiones. Si se detecta una colisión, se ajusta
  * la aceleración de la partícula en función de las fuerzas de colisión y amortiguación.
  * @param particle La partícula que se está evaluando para colisiones en la dirección X.
  * @param limits Un conjunto que almacena los límites del espacio en el que se encuentran las partículas.
  */
  void Block::CollisionsX(Particle& particle, const std::set<Limits>& limits) {
    math::scalar x = 0.0;
    math::scalar x_diff = 0.0;
    // Verifica si el conjunto de límites contiene el límite CX0.
    if (limits.contains(cx0)) {
      x = particle.position.x + particle.hv.x * time_step;
      x_diff = particle_size - (x - bottom_limit.x);
      // Comprueba si la diferencia entre la posición potencial y el límite es suficiente para considerar una colisión.
      if (x_diff > min_collision_diff){
        // Ajusta la aceleración de la partícula en función de la fuerza de colisión y la amortiguación.
        particle.acceleration.x += collision * x_diff - damping * particle.velocity.x;
      }
    }
    // Verifica si el conjunto de límites contiene el límite CXN.
    else if (limits.contains(cxn)) {
      x = particle.position.x + particle.hv.x * time_step;
      x_diff = particle_size - (top_limit.x - x);
      // Comprueba si la diferencia entre la posición potencial y el límite es suficiente para considerar una colisión.
      if (x_diff > min_collision_diff) {
        // Ajusta la aceleración de la partícula en función de la fuerza de colisión y la amortiguación.
        particle.acceleration.x -= collision * x_diff + damping * particle.velocity.x;
      }
    }
  }

  /**
  * Gestiona colisiones en la dirección Y para una partícula específica dentro del bloque.
  *
  * Este método verifica y maneja las colisiones de una partícula en la dirección Y con respecto a los límites definidos
  * en el conjunto 'limits'. Se calcula la posición potencial de la partícula en la dirección Y en el próximo paso de
  * tiempo y se compara con los límites para determinar si hay colisiones. Si se detecta una colisión, se ajusta la
  * aceleración de la partícula en función de las fuerzas de colisión y amortiguación.
  *
  * @param particle La partícula que se está evaluando para colisiones en la dirección Y.
  * @param limits Un conjunto que almacena los límites del espacio en el que se encuentran las partículas.
  */
  void Block::CollisionsY(Particle& particle, const std::set<Limits> & limits) {
    math::scalar y = 0.0;
    math::scalar y_diff = 0.0;

    if (limits.contains(cy0)) {
      y = particle.position.y + particle.hv.y * time_step;
      y_diff = particle_size - (y - bottom_limit.y);
      if (y_diff > min_collision_diff) {
        particle.acceleration.y += collision * y_diff - damping * particle.velocity.y;
      }
    } else if (limits.contains(cyn)) {
      y = particle.position.y + particle.hv.y * time_step;
      y_diff = particle_size - (top_limit.y - y);
      if (y_diff > min_collision_diff) {
        particle.acceleration.y -= collision * y_diff + damping * particle.velocity.y;
      }
    }
  }

  /**
  * Gestiona colisiones en la dirección Z para una partícula específica dentro del bloque.
  *
  * Este método verifica y maneja las colisiones de una partícula en la dirección Z con respecto a los límites definidos
  * en el conjunto 'limits'. Se calcula la posición potencial de la partícula en la dirección Y en el próximo paso de
  * tiempo y se compara con los límites para determinar si hay colisiones. Si se detecta una colisión, se ajusta la
  * aceleración de la partícula en función de las fuerzas de colisión y amortiguación.
  *
  * @param particle La partícula que se está evaluando para colisiones en la dirección Z.
  * @param limits Un conjunto que almacena los límites del espacio en el que se encuentran las partículas.
  */
  void Block::CollisionsZ(Particle& particle, const std::set<Limits> & limits) {
    math::scalar z = 0.0;
    math::scalar z_diff = 0.0;
    if (limits.contains(cz0)) {
      z = particle.position.z + particle.hv.z * time_step;
      z_diff = particle_size - (z - bottom_limit.z);
      if (z_diff > min_collision_diff) {
          particle.acceleration.z += collision * z_diff - damping * particle.velocity.z;
      }
    } else if (limits.contains(czn)) {
      z = particle.position.z + particle.hv.z * time_step;
      z_diff = particle_size - (top_limit.z - z);
      if (z_diff > min_collision_diff) {
            particle.acceleration.z -= collision * z_diff + damping * particle.velocity.z;
      }
    }
  }

  void Block::ProcessLimits(std::set<Limits> & limits) {
    for(auto& particle : particles_){
      LimitsX(particle, limits);
      LimitsY(particle, limits);
      LimitsZ(particle, limits);
    }
  }


  /**
  * Gestiona los límites en la dirección X para una partícula específica dentro del bloque.
  *
  * Este método verifica y maneja los límites en la dirección X para una partícula con respecto a los límites definidos
  * en el conjunto 'limits'. Se calcula la diferencia entre la posición actual de la partícula y los límites en la
  * dirección X. Si la partícula cruza un límite, se ajusta su posición y velocidad, reflejándola en la dirección
  * opuesta.
  *
  * @param particle La partícula que se está evaluando en relación a los límites en la dirección X.
  * @param limits Un conjunto que almacena los límites del espacio en el que se encuentran las partículas.
  */
  void Block::LimitsX(Particle & particle, const std::set<Limits> & limits) {
    math::scalar dx = 0.0;
    if (limits.contains(cx0)) {
      dx = particle.position.x - bottom_limit.x;
      if (dx < 0) {
        particle.position.x = bottom_limit.x - dx;
        particle.velocity.x = -particle.velocity.x;
        particle.hv.x = -particle.hv.x;
      }
    } else if (limits.contains(cxn)) {
      dx = top_limit.x - particle.position.x;
      if (dx < 0) {
        particle.position.x = top_limit.x + dx;
        particle.velocity.x = -particle.velocity.x;
        particle.hv.x = -particle.hv.x;
      }
    }
  }

  /**
  * Gestiona los límites en la dirección Y para una partícula específica dentro del bloque.
  *
  * Este método verifica y maneja los límites en la dirección Y para una partícula con respecto a los límites definidos
  * en el conjunto 'limits'. Se calcula la diferencia entre la posición actual de la partícula y los límites en la
  * dirección Y. Si la partícula cruza un límite, se ajusta su posición y velocidad, reflejándola en la dirección
  * opuesta.
  *
  * @param particle La partícula que se está evaluando en relación a los límites en la dirección Y.
  * @param limits Un conjunto que almacena los límites del espacio en el que se encuentran las partículas.
   */
  void Block::LimitsY(Particle & particle, const std::set<Limits> & limits) {
    math::scalar dy = 0.0;
    if (limits.contains(cy0)) {
      dy = particle.position.y - bottom_limit.y;
      if (dy < 0) {
        particle.position.y = bottom_limit.y - dy;
        particle.velocity.y = -particle.velocity.y;
        particle.hv.y = -particle.hv.y;
      }
    } else if (limits.contains(cyn)) {
      dy = top_limit.y - particle.position.y;
      if (dy < 0) {
        particle.position.y = top_limit.y + dy;
        particle.velocity.y = -particle.velocity.y;
        particle.hv.y = -particle.hv.y;
      }
    }
  }

  /**
  * Gestiona los límites en la dirección X para una partícula específica dentro del bloque.
  *
  * Este método verifica y maneja los límites en la dirección Z para una partícula con respecto a los límites definidos
  * en el conjunto 'limits'. Se calcula la diferencia entre la posición actual de la partícula y los límites en la
  * dirección Z. Si la partícula cruza un límite, se ajusta su posición y velocidad, reflejándola en la dirección
  * opuesta.
  *
  * @param particle La partícula que se está evaluando en relación a los límites en la dirección Z.
  * @param limits Un conjunto que almacena los límites del espacio en el que se encuentran las partículas.
   */
  void Block::LimitsZ(Particle & particle, const std::set<Limits> & limits) {
    math::scalar dz = 0.0;
    if (limits.contains(cz0)) {
      dz = particle.position.z - bottom_limit.z;
      if (dz < 0) {
        particle.position.z = bottom_limit.z - dz;
        particle.velocity.z = -particle.velocity.z;
        particle.hv.z = -particle.hv.z;
      }
    } else if (limits.contains(czn)) {
      dz = top_limit.z - particle.position.z;
      if (dz < 0) {
        particle.position.z = top_limit.z + dz;
        particle.velocity.z = -particle.velocity.z;
        particle.hv.z = -particle.hv.z;
      }
    }
  }


}  // namespace sim
