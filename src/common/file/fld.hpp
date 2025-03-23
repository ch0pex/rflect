#pragma once

#include "particle.hpp"
#include "utils/error.hpp"

#include <fstream>
#include <vector>

namespace sim {
/// Clase encargada de la lectura de fichero .fld
class ifld {
public:
  explicit ifld(std::string const& path);

  ifld();

  ~ifld();

  void open(std::string const& path);

  void close();

  error_code readHeader(double& ppm, int& np);

  std::vector<Particle> readParticles();

  operator bool() const;

private:
  std::ifstream input_file_;
  size_t length_;
};


struct Header {
  math::scalar ppm;
  i32 np;
};

inline std::optional<Header> read_header(std::ifstream& input_file) {
  Header header {};
  f32 tmp {};
  try {
    input_file.seekg(0, std::ifstream::beg);
    input_file.read(reinterpret_cast<char*>(&tmp), sizeof(float));
    input_file.read(reinterpret_cast<char*>(&header.np), sizeof(f32));
  }
  catch (std::runtime_error const& e) {
    std::cout << e.what() << std::endl;
    return {};
  }

  header.ppm = static_cast<math::scalar>(tmp);

  return header;
}

inline std::vector<Particle> read_particles(std::ifstream& input_file) {
  std::vector<Particle> particles;

  input_file.seekg(0, std::ifstream::end);
  std::size_t const length = input_file.tellg();
  input_file.seekg(0, std::ifstream::beg);

  std::vector<f32> tmp((length - header_size) / sizeof(f32));

  particles.reserve((length - header_size) / particle_components);
  input_file.seekg(header_size, std::ifstream::beg);
  input_file.read(reinterpret_cast<char*>(tmp.data()), length - header_size);

  for (size_t i = 0; i < tmp.size(); i += particle_components) {
    math::vec3 position = {tmp[i], tmp[i + 1], tmp[i + 2]};
    math::vec3 vec_hv   = {tmp[i + 3], tmp[i + 4], tmp[i + 5]};
    math::vec3 velocity = {tmp[i + 6], tmp[i + 7], tmp[i + 8]};
    particles.emplace_back(i / particle_components, position, vec_hv, velocity);
  }

  return particles;
}


class ofld {
public:
  explicit ofld(std::string const& path);

  ofld();

  ~ofld();

  void open(std::string const& path);

  void close();

  error_code writeHeader(int np, double ppm);

  error_code writeParticles(std::vector<Particle const*>& particles);

  operator bool() const { return output_file_.is_open(); }

private:
  std::ofstream output_file_;
};
} // namespace sim
