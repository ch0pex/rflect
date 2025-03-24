#include "sim/args/proargs.hpp"

#include <gtest/gtest.h>
#include <span>


namespace sim {
  /**
   * Test función CheckCount con argumentos válidos.
   */
  TEST(ProargsTest, CheckCountValidArgs) {
    // Se usan arrays tipo C puesto que imitan a los argumentos del programa que se pasan a la funcion main
    char const * args[] = {"prog_name", "10", "init_path", "final_path"};
    Proargs const parser(std::span<char const *>(args, 4));
    EXPECT_EQ(0, parser.CheckCount());
  }

  /**
   * Test función CheckCount con argumentos inválidos (faltan argumentos).
   */
  TEST(ProargsTest, CheckCountInvalidArgs) {
    char const * args[] = {"prog_name", "10", "init_path"};
    Proargs const parser(std::span<char const *>(args, 3));
    EXPECT_EQ(-1, parser.CheckCount());
  }

  /**
   * Test función CheckNts con un valor numérico válido.
   */
  TEST(ProargsTest, CheckNtsValidValue) {
    int nts_value       = 0;
    char const * args[] = {"prog_name", "10", "init_path", "final_path"};
    Proargs parser(std::span<char const *>(args, 4));
    EXPECT_EQ(0, parser.CheckNts(nts_value));
    EXPECT_EQ(10, nts_value);
  }

  /**
   * Test función CheckNts con un valor no numérico.
   */
  TEST(ProargsTest, CheckNtsNonNumeric) {
    char const * args[] = {"prog_name", "abc", "init_path", "final_path"};
    Proargs parser(std::span<char const *>(args, 4));
    int nts = 0;
    EXPECT_EQ(-1, parser.CheckNts(nts));
  }

  /**
   * Test función CheckNts con un valor numérico negativo.
   */
  TEST(ProargsTest, CheckNtsNegativeValue) {
    int nts_value       = 0;
    char const * args[] = {"prog_name", "-10", "init_path", "final_path"};
    Proargs parser(std::span<char const *>(args, 4));
    EXPECT_EQ(-2, parser.CheckNts(nts_value));
  }
}  // namespace sim
