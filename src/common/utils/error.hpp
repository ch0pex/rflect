#ifndef FLUID_ERROR_HPP
#define FLUID_ERROR_HPP

namespace sim {
  /// Enum que contiene los distintos errores que puede arrojar el simulador
  enum error_code {
    PARTICLE_NUM_ERR = -5,
    FINAL_FILE_ERR   = -4,
    INIT_FILE_ERR,
    TIME_STEP_ERR,
    ARGS_ERR,
    SUCCESS
  };
}  // namespace sim

#endif  // FLUID_ERROR_HPP
