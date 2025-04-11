/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file main.cpp
 * @version 1.0
 * @date 23/03/2025
 * @brief Short description
 *
 * Longer description
 */

#include "common.hpp"

int main() {
  // Check correctness
  for (sim::i8 i = 1; i <= 5; ++i) {
    test_simulation("large", i);
  }
  for (sim::i8 i = 1; i <= 5; ++i) {
    test_simulation("small", i);
  }

  // Benchmark
  for (sim::i8 i = 1; i <= 1; ++i) {
    test_simulation("large", 1000);
  }
}
