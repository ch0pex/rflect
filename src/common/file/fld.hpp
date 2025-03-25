#pragma once

#include "particle.hpp"
#include "simulator.hpp"
#include "utils/error.hpp"

#include <flat_map>
#include <fstream>
#include <span>
#include <vector>

namespace sim {

namespace detail {

inline err::expected<math::scalar> read_header(std::ifstream& file) {
  f32 particles_per_meter = 0.0F;
  i32 np                  = 0.0F;
  i32 file_length         = 0;

  try {
    file.seekg(0, std::ifstream::end);
    file_length = file.tellg();
    file.seekg(0, std::ifstream::beg);
    file.read(reinterpret_cast<char*>(&particles_per_meter), sizeof(particles_per_meter)); // NOLINT
    file.read(reinterpret_cast<char*>(&np), sizeof(np)); // NOLINT
  }
  catch (std::ifstream::failure const& e) {
    return err::unexpected("Exception raised reading header");
  }

  if (np <= 0) {
    std::cout << "Invalid number of particles\n";
    return err::unexpected(std::format("Invalid number of particles: {}", np));
  }

  if (i32 const particles = (file_length - header_size) / sizeof(f32) / particle_components; particles != np) {
    return err::unexpected(
        std::format("Number of particles is not coherent with header\nExpected: {}\n Found: {}\n", np, particles)
    );
  }
  return particles_per_meter;
}

inline auto read_particles(std::ifstream& file) -> std::vector<Particle> {
  std::vector<Particle> particles;

  try {
    file.seekg(0, std::ifstream::end);
    u64 const file_length = file.tellg();
    file.seekg(0, std::ifstream::beg);

    std::vector<f32> tmp((file_length - header_size) / sizeof(f32));
    particles.reserve((file_length - header_size) / particle_components);
    file.seekg(header_size, std::ifstream::beg);
    file.read(reinterpret_cast<char*>(tmp.data()), file_length - header_size); // NOLINT
    for (size_t i = 0; i < tmp.size(); i += particle_components) {
      math::vec3 position = {tmp[i], tmp[i + 1], tmp[i + 2]};
      math::vec3 vec_hv   = {tmp[i + 3], tmp[i + 4], tmp[i + 5]};
      math::vec3 velocity = {tmp[i + 6], tmp[i + 7], tmp[i + 8]};
      particles.emplace_back(i / particle_components, position, vec_hv, velocity);
    }
  }
  catch (std::ifstream::failure const& e) {
    particles.clear();
    return particles;
  }

  return particles;
}

inline void write_header(std::ofstream& file, int np, math::scalar const ppm) {
  auto particle_per_meter = static_cast<float>(ppm);
  file.write(reinterpret_cast<char*>(&particle_per_meter), sizeof(f32)); // NOLINT
  file.write(reinterpret_cast<char*>(&np), sizeof(i32)); // NOLINT
}

inline void write_particles(std::ofstream& file, std::span<Particle const* const> const particles) {
  std::array<f32, particle_components> tmp_values {};
  for (auto const& particle: particles) {
    tmp_values[0] = particle->position.x;
    tmp_values[1] = particle->position.y;
    tmp_values[2] = particle->position.z;
    tmp_values[3] = particle->hv.x;
    tmp_values[4] = particle->hv.y;
    tmp_values[5] = particle->hv.z;
    tmp_values[6] = particle->velocity.x;
    tmp_values[7] = particle->velocity.y;
    tmp_values[8] = particle->velocity.z;
    file.write(reinterpret_cast<char*>(tmp_values.data()), sizeof(float) * particle_components); // NOLINT
  }
}
} // namespace detail


inline auto read_input_file = [](Arguments const& arguments) -> err::expected<Simulation> {
  std::ifstream file {arguments.input_file, std::ios::binary};
  auto const particles_per_meter = detail::read_header(file);

  if (not particles_per_meter) {
    return err::unexpected(particles_per_meter.error().what());
  }

  auto particles = detail::read_particles(file);
  if (particles.size() == 0) {
    return err::unexpected(particles_per_meter.error().what());
  }

  // std::cout << "Number of particles: " << num_particles << "\n";
  // std::cout << "Particles per meter: " << particles_per_meter.value() << "\n";
  // std::cout << "Smoothing length: " << particles_param_.smoothing << "\n";
  // std::cout << "Particles Mass: " << particles_param_.mass << "\n";

  return Simulation {
    .arguments        = arguments,
    .fluid_properties = fluid_properties(*particles_per_meter),
    .grid             = Grid {std::move(particles), mul_rad / *particles_per_meter},
  };
};

constexpr auto write_output = [](Simulation&& sim) -> err::expected<Simulation> {
  std::flat_map<u64, Particle const*> results;
  std::ofstream file {sim.arguments.output_file, std::ios::binary};


  for (auto& block: sim.grid.GetBlocks()) {
    for (auto& particle: block.GetParticles()) {
      results[particle.id] = &particle;
    }
  }

  try {
    detail::write_header(file, results.size(), sim.fluid_properties.particles_per_meter);
    detail::write_particles(file, results.values());
  }
  catch (std::ifstream::failure const& e) {
    return err::unexpected(e.what());
  }

  return std::move(sim);
};

} // namespace sim
