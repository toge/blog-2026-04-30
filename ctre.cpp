#include <print>
#include <iostream>
#include <chrono>

#include "ctre.hpp"

#include "pattern.h"

int main() {
  auto&& data = read_file_to_string();

  auto total = std::int64_t{0};
  for (auto i = 0; i < COUNT; ++i) {
    auto const start = std::chrono::high_resolution_clock::now();

    // CTREでのマッチング
    {
      for (auto const& match : ctre::range<PATTERN>(data)) {
        std::print("{}{}\n", match.get<1>().to_view(), match.get<2>().to_view());
      }
    }

    auto const end = std::chrono::high_resolution_clock::now();
    total += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
  }

  std::print(std::cerr, "ctre       : {:15.3f} ns\n", static_cast<double>(total) / COUNT);
  return 0;
}
