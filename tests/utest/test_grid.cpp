#include "sim/grid/grid.hpp"
#include "sim/grid/particle.hpp"

#include <gtest/gtest.h>

namespace sim {
  // setting the environment variables that will be used in testing
constexpr size_t kParticleCount = 10;
constexpr size_t kBlockSize = 10;
constexpr double kParticlesPerMeter = 100.1;

constexpr vec3d kInitialPositionOffset = vec3d(-0.06649009138345718, -0.07730470597743988, -0.059135954827070236);
constexpr vec3d kHvVector = vec3d(4.0, 5.0, 6.0);
constexpr vec3d kVelocityIncrement = vec3d(1.0, 8.0, 9.0);
constexpr double kDensityMultiplier = 0.8571428571428572;
constexpr double asserted_mass = 0.00099700599001497931;

  TEST(GridTest, TestCalculateAccelerations) {
    // Initialize the Grid with specific parameters and particles
    std::optional<Grid> grid;
    std::vector<Particle> particles;
    // Starting with a particle that is in block 1
    for (size_t i = 1; i <= kParticleCount; ++i) {
      const double factor = kDensityMultiplier * static_cast<double>(i);
      const size_t particle_id = i;
      vec3d position = kInitialPositionOffset - vec3d(factor, 0.0, 0.0);
      vec3d vec_hv = kHvVector;
      vec3d velocity = kVelocityIncrement * static_cast<double>(i);

      particles.emplace_back(particle_id, position, vec_hv, velocity);
    }
    grid.emplace(kBlockSize, kParticlesPerMeter, particles);

    grid->CalculateAccelerations();

    const double density = grid->GetBlocks().at(0).GetParticles().at(0).density;

    EXPECT_EQ(321.71521414997841, density);
  }

  TEST(GridTest, TestProcessCollisions) {
    // Initialize the Grid with specific parameters and particles
    std::optional<Grid> grid;
    std::vector<Particle> particles;
    // Starting with a particle that is in block 1
    for (size_t i = 1; i <= kParticleCount; ++i) {
      const double factor = kDensityMultiplier * static_cast<double>(i);
      const size_t particle_id = i;
      vec3d position = kInitialPositionOffset - vec3d(factor, 0.0, 0.0);
      vec3d vec_hv = kHvVector;
      vec3d velocity = kVelocityIncrement * static_cast<double>(i);

      particles.emplace_back(particle_id, position, vec_hv, velocity);
    }
    grid.emplace(kBlockSize, kParticlesPerMeter, particles);

    grid->ProcessCollisions();

    const double acceleration_x = grid->GetBlocks()[0].GetParticles()[0].acceleration.x;

    EXPECT_EQ(25516.988455789433, acceleration_x);
  }

  TEST(GridTest, TestMoveParticles) {
    // Initialize the Grid with specific parameters and particles
    std::optional<Grid> grid;
    std::vector<Particle> particles;
    // Starting with a particle that is in block 1
    for (size_t i = 1; i <= kParticleCount; ++i) {
      const double factor = kDensityMultiplier * static_cast<double>(i);
      const size_t particle_id = i;
      vec3d position = kInitialPositionOffset - vec3d(factor, 0.0, 0.0);
      vec3d vec_hv = kHvVector;
      vec3d velocity = kVelocityIncrement * static_cast<double>(i);

      particles.emplace_back(particle_id, position, vec_hv, velocity);
    }
    grid.emplace(kBlockSize, kParticlesPerMeter, particles);

    grid->MoveParticles();

    const double position_x = grid->GetBlocks()[0].GetParticles()[0].position.x;

    EXPECT_EQ(-0.91963294852631439, position_x);
  }

  TEST(GridTest, TestProcessLimits) {
    // Initialize the Grid with specific parameters and particles
    std::optional<Grid> grid;
    std::vector<Particle> particles;
    // Starting with a particle that is in block 1
    for (size_t i = 1; i <= kParticleCount; ++i) {
      const double factor = kDensityMultiplier * static_cast<double>(i);
      const size_t particle_id = i;
      vec3d position = kInitialPositionOffset - vec3d(factor, 0.0, 0.0);
      vec3d vec_hv = kHvVector;
      vec3d velocity = kVelocityIncrement * static_cast<double>(i);

      particles.emplace_back(particle_id, position, vec_hv, velocity);
    }
    grid.emplace(kBlockSize, kParticlesPerMeter, particles);

    grid->ProcessLimits();

    const vec3d new_position = grid->GetBlocks()[0].GetParticles()[0].position;

    EXPECT_EQ(0.7936329485263145, new_position.x);
  }

  TEST(GridTest, TestNumParticles) {
    // Initialize the Grid with specific parameters and particles
    std::optional<Grid> grid;
    std::vector<Particle> particles;
    // Starting with a particle that is in block 1
    for (size_t i = 1; i <= kParticleCount; ++i) {
      const double factor = kDensityMultiplier * static_cast<double>(i);
      const size_t particle_id = i;
      vec3d position = kInitialPositionOffset - vec3d(factor, 0.0, 0.0);
      vec3d vec_hv = kHvVector;
      vec3d velocity = kVelocityIncrement * static_cast<double>(i);

      particles.emplace_back(particle_id, position, vec_hv, velocity);
    }
    grid.emplace(kBlockSize, kParticlesPerMeter, particles);

    const int num_particles = grid->GetNumParticles();

    EXPECT_EQ(num_particles, kParticleCount);
  }

  TEST(GridTest, TestGetParticlesPerMeter) {
    // Initialize the Grid with specific parameters and particles
    std::optional<Grid> grid;
    std::vector<Particle> particles;
    // Starting with a particle that is in block 1
    for (size_t i = 1; i <= kParticleCount; ++i) {
      const double factor = kDensityMultiplier * static_cast<double>(i);
      const size_t particle_id = i;
      vec3d position = kInitialPositionOffset - vec3d(factor, 0.0, 0.0);
      vec3d vec_hv = kHvVector;
      vec3d velocity = kVelocityIncrement * static_cast<double>(i);

      particles.emplace_back(particle_id, position, vec_hv, velocity);
    }
    grid.emplace(kBlockSize, kParticlesPerMeter, particles);

    const double num_particles = grid->GetParticlesPerMeter();

    EXPECT_EQ(num_particles, kParticlesPerMeter);
  }

  TEST(GridTest, TestParticlesData) {
    // Initialize the Grid with specific parameters and particles
    std::optional<Grid> grid;
    std::vector<Particle> particles;
    // Starting with a particle that is in block 1
    for (size_t i = 1; i <= kParticleCount; ++i) {
      const double factor = kDensityMultiplier * static_cast<double>(i);
      const size_t particle_id = i;
      vec3d position = kInitialPositionOffset - vec3d(factor, 0.0, 0.0);
      vec3d vec_hv = kHvVector;
      vec3d velocity = kVelocityIncrement * static_cast<double>(i);

      particles.emplace_back(particle_id, position, vec_hv, velocity);
    }
    grid.emplace(kBlockSize, kParticlesPerMeter, particles);

    const ParticlesData& particles_data = grid->GetParameters();

    EXPECT_EQ(particles_data.mass, asserted_mass);
  }
}