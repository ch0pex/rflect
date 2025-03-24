#pragma once


#include "args/proargs.hpp"
#include "grid.hpp"
#include "utils/error.hpp"

#include <optional>
#include <span>

namespace sim {
  /**
   * Clase que representa al simulador de fluidos, contiene todos los metodos para ejecutar las
   * distintas fases de la simulaicon asi como el grid, los ficheros y el numero de iteraciones
   */
  class Simulator {
    public:
      explicit Simulator(std::span<char const *> args_view);

      error_code ParseArgs();

      error_code InitSim();

      error_code ProcessSim();

      error_code StoreResults();

    private:
      Proargs args_parser_;
      ifld init_file_;
      ofld final_file_;
      std::optional<Grid> grid_;
      int nts_;
  };
}  // namespace sim
