#ifndef FLUID_FLD_HPP
#define FLUID_FLD_HPP

#include "particle.hpp"
#include "utils/error.hpp"

#include <fstream>
#include <vector>

namespace sim {
  /// Clase encargada de la lectura de fichero .fld
  class ifld {
    public:
      explicit ifld(std::string const & path);

      ifld();

      ~ifld();

      void Open(std::string const & path);

      void Close();

      sim::error_code ReadHeader(double & ppm, int & np);

      std::vector<Particle> ReadParticles();

      operator bool() const;

    private:
      std::ifstream input_file_;
      size_t length_;
  };

  /// Clase encargada de la escritura de fichero .fld
  class ofld {  // TODO: Implementar escritura
    public:
      explicit ofld(std::string const & path);

      ofld();

      ~ofld();

      void Open(std::string const & path);

      void Close();

      sim::error_code WriteHeader(int np, double ppm);

      sim::error_code WriteParticles(std::vector<Particle *> & particles);

      operator bool() const;

    private:
      std::ofstream output_file_;
  };
}  // namespace sim

#endif  // FLUID_FLD_HPP
