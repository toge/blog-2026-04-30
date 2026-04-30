#include <print>
#include <iostream>
#include <chrono>

#include <regex>

#include "pattern.h"

int main() {
  auto&& data = read_file_to_string();

  auto total = std::int64_t{0};
  for (auto i = 0; i < COUNT; ++i) {
    auto const start = std::chrono::high_resolution_clock::now();

    // std::regexでのマッチング
    {
      auto re = std::regex{PATTERN};
      auto const begin = std::sregex_iterator{data.begin(), data.end(), re};
      auto const end = std::sregex_iterator{};
      for (auto it = begin; it != end; ++it) {
        std::print("{}{}\n", (*it)[1].str(), (*it)[2].str());
      }
    }

    auto const end = std::chrono::high_resolution_clock::now();
    total += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
  }

  std::print(std::cerr, "std::regex : {:15.3f} ns\n", static_cast<double>(total) / COUNT);
  return 0;
}
