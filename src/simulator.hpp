#pragma once

#include "common/args/proargs.hpp"
#include "common/file/fld.hpp"
#include "common/utils/error.hpp"

#include "grid.hpp"

#include <optional>
#include <span>

namespace sim {



/**
 * Simulator main class
 */
class Simulator {
public:
  explicit Simulator(std::span<char const*> args_view);

  error_code parseArgs();

  error_code initSim();

  error_code process();

  error_code storeResults();

private:
  Proargs args_parser_;
  ifld init_file_;
  ofld final_file_;
  std::optional<Grid> grid_;
  int nts_;
};

} // namespace sim
