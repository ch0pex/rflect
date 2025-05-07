/**
 * @file test_enum.hpp
 * @version 1.0
 * @date 5/1/25
 * @brief Tests for enum introspection and manipulation functions in rflect.
 *
 * Contains Doctest test cases to verify the correct behavior of
 * enum_name, enum_value, enum_cast, enum_count, and the stream insertion
 * operator for enums provided by the rflect library.
 */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <rflect/rflect.hpp>

#include <optional>
#include <sstream>
#include <string>
#include <string_view>

enum class Color : std::int8_t { Red = 0, Green = 2, Blue = 3 };

enum Status : std::int32_t { Ok = 0, Warning = 1, Error = -1 };

TEST_SUITE_BEGIN("Enum");

TEST_CASE("enum_name returns correct names for enum class") {
  CHECK(rflect::enum_name(Color::Red) == "Red");
  CHECK(rflect::enum_name(Color::Green) == "Green");
  CHECK(rflect::enum_name(Color::Blue) == "Blue");

  // Test with a value that does not correspond to any enumerator
  CHECK(rflect::enum_name(static_cast<Color>(42)) == "<unnamed>");
  // Test with a value that falls within the range of underlying type but not explicitly defined
  CHECK(rflect::enum_name(static_cast<Color>(1)) == "<unnamed>");
}

TEST_CASE("enum_name returns correct names for unscoped enum") {
  CHECK(rflect::enum_name(Status::Ok) == "Ok");
  CHECK(rflect::enum_name(Status::Warning) == "Warning");
  CHECK(rflect::enum_name(Status::Error) == "Error");

  // Test with a value that does not correspond to any enumerator
  CHECK(rflect::enum_name(static_cast<Status>(99)) == "<unnamed>");
}


TEST_CASE("enum_value returns correct enum value from index for enum class") {
  CHECK(rflect::enum_value<Color>(0) == Color::Red);
  CHECK(rflect::enum_value<Color>(1) == Color::Green);
  CHECK(rflect::enum_value<Color>(2) == Color::Blue);
}

TEST_CASE("enum_value returns correct enum value from index for unscoped enum") {
  CHECK(rflect::enum_value<Status>(0) == Status::Ok);
  CHECK(rflect::enum_value<Status>(1) == Status::Warning);
  CHECK(rflect::enum_value<Status>(2) == Status::Error);
}

TEST_CASE("enum_cast from name returns correct enum for enum class") {
  CHECK(rflect::enum_cast<Color>("Red") == Color::Red);
  CHECK(rflect::enum_cast<Color>("Green") == Color::Green);
  CHECK(rflect::enum_cast<Color>("Blue") == Color::Blue);
  CHECK(rflect::enum_cast<Color>("Invalid") == std::nullopt);

  // Test case sensitivity (default predicate is std::equal_to)
  CHECK(rflect::enum_cast<Color>("red") == std::nullopt);
  CHECK(rflect::enum_cast<Color>("RED") == std::nullopt);

  // Test with leading/trailing spaces (default predicate is std::equal_to)
  CHECK(rflect::enum_cast<Color>(" Red ") == std::nullopt);
}

TEST_CASE("enum_cast from name returns correct enum for unscoped enum") {
  CHECK(rflect::enum_cast<Status>("Ok") == Status::Ok);
  CHECK(rflect::enum_cast<Status>("Warning") == Status::Warning);
  CHECK(rflect::enum_cast<Status>("Error") == Status::Error);
  CHECK(rflect::enum_cast<Status>("Unknown") == std::nullopt);
}


TEST_CASE("enum_cast from integral returns correct enum for enum class") {
  CHECK(rflect::enum_cast<Color>(0) == Color::Red);
  CHECK(rflect::enum_cast<Color>(2) == Color::Green);
  CHECK(rflect::enum_cast<Color>(3) == Color::Blue);
  CHECK(rflect::enum_cast<Color>(42) == std::nullopt); // Value not present

  // Test with an integral value that matches an enumerator's underlying value
  CHECK(rflect::enum_cast<Color>(static_cast<std::int8_t>(0)) == Color::Red);
  CHECK(rflect::enum_cast<Color>(static_cast<int>(2)) == Color::Green); // Test different integral type
}

TEST_CASE("enum_cast from integral returns correct enum for unscoped enum") {
  CHECK(rflect::enum_cast<Status>(0) == Status::Ok);
  CHECK(rflect::enum_cast<Status>(1) == Status::Warning);
  CHECK(rflect::enum_cast<Status>(-1) == Status::Error);
  CHECK(rflect::enum_cast<Status>(99) == std::nullopt); // Value not present
}


TEST_CASE("enum_count returns correct number of enumerators for enum class") {
  CHECK(rflect::enum_count<Color>() == 3);
}

TEST_CASE("enum_count returns correct number of enumerators for unscoped enum") {
  CHECK(rflect::enum_count<Status>() == 3);
}
TEST_SUITE_END();