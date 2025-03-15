#pragma once

#include "file/fld.hpp"
#include "utils/error.hpp"

#include <span>

namespace sim {
  /// Clase encargada de comprobar los argumentos pasados por el usuario para ejecucion del programa
  class Proargs {
    public:
      explicit Proargs(std::span<char const *> args);

      [[nodiscard]] error_code CheckCount() const;

      error_code CheckNts(int & nts);

      error_code CheckOpenFiles(ifld & init_file, ofld & final_file);

      std::string GetInitPath() const { return args_.at(2); };

      std::string GetFinalPath() const { return args_.at(3); };

    private:
      std::vector<char const *> args_;
  };
}  // namespace sim
