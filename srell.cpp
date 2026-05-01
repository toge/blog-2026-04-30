#include <chrono>
#include <iostream>
#include <print>
#include <string_view>

#include <srell.hpp>

#include "pattern.h"

int main() {
  auto const data = read_file_to_string();
  auto const data_view = std::string_view{data};

  auto total = std::int64_t{0};
  for (auto i = 0; i < COUNT; ++i) {
    auto const start = std::chrono::high_resolution_clock::now();

    // SRELLでのマッチング
    {
      auto const re = srell::regex{PATTERN};

      auto const begin = srell::regex_iterator<std::string_view::const_iterator>{data_view.begin(), data_view.end(), re};
      auto const end = srell::regex_iterator<std::string_view::const_iterator>{};

      for (auto it = begin; it != end; ++it) {
        auto const& match = *it;
        std::print("{}{}\n", match.str(1), match.str(2));
      }
    }

    auto const end = std::chrono::high_resolution_clock::now();
    total += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
  }

  std::print(std::cerr, "srell      : {:15.3f} ns\n", static_cast<double>(total) / COUNT);
  return 0;
}
