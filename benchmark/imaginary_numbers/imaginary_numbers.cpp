/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file imaginary_numbers_benchmark.cpp
 * @version 1.1
 * @date 04/26/25
 * @brief Benchmark comparing SoA, AoS and rflect::dual_vector.
 */

#include <rflect/rflect.hpp>
#include <benchmark/benchmark.h>

#include <ranges>
#include <vector>

struct Complex {
  DEFINE_PROXY(real, imag);
  float real;
  float imag;
};

struct ComplexSoA {
  std::vector<float> real;
  std::vector<float> imag;
};

static void benchmark_aos(benchmark::State& state) {
  std::size_t const n = static_cast<std::size_t>(state.range(0));
    std::vector<Complex> a(n);
    std::vector<Complex> b(n);
    float v = 0.0F;
    for (auto [x,y] : std::views::zip(a,b)) {
      x.real = v++;
      y.real = v;
    }
  for (auto _ : state) {
    for (auto [x,y] : std::views::zip(a,b)) {
      x.real *= y.real;
    }
  }
}

static void benchmark_soa(benchmark::State& state) {
  std::size_t const n = state.range(0);
    ComplexSoA a;
    ComplexSoA b;
    a.real.resize(n);   a.imag.resize(n);
    b.real.resize(n);   b.imag.resize(n);
    float v = 0.0F;
    for (auto [x,y] : std::views::zip(a.real, b.real)) {
      x = v++;
      y = v;
    }
  for (auto _ : state) {
    for (auto [x,y] : std::views::zip(a.real, b.real)) {
      x *= y;
    }
  }
}

template<rflect::memory_layout Layout>
static void benchmark_rflect(benchmark::State& state) {
  auto n = static_cast<std::size_t>(state.range(0));
    rflect::dual_vector<Complex, Layout> a(n), b(n);
    float v = 0.0F;
    for (auto [x,y] : std::views::zip(a, b)) {
      x.real() = v++;
      y.real() = v;
    }
  for (auto _ : state) {
    for (auto [x,y] : std::views::zip(a, b)) {
      x.real() *= y.real();
    }
  }
}

constexpr auto time_unit = benchmark::kMicrosecond;

// clang-format off

BENCHMARK(benchmark_soa)
    ->Arg(1<<10)
    ->Arg(1<<14)
    ->Arg(1<<18)
    ->Arg(1<<22)
    ->Unit(time_unit);

BENCHMARK_TEMPLATE(benchmark_rflect, rflect::layout::soa)
    ->Arg(1<<10)
    ->Arg(1<<14)
    ->Arg(1<<18)
    ->Arg(1<<22)
    ->Unit(time_unit)
    ->Name("RflectSoa");

BENCHMARK(benchmark_aos)
    ->Arg(1<<10)
    ->Arg(1<<14)
    ->Arg(1<<18)
    ->Arg(1<<22)
    ->Unit(time_unit);

BENCHMARK_TEMPLATE(benchmark_rflect, rflect::layout::aos)
    ->Arg(1<<10)
    ->Arg(1<<14)
    ->Arg(1<<18)
    ->Arg(1<<22)
    ->Unit(time_unit)
    ->Name("RflectAos");

// clang-format on
BENCHMARK_MAIN();
