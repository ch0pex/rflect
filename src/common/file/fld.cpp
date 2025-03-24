#include "fld.hpp"

#include "utils/constants.hpp"

#include <array>
#include <cstring>
#include <iostream>

namespace sim {
  /**
   * Constructor explicito de la clase cuando se proporciona una path, se abre el archivo
   * directamente con permisos de lectura
   * @param path: Ruta del archivo que se desea leer
   */
  ifld::ifld(std::string const & path) : length_(0) {
    Open(path);
  }

  /**
   * Constructor por defecto de la clase, si no se proporciona ningun path, se colocan valores por
   * defecto en length y is_open
   */
  ifld::ifld() : length_(0) { }

  /**
   * Destructor de la clase, se asegura de que si el objeto ifld sale de scope el archivo se cierre
   */
  ifld::~ifld() {
    Close();
  }

  /**
   * Abre el fichero .fld como archivo binario con permisos de lectura, cambia el valor length
   * @param path: archivo .fld que se desea abrir
   */
  void ifld::Open(std::string const & path) {
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
  void ifld::Close() {
    if (input_file_.is_open()) { input_file_.close(); }
  }

  /**
   * Lee el header del archivo y guarda sus valores si son correctos en los parametros
   * @param ppm  Referencia donde se guarda las particulas por metro especificadas en el header
   * @param np Referencia donde se guarda el numero de particulas especificado en el header
   * @return Si el numero de particulas es menor que 0 o no coincide con las particulas encontradas
   * en el archivo se devuelve PARTICLE_NUM_ERR (-5), en caso de exito se devuelve SUCCESS (0)
   */
  sim::error_code ifld::ReadHeader(double & ppm, int & np) {
    float tmp = 0.0F;

    input_file_.seekg(0, std::ifstream::beg);
    // el siguiente comentario está justificado para esta parte de la práctica por el profesorado
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    input_file_.read(reinterpret_cast<char *>(&tmp), sizeof(float));
    ppm = static_cast<double>(tmp);
    // el siguiente comentario está justificado para esta parte de la práctica por el profesorado
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    input_file_.read(reinterpret_cast<char *>(&np), sizeof(float));
    if (np <= 0) {
      std::cout << "Invalid number of particles\n";
      return (PARTICLE_NUM_ERR);
    }
    // En cada partícula tenemos 9 números (3 vectores con 3 datos)
    // si np es distinto del número de floats partido de 9, el número de partículas del header no
    // coincide con las del archivo
    if (static_cast<size_t>(np) !=
        ((length_ - SIZE_HEADER) / sizeof(float)) / PARTICLE_COMPONENTS) {
      std::cout << "Number of particles mismatch. Header: " << np
                << " Found: " << ((length_ - SIZE_HEADER) / 4) / PARTICLE_COMPONENTS << "\n";
      return (PARTICLE_NUM_ERR);
    }
    return (SUCCESS);
  }

  /**
   * Lee todas las particulas almacenadas en el fichero inicial
   * @return Devuelve las particulas en un vector (esto no es un problema a partir de la version
   * C++11)
   */
  std::vector<Particle> ifld::ReadParticles() {
    std::vector<Particle> particles;
    std::vector<float> tmp((length_ - SIZE_HEADER) / sizeof(float));
    math::vec3 position;
    math::vec3 vec_hv;
    math::vec3 velocity;

    particles.reserve((length_ - SIZE_HEADER) / PARTICLE_COMPONENTS);  // numero de componentes de una particula
    input_file_.seekg(SIZE_HEADER, std::ifstream::beg);
    // el siguiente comentario está justificado para esta parte de la práctica por el profesorado
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    input_file_.read(reinterpret_cast<char *>(tmp.data()), length_ - SIZE_HEADER);
    for (size_t i = 0; i < tmp.size(); i += PARTICLE_COMPONENTS) {
      position = {tmp[i], tmp[i + 1], tmp[i + 2]};
      vec_hv   = {tmp[i + 3], tmp[i + 4], tmp[i + 5]};
      velocity = {tmp[i + 6], tmp[i + 7], tmp[i + 8]};
      particles.emplace_back(i / PARTICLE_COMPONENTS, position, vec_hv, velocity);
    }

    return (particles);  // NO se si tiene coste creo que no
  }

  /**
   * Operador booleano de la clase
   * @return Devuelve true si hay un fichero fld abierto y false en caso contrario
   */
  ifld::operator bool() const {
    return input_file_.is_open();
  }

  /**
   * Constructor explicito de la clase cuando se proporciona una path, se abre el archivo
   * directamente con permisos escritura
   * @param path
   */
  ofld::ofld(std::string const & path) {
    Open(path);
  }

  /**
   * Constructor por defecto de ofld
   */
  ofld::ofld() = default;

  /**
   * Destructor de la clase, se asegura de que si el objeto ofld sale de scope el archivo se cierre
   */
  ofld::~ofld() {
    Close();
  }

  /**
   * Abre el fichero .fld como archivo binario con permisos de escritura
   * @param path: archivo .fld en el que se desea escribir
   */
  void ofld::Open(std::string const & path) {
    if (!output_file_.is_open()) { output_file_.open(path, std::ios::binary); }
  }

  /**
   * Cierra el archivo .fld si esta abierto
   */
  void ofld::Close() {
    if (output_file_.is_open()) { output_file_.close(); }
  }

  /**
  * Escribe una cabecera en un archivo de salida.
  *
  * @param np Número de partículas a escribir en la cabecera.
  * @param ppm Cantidad de partículas por metro (partículas por metro).
  * @return Un código de error que indica el resultado de la operación.
  */
  sim::error_code ofld::WriteHeader(int np, double ppm) {
    // Convierte ppm a un valor de punto flotante y escribe el número de partículas y ppm en el archivo de salida.
    auto particle_per_meter = static_cast<float>(ppm);
    // el siguiente comentario está justificado para esta parte de la práctica por el profesorado
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    output_file_.write(reinterpret_cast<char *>(&particle_per_meter), sizeof(float));
    // el siguiente comentario está justificado para esta parte de la práctica por el profesorado
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    output_file_.write(reinterpret_cast<char *>(&np), sizeof(int));
    return (SUCCESS);
  }

  /**
   *
   * @return
   */
  sim::error_code ofld::WriteParticles(std::vector<Particle *> & particles) {
    // Inicializa un array temporal para almacenar los datos de cada partícula.
    std::array<float, PARTICLE_COMPONENTS> tmp_values{};
    for (auto & particle : particles) {
      // Copia los componentes de la partícula en el array temporal y luego escribe los datos en el archivo de salida.
      tmp_values[0] = static_cast<float>(particle->position.x);
      tmp_values[1] = static_cast<float>(particle->position.y);
      tmp_values[2] = static_cast<float>(particle->position.z);
      tmp_values[3] = static_cast<float>(particle->hv.x);
      tmp_values[4] = static_cast<float>(particle->hv.y);
      tmp_values[5] = static_cast<float>(particle->hv.z);
      tmp_values[6] = static_cast<float>(particle->velocity.x);
      tmp_values[7] = static_cast<float>(particle->velocity.y);
      tmp_values[8] = static_cast<float>(particle->velocity.z);
      // el siguiente comentario está justificado para esta parte de la práctica por el profesorado
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      output_file_.write(reinterpret_cast<char *>(tmp_values.data()),
                         sizeof(float) * PARTICLE_COMPONENTS);
    }
    return (SUCCESS);
  }

  /**
   * Convierte el objeto de la clase ofld en un valor booleano.
   *
   * @return true si el archivo de salida está abierto, false si está cerrado.
   */
  ofld::operator bool() const {
    return output_file_.is_open();
  }

}  // namespace sim
