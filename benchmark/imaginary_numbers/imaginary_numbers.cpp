/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file imaginary_numbers.hpp
 * @version 1.0
 * @date 4/11/25
 * @brief Short description
 *
 * Longer description
 */


#include <chrono>
#include <cmath>
#include <iostream>
#include <ranges>
#include <vector>
#include <rflect/rflect.hpp>

struct Complex {
  DEFINE_PROXY(real, imag);
  float real;
  float imag;
};

struct ComplexSoA {
  std::vector<float> real;
  std::vector<float> imag;
};

constexpr size_t N     = 1'000'000'000;
constexpr int NUM_RUNS = 2;

template<rflect::memory_layout layout>
double run_benchmark_rflect() {
  rflect::dual_vector<Complex, layout> vector_a(N);
  rflect::dual_vector<Complex, layout> vector_b(N);

  float value = 0.0F;
  for (auto [a, b]: std::views::zip(vector_a, vector_b)) {
    a.real() = value++;
    b.real() = value;
  }

  auto const start = std::chrono::high_resolution_clock::now();
  for (auto [a, b]: std::views::zip(vector_a, vector_b)) {
    a.real() *= b.real();
  }
  auto const end = std::chrono::high_resolution_clock::now();

  // std::cout << "Result reflect " << display_string_of(^^layout) << ": ";
  // for (auto const a : vector_a) {
  //   std::cout << a.real() << ", ";
  // }
  // std::cout << "\n";
  return std::chrono::duration<double, std::milli>(end - start).count();
}

double run_benchmark_aos() {
  std::vector<Complex> vector_a(N);
  std::vector<Complex> vector_b(N);

  float value = 0.0F;
  for (auto [a, b]: std::views::zip(vector_a, vector_b)) {
    a.real = value++;
    b.real = value;
  }

  auto const start = std::chrono::high_resolution_clock::now();
  for (auto [a, b]: std::views::zip(vector_a, vector_b)) {
    a.real *= b.real;
  }
  auto const end = std::chrono::high_resolution_clock::now();

  // std::cout << "Result aos: ";
  // for (auto const [real, imag] : vector_a) {
  //   std::cout << real << ", ";
  // }
  // std::cout << "\n";
  return std::chrono::duration<double, std::milli>(end - start).count();
}

double run_benchmark_soa() {
  ComplexSoA vector_a;
  ComplexSoA vector_b;
  vector_a.real.resize(N);
  vector_a.imag.resize(N);
  vector_b.real.resize(N);
  vector_b.imag.resize(N);

  float value = 0.0F;
  for (auto [ar, br]: std::views::zip(vector_a.real, vector_b.real)) {
    ar = value++;
    br = value;
  }

  auto const start = std::chrono::high_resolution_clock::now();
  for (auto [ar, br]: std::views::zip(vector_a.real, vector_b.real)) {
    ar *= br;
  }
  auto const end = std::chrono::high_resolution_clock::now();

  // std::cout << "Result soa: ";
  // for (auto const real : vector_a.real) {
  //   std::cout << real << ", ";
  // }
  // std::cout << "\n";
  return std::chrono::duration<double, std::milli>(end - start).count();
}

std::pair<double, double> compute_stats(std::vector<double> const& times) {
  double sum = 0;
  for (auto const t: times)
    sum += t;
  double mean = sum / times.size();

  double sq_sum = 0;
  for (auto t: times)
    sq_sum += (t - mean) * (t - mean);
  double stddev = std::sqrt(sq_sum / times.size());

  return {mean, stddev};
}

int main() {
  std::vector<double> aos_times;
  std::vector<double> soa_times;
  std::vector<double> rflect_soa_times;
  std::vector<double> rflect_aos_times;

  for (int i = 0; i < NUM_RUNS; ++i) {
    std::cout << std::format("Run number: {}\n", i);
    aos_times.push_back(run_benchmark_aos());
    soa_times.push_back(run_benchmark_soa());
    rflect_soa_times.push_back(run_benchmark_rflect<rflect::layout::soa>());
    rflect_aos_times.push_back(run_benchmark_rflect<rflect::layout::aos>());
  }

  auto [aos_mean, aos_stddev] = compute_stats(aos_times);
  auto [soa_mean, soa_stddev] = compute_stats(soa_times);
  auto [rflect_aos_mean, rflect_aos_stddev] = compute_stats(rflect_aos_times);
  auto [rflect_soa_mean, rflect_soa_stddev] = compute_stats(rflect_soa_times);

  std::cout << "AoS Average: " << aos_mean << " ms (±" << aos_stddev << ")\n";
  std::cout << "SoA Average: " << soa_mean << " ms (±" << soa_stddev << ")\n";
  std::cout << "rflect AoS Average: " << rflect_aos_mean << " ms (±" << rflect_aos_stddev << ")\n";
  std::cout << "rflect SoA Average: " << rflect_soa_mean << " ms (±" << rflect_soa_stddev << ")\n";
}
