#include "grid.hpp"

#include <cmath>
#include <iostream>

namespace sim {
/**
 * Constructor de Grid, a partir de los parametros se inicializa el grid colocando todas las particulas en los
 * respectivos bloques asi como tambien se calculan los bloques adjacentes y los bloques limites. Por ultimo se emite
 * un mensaje con los parametros resultantes de la inicializacion de la simulacion
 * @param np: numero de partiuclas
 * @param ppm: partiuclas por metro
 * @param particles: vector de particulas que contiene el grid
 */
Grid::Grid(int np, math::scalar ppm, std::vector<Particle>& particles) :
  num_particles(np), particles_param_(ppm),
  grid_size_(
      {static_cast<u64>(std::floor((top_limit.x - bottom_limit.x) / particles_param_.smoothing)),
       static_cast<u64>(std::floor((top_limit.y - bottom_limit.y) / particles_param_.smoothing)),
       static_cast<u64>(std::floor((top_limit.z - bottom_limit.z) / particles_param_.smoothing))}
  ),
  block_size_({
    (top_limit.x - bottom_limit.x) / static_cast<math::scalar>(grid_size_.x),
    (top_limit.y - bottom_limit.y) / static_cast<math::scalar>(grid_size_.y),
    (top_limit.z - bottom_limit.z) / static_cast<math::scalar>(grid_size_.z),
  }),
  num_blocks_(grid_size_.x * grid_size_.y * grid_size_.z), blocks_(num_blocks_), adjacent_blocks_(num_blocks_) {
  for (auto& particle: particles) {
    size_t const block_index =
        getBlockIndex(particle.position); // Coger la posicion de la particula comprobar en que bloque le toca
    blocks_[block_index].addParticle(particle); // Anadir la particula a dicho bloque
  }

  // Se calculan los bloques adjacentes para cada bloque y tambien determina que bloques son limites Cx, Cy, Cz
  for (size_t i = 0; i < num_blocks_; ++i) {
    calculateAdjacentAndLimitBlocks(i);
  }
  initMessage();
}

/**
 * Funcion de reposicionamiento de las particulas, crea un vector de bloques nuevo donde se recolocaran las particulas
 * en funcion de sus nuevas posiciones, de esta forma evitamos reposicionar particulas varias veces.
 * Una vez colocadas todas las particulas se intercambia el vector de bloques antiguo por el nuevo.
 */
void Grid::repositioning() {
  std::vector<Block> aux(num_blocks_);

  for (auto& block: blocks_) {
    for (auto& particle: block.particles) {
      aux[getBlockIndex(particle.position)].addParticle(particle);
    }
  }
  blocks_ = std::move(aux);
}

/**
 * Funcion de calculo de aceleraciones, primero se calculan todas las densidades y posteriormente todas las
 * aceleraciones
 */
void Grid::calculateAccelerations() {
  // Las densisdes y aceleraciones no son copiadas en el grid auxiliar por lo que ya son 0
  // a = 0
  // d = 0

  for (size_t block_index = 0; block_index < num_blocks_; ++block_index) {
    // Se calculan la densidad y aceleracion entre las particulas de un mismo bloque y bloques adjacentes
    blocks_[block_index].calcDensities(particles_param_, adjacent_blocks_[block_index], blocks_);
  }

  for (size_t block_index = 0; block_index < num_blocks_; ++block_index) {
    // Se calcula la aceleracion entre las particulas de un mismo bloque y bloques adjacentes
    blocks_[block_index].calcAccelerations(particles_param_, adjacent_blocks_[block_index], blocks_);
  }
}

/**
 * Funcion para procesar las colisiones, recorre todos los bloques que son limites del grid y procesa las colisiones
 * con los limites modificando las aceleraciondes de las particulas
 */
void Grid::processCollisions() {
  for (auto& [index, limits]: grid_limits_) {
    blocks_[index].processCollisions(limits);
  }
}

/**
 * Funcion para mover las particulas, por cada una de las particulas se actualiza su posicion, hv y velocidad en
 * funcion de la aceleracion y densidad calculada en los pasos anteriores
 */
void Grid::moveParticles() {
  for (auto& block: blocks_) {
    block.moveParticles();
  }
}

/**
 * Funcion para procesar los limites, recorre todos los bloques que son limites del grid y procesa dichos limites
 * modificando la velocidad, hv y velocidad de las particulas
 */
void Grid::processLimits() {
  for (auto& [index, limits]: grid_limits_) {
    blocks_[index].processLimits(limits);
  }
}

/**
 * Obtiene el índice del bloque al que pertenece una partícula
 *
 * @param particle_pos posición de la partícula en coordenadas x,y,z
 * @return índice del bloque al que pertenece la partícula en la cuadrícula.
 */
size_t Grid::getBlockIndex(math::vec3 const particle_pos) const {
  // i,j,k posicion del bloque en la malla --> pasarlo al indice del bloque
  math::scalar pos_i = std::floor((particle_pos.x - bottom_limit.x) / block_size_.x);
  math::scalar pos_j = std::floor((particle_pos.y - bottom_limit.y) / block_size_.y);
  math::scalar pos_k = std::floor((particle_pos.z - bottom_limit.z) / block_size_.z);

  // Asegura que las coordenadas estén dentro de los límites de la cuadrícula
  if (pos_i < 0) {
    pos_i = 0;
  }
  else if (pos_i >= static_cast<math::scalar>(grid_size_.x)) {
    pos_i = static_cast<math::scalar>(grid_size_.x) - 1;
  }
  if (pos_j < 0) {
    pos_j = 0;
  }
  else if (pos_j >= static_cast<math::scalar>(grid_size_.y)) {
    pos_j = static_cast<math::scalar>(grid_size_.y) - 1;
  }
  if (pos_k < 0) {
    pos_k = 0;
  }
  else if (pos_k >= static_cast<math::scalar>(grid_size_.z)) {
    pos_k = static_cast<math::scalar>(grid_size_.z) - 1;
  }
  return (
      static_cast<size_t>(pos_i) + (static_cast<size_t>(pos_j) * grid_size_.x) +
      (static_cast<size_t>(pos_k) * grid_size_.x * grid_size_.y)
  );
}

void Grid::initMessage() const {
  std::cout << "Number of particles: " << num_particles << "\n";
  std::cout << "Particles per meter: " << particles_param_.particles_per_meter << "\n";
  std::cout << "Smoothing length: " << particles_param_.smoothing << "\n";
  std::cout << "Particles Mass: " << particles_param_.mass << "\n";
  std::cout << "Grid size: " << grid_size_ << "\n";
  std::cout << "Number of blocks: " << num_blocks_ << "\n";
  std::cout << "Block size: " << block_size_ << "\n";
}

/**
 * Calcula los bloques adjacentes para cada bloque pasado por parametro, a su vez determina si dicho bloque pertenece
 * a una de las caras del grid, y lo almacena en dicho caso en grid_limits
 * @param index: indice del bloque
 */
void Grid::calculateAdjacentAndLimitBlocks(size_t index) {
  math::Vec3<i32> const block_pos = {
    static_cast<i32>(index % grid_size_.x), //
    static_cast<i32>(index / grid_size_.x % grid_size_.y), //
    static_cast<i32>(index / (grid_size_.x * grid_size_.y))
  };

  for (int i = -1; i <= 1; ++i) {
    for (int j = -1; j <= 1; ++j) {
      for (int k = -1; k <= 1; ++k) {
        if (i == 0 && j == 0 && k == 0) {
          continue;
        }

        if (math::Vec3 const neighbor_pos = {block_pos.x + i, block_pos.y + j, block_pos.z + k};
            blockInBounds(neighbor_pos)) {
          size_t const neighbor_index =
              neighbor_pos.x + neighbor_pos.y * grid_size_.x + neighbor_pos.z * grid_size_.x * grid_size_.y;

          // Solo se anaden a la lista de vecinos los bloques con mas indice para asi no repetir calculos
          if (neighbor_index > index) {
            adjacent_blocks_[index].push_back(neighbor_index);
          }
        }
        else {
          addBlockToLimits(index, neighbor_pos);
        }
      }
    }
  }
}

/**
 * Funcion utilizada por CalculateAdjacentAndLimitBlocks para determinar si la posicion pasada por parametro
 * corresponde a un bloque del grid
 * @param block_pos
 * @return
 */
bool Grid::blockInBounds(math::Vec3<i32> const block_pos) const {
  return block_pos.x >= 0 && static_cast<size_t>(block_pos.x) < grid_size_.x && block_pos.y >= 0 &&
         static_cast<size_t>(block_pos.y) < grid_size_.y && block_pos.z >= 0 &&
         static_cast<size_t>(block_pos.z) < grid_size_.z;
}

void Grid::addBlockToLimits(u64 const index, math::Vec3<i32> const neighbor_pos) {
  if (neighbor_pos.x < 0) {
    grid_limits_[index].insert(CX0);
  }
  else if (static_cast<size_t>(neighbor_pos.x) >= grid_size_.x) {
    grid_limits_[index].insert(CXN);
  }

  if (neighbor_pos.y < 0) {
    grid_limits_[index].insert(CY0);
  }
  else if (static_cast<size_t>(neighbor_pos.y) >= grid_size_.y) {
    grid_limits_[index].insert(CYN);
  }

  if (neighbor_pos.z < 0) {
    grid_limits_[index].insert(CZ0);
  }
  else if (static_cast<size_t>(neighbor_pos.z) >= grid_size_.z) {
    grid_limits_[index].insert(CZN);
  }
}

} // namespace sim
