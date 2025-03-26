#include "block.hpp"
#include <gtest/gtest.h>
#include <set>

namespace sim {
  // Definición de constantes para la configuración inicial de partículas
constexpr math::vec3 kInitialPositionOffset = math::vec3(-0.06649009138345718, -0.07730470597743988, -0.059135954827070236);
constexpr math::vec3 kHvVector = math::vec3(4.0, 5.0, 6.0);
constexpr math::vec3 kVelocityIncrement = math::vec3(1.0, 8.0, 9.0);
constexpr math::scalar def_position = 100.0;

  // Test para probar colisiones
  TEST(Block, TestCollisionZ) {
    // Creación de un bloque opcional y un conjunto de límites
    Block block;
    std::set<Limits> limits = {CZN};
    constexpr math::scalar expected_acceleration = -2999388.0000000005;

    // Configuración de una partícula específica para el test
    constexpr size_t particle_id = 1;
    math::vec3 position = kInitialPositionOffset;
    math::vec3 vec_hv = kHvVector;
    math::vec3 velocity = kVelocityIncrement;
    Particle particle(particle_id, position, vec_hv, velocity);

    // Se establece una posición inusual para provocar una colisión en el eje z
    particle.position.z = def_position;

    // Se añade la partícula al bloque y se procesan las colisiones
    block.AddParticle(particle);
    block.ProcessCollisions(limits);
    const math::scalar new_acceleration = block.particles[0].acceleration.z;
    ASSERT_EQ(new_acceleration, expected_acceleration);
  }

  TEST(Block, TestCollisionX) {
    // Creación de un bloque opcional y un conjunto de límites
    Block block;
    std::set<Limits> limits = {CXN};
    constexpr math::scalar expected_acceleration = -2998304.0000000005;
    // Configuración de una partícula específica para el test
    constexpr size_t particle_id = 1;
    math::vec3 position = kInitialPositionOffset;
    math::vec3 vec_hv = kHvVector;
    math::vec3 velocity = kVelocityIncrement;
    Particle particle(particle_id, position, vec_hv, velocity);

    // Se establece una posición inusual para provocar una colisión en el eje x
    particle.position.x = def_position;

    // Se añade la partícula al bloque y se procesan las colisiones
    block.AddParticle(particle);
    block.ProcessCollisions(limits);
    const math::scalar new_acceleration = block.particles[0].acceleration.x;
    ASSERT_EQ(new_acceleration, expected_acceleration);
  }

  TEST(Block, TestCollisionY) {
    // Creación de un bloque opcional y un conjunto de límites
    Block block;
    std::set<Limits> limits = {CYN};
    constexpr math::scalar expected_acceleration = -2998189.8000000003;
    // Configuración de una partícula específica para el test
    constexpr size_t particle_id = 1;
    math::vec3 position = kInitialPositionOffset;
    math::vec3 vec_hv = kHvVector;
    math::vec3 velocity = kVelocityIncrement;
    Particle particle(particle_id, position, vec_hv, velocity);

    // Se establece una posición inusual para provocar una colisión en el eje Y
    particle.position.y = def_position;

    // Se añade la partícula al bloque y se procesan las colisiones
    block.AddParticle(particle);
    block.ProcessCollisions(limits);
    const math::scalar new_acceleration = block.particles[0].acceleration.y;
    ASSERT_EQ(new_acceleration, expected_acceleration);
  }

  // Test para probar los límites del bloque
  TEST(Block, TestLimitsZ) {
    // Creación de un bloque opcional y un conjunto de límites
    Block block;
    std::set<Limits> limits = {CZN};
    constexpr math::scalar position_expected = -99.87; // Posicion en Z esperada despues de ProcessLimits

    // Configuración de una partícula específica para el test
    constexpr size_t particle_id = 1;
    math::vec3 position = kInitialPositionOffset;
    math::vec3 vec_hv = kHvVector;
    math::vec3 velocity = kVelocityIncrement;
    Particle particle(particle_id, position, vec_hv, velocity);

    // Se establece una posición inusual para provocar una colisión en el eje z
    particle.position.z = def_position;

    // Se añade la partícula al bloque y se procesan las colisiones
    block.AddParticle(particle);
    block.ProcessLimits(limits);
    const math::scalar new_position = block.particles[0].position.z;
    ASSERT_EQ(new_position, position_expected);
  }

  TEST(Block, TestLimitsX) {
    // Creación de un bloque opcional y un conjunto de límites
    Block block;
    std::set<Limits> limits = {CXN};
    constexpr math::scalar position_expected = -99.87; // Posicion en X esperada despues de ProcessLimits
    // Configuración de una partícula específica para el test
    constexpr size_t particle_id = 1;
    math::vec3 position = kInitialPositionOffset;
    math::vec3 vec_hv = kHvVector;
    math::vec3 velocity = kVelocityIncrement;
    Particle particle(particle_id, position, vec_hv, velocity);

    // Se establece una posición inusual para provocar una colisión en el eje x
    particle.position.x = def_position;

    // Se añade la partícula al bloque y se procesan las colisiones
    block.addParticle(particle);
    block.processLimits(limits);
    const math::scalar new_position = block.particles[0].position.x;
    ASSERT_EQ(new_position, position_expected);
  }

  TEST(Block, TestLimitsY) {
    // Creación de un bloque opcional y un conjunto de límites
    Block block;
    std::set<Limits> limits = {CYN};
    constexpr math::scalar position_expected = -99.800000000000011; // Posicion en y esperada despues de ProcessLimits
    // Configuración de una partícula específica para el test
    constexpr size_t particle_id = 1;
    math::vec3 position = kInitialPositionOffset;
    math::vec3 vec_hv = kHvVector;
    math::vec3 velocity = kVelocityIncrement;
    Particle particle(particle_id, position, vec_hv, velocity);

    // Se establece una posición inusual para provocar una colisión en el eje Y
    particle.position.y = def_position;

    // Se añade la partícula al bloque y se procesan las colisiones
    block.AddParticle(particle);
    block.ProcessLimits(limits);
    const math::scalar new_position = block.particles[0].position.y;
    ASSERT_EQ(new_position, position_expected);
  }
} // namespace sim
