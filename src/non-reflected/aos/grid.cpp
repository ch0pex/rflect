#include "grid.hpp"

#include <cmath>
#include <iostream>

namespace sim {

  /**
   * Funcion de reposicionamiento de las particulas, crea un vector de bloques nuevo donde se recolocaran las particulas
   * en funcion de sus nuevas posiciones, de esta forma evitamos reposicionar particulas varias veces.
   * Una vez colocadas todas las particulas se intercambia el vector de bloques antiguo por el nuevo.
   */
  void Grid::repositioning() {
    std::vector<Block> aux(num_blocks_);

    for (auto & block : blocks_) {
      for (auto & particle : block.particles) {
        aux[getBlockIndex(particle.position)].addParticle(particle);
      }
    }
    blocks_ = std::move(aux);
  }

  /**
   * Funcion de calculo de aceleraciones, primero se calculan todas las densidades y posteriormente todas las
   * aceleraciones
   */
  void Grid::calculateAccelerations(FluidProperties const& fluid_properties) {
    // Las densisdes y aceleraciones no son copiadas en el grid auxiliar por lo que ya son 0
    // a = 0
    // d = 0

    for (u64 block_index = 0; block_index < num_blocks_; ++block_index) {
      // Se calculan la densidad y aceleracion entre las particulas de un mismo bloque y bloques adjacentes
      blocks_[block_index].calcDensities(fluid_properties, adjacent_blocks_[block_index], blocks_);
    }

    for (u64 block_index = 0; block_index < num_blocks_; ++block_index) {
      // Se calcula la aceleracion entre las particulas de un mismo bloque y bloques adjacentes
      blocks_[block_index].calcAccelerations(fluid_properties, adjacent_blocks_[block_index], blocks_);
    }
  }

  /**
   * Funcion para procesar las colisiones, recorre todos los bloques que son limites del grid y procesa las colisiones
   * con los limites modificando las aceleraciondes de las particulas
   */
  void Grid::processCollisions() {
    for(auto& [index, limits] : grid_limits_){
      blocks_[index].processCollisions(limits);
    }
  }

  /**
   * Funcion para mover las particulas, por cada una de las particulas se actualiza su posicion, hv y velocidad en
   * funcion de la aceleracion y densidad calculada en los pasos anteriores
   */
  void Grid::moveParticles() {
    for(auto& block : blocks_) {
      block.moveParticles();
    }
  }

  /**
   * Funcion para procesar los limites, recorre todos los bloques que son limites del grid y procesa dichos limites
   * modificando la velocidad, hv y velocidad de las particulas
   */
  void Grid::processLimits() {
    for(auto& [index, limits] : grid_limits_){
      blocks_[index].processLimits(limits);
    }
  }

  /**
   * Obtiene el índice del bloque al que pertenece una partícula
   *
   * @param particle_pos posición de la partícula en coordenadas x,y,z
   * @return índice del bloque al que pertenece la partícula en la cuadrícula.
   */
  u32 Grid::getBlockIndex(math::vec3 const& particle_pos) const {
    math::scalar pos_i = std::floor((particle_pos.x - bottom_limit.x) / block_size_.x);
    math::scalar pos_j = std::floor((particle_pos.y - bottom_limit.y) / block_size_.y);
    math::scalar pos_k = std::floor((particle_pos.z - bottom_limit.z) / block_size_.z);

    if (pos_i < 0) {
      pos_i = 0;
    } else if (pos_i >= static_cast<math::scalar>(grid_size_.x)) {
      pos_i = static_cast<math::scalar>(grid_size_.x) - 1;
    }
    if (pos_j < 0) {
      pos_j = 0;
    } else if (pos_j >= static_cast<math::scalar>(grid_size_.y)) {
      pos_j = static_cast<math::scalar>(grid_size_.y) - 1;
    }
    if (pos_k < 0) {
      pos_k = 0;
    } else if (pos_k >= static_cast<math::scalar>(grid_size_.z)) {
      pos_k = static_cast<math::scalar>(grid_size_.z) - 1;
    }
    return (static_cast<u32>(pos_i) + (static_cast<u32>(pos_j) * grid_size_.x) +
            (static_cast<u32>(pos_k) * grid_size_.x * grid_size_.y));
  }

  /**
   * Calcula los bloques adjacentes para cada bloque pasado por parametro, a su vez determina si dicho bloque pertenece
   * a una de las caras del grid, y lo almacena en dicho caso en grid_limits
   * @param index: indice del bloque
   */
  void Grid::calculateAdjacentAndLimitBlocks(u32 const index) {
    math::Vec3 const block_pos = {
      static_cast<int>(index % grid_size_.x), static_cast<int>(index / grid_size_.x % grid_size_.y),
                                 static_cast<int>(index / (grid_size_.x * grid_size_.y))
    };

    for (int i = -1; i <= 1; ++i) {
      for (int j = -1; j <= 1; ++j) {
        for (int k = -1; k <= 1; ++k) {
          if (i == 0 && j == 0 && k == 0) {
            continue;
          }

          if (math::Vec3 const neighbor_pos = {block_pos.x + i, block_pos.y + j, block_pos.z + k};
              blockInBounds(neighbor_pos)) {
            u64 const neighbor_index = neighbor_pos.x + (neighbor_pos.y * grid_size_.x)
                                          + (neighbor_pos.z * grid_size_.x * grid_size_.y);
            if (neighbor_index > index) {
              adjacent_blocks_[index].push_back(neighbor_index);
            }
          } else {
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
  bool Grid::blockInBounds(const math::Vec3<int>& block_pos) const {
    return block_pos.x >= 0 && static_cast<u64>(block_pos.x) < grid_size_.x
           && block_pos.y >= 0 && static_cast<u64>(block_pos.y) < grid_size_.y &&
           block_pos.z >= 0 && static_cast<u64>(block_pos.z) < grid_size_.z;
  }

 void Grid::addBlockToLimits(u32 const index, const math::Vec3<i32> & neighbor_pos) {
   if(neighbor_pos.x < 0 ) {
     grid_limits_[index].insert(cx0);
   } else if (static_cast<u64>(neighbor_pos.x) >= grid_size_.x) {
     grid_limits_[index].insert(cxn);
   }

   if(neighbor_pos.y < 0 ) {
     grid_limits_[index].insert(cy0);
   } else if (static_cast<u64>(neighbor_pos.y) >= grid_size_.y) {
     grid_limits_[index].insert(cyn);
   }

   if(neighbor_pos.z < 0 ) {
     grid_limits_[index].insert(cz0);
   } else if (static_cast<u64>(neighbor_pos.z) >= grid_size_.z) {
     grid_limits_[index].insert(czn);
   }
 }

  std::vector<Block> & Grid::getBlocks() {
    return blocks_;
  }

}  // namespace sim
