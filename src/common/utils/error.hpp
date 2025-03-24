#pragma once

namespace sim {
  /// Enum que contiene los distintos errores que puede arrojar el simulador
  enum error_code {
    wrong_particle_number = -5,
    output_error   = -4,
    init_file_error,
    wrong_time_step,
    wrong_args,
    success
  };
}  // namespace sim

