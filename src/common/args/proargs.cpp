#include "proargs.hpp"

#include "file/fld.hpp"

#include <iostream>

namespace sim {
  /**
   * Contructor de la clase
   * @param args_view: Este argumento es un span creado a partir de argv
   */
  Proargs::Proargs(std::span<char const *> const args_view)
    : args_(args_view.begin(), args_view.end()) { }

  /**
   * Comprueba que el numero de argumentos sea el correcto
   * @return Si el numero de argumentos es el adecuado se devuelve SUCCESS (0) en caso contrario
   * ARGS_ERR (-1)
   */
  error_code Proargs::CheckCount() const {
    if (args_.size() != 4) {
      std::cout << "Error: Invalid number of arguments: " << args_.size() << "\n";
      return (args_err);
    }
    return (success);
  }

  /**
   * Comprueba que el nts pasado por el usuario sea correcto
   * @param nts: referencia a variable donde se almacenara el nts como int si es correcto
   * @return Se devuelve ARGS_ERR (-1) si el time step indicado no es numerico, TIME_STEP_ERR (-2)
   * si es menor o igual a 0 y SUCCESS(0) en caso de exito
   */
  error_code Proargs::CheckNts(int & nts) {
    char * endPtr = nullptr;
    long const nts_long =
        std::strtol(args_.at(1), &endPtr, 10);

    if (*endPtr != '\0') {
      std::cout << "Error: time step must be numeric\n";
      return args_err;
    }
    if (nts_long <= 0) {
      std::cout << "Error: Invalid number of time steps\n";
      return time_step_err;
    }
    nts = static_cast<i32>(nts_long);
    return success;
  }

  /**
   * Comprueba que los paths pasados por argumentos y ambos archivos pueden ser abiertos
   * @param init_file: archivo que se desea leer
   * @param final_file: archivo donde se desea escribir los resultados
   * @return Si no se puede abrir el archivo inicial se devuelve INIT_FILE_ERR (-3), si no se puede
   * abrir el archivo final se devuelve FINAL_FILE_ERR (-4) y SUCCESS (0) en caso de exito
   */
  error_code Proargs::CheckOpenFiles(sim::ifld & init_file, sim::ofld & final_file) {
    init_file.open(GetInitPath());
    if (!init_file) {
      std::cout << "Error: Cannot open " << args_.at(2) << " for reading\n";
      return (init_file_err);
    }

    final_file.open(GetFinalPath());
    if (!final_file) {
      std::cout << "Error: Cannot open " << args_.at(3) << " for writing\n";
      return final_file_err;
    }
    return success;
  }

}  // namespace sim