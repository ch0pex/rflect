#pragma once

namespace sim {
  /// Enum que contiene los distintos errores que puede arrojar el simulador
  enum error_code {
    particle_num_err = -5,
    final_file_err   = -4,
    init_file_err,
    time_step_err,
    args_err,
    success
  };
}  // namespace sim

