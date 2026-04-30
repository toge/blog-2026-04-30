#include <chrono>
#include <iostream>
#include <print>

#include "re2/re2.h"

#include "pattern.h"

int main() {
  auto const data = read_file_to_string();

  auto total = std::int64_t{0};
  for (auto i = 0; i < COUNT; ++i) {
    auto const start = std::chrono::high_resolution_clock::now();

    // RE2でのマッチング
    {
      auto const re = re2::RE2{PATTERN};
      if (!re.ok()) {
        return 1;
      }

      auto input = re2::StringPiece{data};
      auto g1 = re2::StringPiece{};
      auto g2 = re2::StringPiece{};
      while (re2::RE2::FindAndConsume(&input, re, &g1, &g2)) {
        std::print("{}{}\n", g1, g2);
      }
    }

    auto const end = std::chrono::high_resolution_clock::now();
    total += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
  }

  std::print(std::cerr, "re2        : {:15.3f} ns\n", static_cast<double>(total) / COUNT);
  return 0;
}
