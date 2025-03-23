#include "fld.hpp"

#include "utils/constants.hpp"

#include <iostream>
#include <span>

namespace sim {
/**
 * Constructor explicito de la clase cuando se proporciona una path, se abre el archivo
 * directamente con permisos de lectura
 * @param path: Ruta del archivo que se desea leer
 */
ifld::ifld(std::string const& path) : length_(0) { open(path); }

/**
 * Constructor por defecto de la clase, si no se proporciona ningun path, se colocan valores por
 * defecto en length y is_open
 */
ifld::ifld() : length_(0) { }

/**
 * Destructor de la clase, se asegura de que si el objeto ifld sale de scope el archivo se cierre
 */
ifld::~ifld() { close(); }

/**
 * Abre el fichero .fld como archivo binario con permisos de lectura, cambia el valor length
 * @param path: archivo .fld que se desea abrir
 */
void ifld::open(std::string const& path) {
  if (!input_file_.is_open()) {
    input_file_.open(path, std::ios::binary);
    input_file_.seekg(0, std::ifstream::end);
    length_ = input_file_.tellg();
    input_file_.seekg(0, std::ifstream::beg);
  }
}

/**
 * Cierra el archivo .fld si esta abierto
 */
void ifld::close() {
  if (input_file_.is_open()) {
    input_file_.close();
  }
}

/**
 * Lee el header del archivo y guarda sus valores si son correctos en los parametros
 * @param ppm  Referencia donde se guarda las particulas por metro especificadas en el header
 * @param np Referencia donde se guarda el numero de particulas especificado en el header
 * @return Si el numero de particulas es menor que 0 o no coincide con las particulas encontradas
 * en el archivo se devuelve PARTICLE_NUM_ERR (-5), en caso de exito se devuelve SUCCESS (0)
 */
error_code ifld::readHeader(math::scalar& ppm, int& np) {
  float tmp = 0.0F;

  input_file_.seekg(0, std::ifstream::beg);
  input_file_.read(reinterpret_cast<char*>(&tmp), sizeof(float));
  input_file_.read(reinterpret_cast<char*>(&np), sizeof(float));

  ppm = static_cast<math::scalar>(tmp);

  if (np <= 0) {
    std::cout << "Invalid number of particles\n";
    return particle_num_err;
  }

  // Check mismatches between header a file content
  if (std::size_t const particles_num = ((length_ - header_size) / sizeof(f32)) / particle_components;
      static_cast<size_t>(np) != particles_num) {
    std::cout << "Number of particles mismatch. Header: " << np << " Found: " << particles_num << "\n";
    return particle_num_err;
  }
  return success;
}

/**
 * Lee todas las particulas almacenadas en el fichero inicial
 * @return Devuelve las particulas en un vector (esto no es un problema a partir de la version
 * C++11)
 */
std::vector<Particle> ifld::readParticles() {
  std::vector<Particle> particles;
  std::vector<f32> tmp((length_ - header_size) / sizeof(f32));

  particles.reserve((length_ - header_size) / particle_components); // numero de componentes de una particula
  input_file_.seekg(header_size, std::ifstream::beg);
  input_file_.read(std::bit_cast<char*>(tmp.data()), length_ - header_size);

  for (size_t i = 0; i < tmp.size(); i += particle_components) {
    math::vec3 position = {tmp[i], tmp[i + 1], tmp[i + 2]};
    math::vec3 vec_hv   = {tmp[i + 3], tmp[i + 4], tmp[i + 5]};
    math::vec3 velocity = {tmp[i + 6], tmp[i + 7], tmp[i + 8]};
    particles.emplace_back(i / particle_components, position, vec_hv, velocity);
  }

  return particles;
}

/**
 * Operador booleano de la clase
 * @return Devuelve true si hay un fichero fld abierto y false en caso contrario
 */
ifld::operator bool() const { return input_file_.is_open(); }

/**
 * Constructor explicito de la clase cuando se proporciona una path, se abre el archivo
 * directamente con permisos escritura
 * @param path
 */
ofld::ofld(std::string const& path) { open(path); }

/**
 * Constructor por defecto de ofld
 */
ofld::ofld() = default;

/**
 * Destructor de la clase, se asegura de que si el objeto ofld sale de scope el archivo se cierre
 */
ofld::~ofld() { close(); }

/**
 * Abre el fichero .fld como archivo binario con permisos de escritura
 * @param path: archivo .fld en el que se desea escribir
 */
void ofld::open(std::string const& path) {
  if (!output_file_.is_open()) {
    output_file_.open(path, std::ios::binary);
  }
}

/**
 * Cierra el archivo .fld si esta abierto
 */
void ofld::close() {
  if (output_file_.is_open()) {
    output_file_.close();
  }
}

/**
 * Writes the file header
 *
 * @param np Number of particles in the file
 * @param ppm Particles per meter
 */
error_code ofld::writeHeader(i32 const np, f64 const ppm) {
  auto const particle_per_meter = static_cast<f32>(ppm);
  output_file_.write(reinterpret_cast<char const*>(&particle_per_meter), sizeof particle_per_meter); // NOLINT
  output_file_.write(reinterpret_cast<char const*>(np), sizeof np); // NOLINT
  return (success);
}

error_code ofld::writeParticles(std::vector<Particle const*>& particles) {
  for (auto& particle: particles) {
    output_file_.write(reinterpret_cast<char*>(&particle), sizeof(f32) * particle_components); // NOLINT
  }
  return (success);
}

} // namespace sim
