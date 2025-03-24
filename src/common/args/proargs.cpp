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
  sim::error_code Proargs::CheckCount() const {
    if (args_.size() != 4) {
      std::cout << "Error: Invalid number of arguments: " << args_.size() << "\n";
      return (ARGS_ERR);
    }
    return (SUCCESS);
  }

  /**
   * Comprueba que el nts pasado por el usuario sea correcto
   * @param nts: referencia a variable donde se almacenara el nts como int si es correcto
   * @return Se devuelve ARGS_ERR (-1) si el time step indicado no es numerico, TIME_STEP_ERR (-2)
   * si es menor o igual a 0 y SUCCESS(0) en caso de exito
   */
  sim::error_code Proargs::CheckNts(int & nts) {
    char * endPtr = nullptr;
    long const nts_long =
        std::strtol(args_.at(1), &endPtr, 10);  // Se usa strtol para mejor prevencion de errores

    if (*endPtr != '\0') {
      std::cout << "Error: time step must be numeric\n";
      return (ARGS_ERR);
    }
    if (nts_long <= 0) {
      std::cout << "Error: Invalid number of time steps\n";
      return (TIME_STEP_ERR);
    }
    nts = static_cast<int>(nts_long);
    return (SUCCESS);
  }

  /**
   * Comprueba que los paths pasados por argumentos y ambos archivos pueden ser abiertos
   * @param init_file: archivo que se desea leer
   * @param final_file: archivo donde se desea escribir los resultados
   * @return Si no se puede abrir el archivo inicial se devuelve INIT_FILE_ERR (-3), si no se puede
   * abrir el archivo final se devuelve FINAL_FILE_ERR (-4) y SUCCESS (0) en caso de exito
   */
  sim::error_code Proargs::CheckOpenFiles(sim::ifld & init_file, sim::ofld & final_file) {
    init_file.Open(GetInitPath());
    if (!init_file) {
      std::cout << "Error: Cannot open " << args_.at(2) << " for reading\n";
      return (INIT_FILE_ERR);
    }

    final_file.Open(GetFinalPath());
    if (!final_file) {
      std::cout << "Error: Cannot open " << args_.at(3) << " for writing\n";
      return (FINAL_FILE_ERR);
    }
    // Quiza cerrar final file, no dejarlo abierto toda la ejecucion del programa
    return (SUCCESS);
  }

}  // namespace sim