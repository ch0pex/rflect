/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file main.cpp
 * @version 1.0
 * @date 23/03/2025
 * @brief Benchmark entrypoint
 *
 */

#include "common.hpp"

int main() {
  try {
    test_simulation("large", 2000);
  }
  catch (...) {
    std::cout << "Something went wrong\n";
  }
}
