#ifndef ARQUICOMP_P1_PROARGS_HPP
#define ARQUICOMP_P1_PROARGS_HPP

#include "file/fld.hpp"
#include "utils/error.hpp"

#include <span>

namespace sim {
  /// Clase encargada de comprobar los argumentos pasados por el usuario para ejecucion del programa
  class Proargs {
    public:
      explicit Proargs(std::span<char const *> args);

      [[nodiscard]] sim::error_code CheckCount() const;

      sim::error_code CheckNts(int & nts);

      sim::error_code CheckOpenFiles(sim::ifld & init_file, sim::ofld & final_file);

      inline std::string GetInitPath() { return args_.at(2); };

      inline std::string GetFinalPath() { return args_.at(3); };

    private:
      std::vector<char const *> args_;
  };
}  // namespace sim

#endif  // ARQUICOMP_P1_PROARGS_HPP