#include <print>
#include <iostream>
#include <chrono>
#include <string_view>
#include <pcrepp.hpp>

#include "pattern.h"

int main() {
  auto const data = read_file_to_string();

  auto total = std::int64_t{0};

  for (auto i = 0; i < COUNT; ++i) {
    auto const start = std::chrono::high_resolution_clock::now();

    // pcreppでのマッチング
#ifdef PCREPP_JIT
    auto ctx_res = pcrepp::context<true>::create(PATTERN);
#else
    auto ctx_res = pcrepp::context<false>::create(PATTERN);
#endif

    if (!ctx_res) {
      std::cerr << "Failed to compile pattern\n";
      return 1;
    }

    auto const& ctx = *ctx_res;

    for (auto const& res : ctx.search_all(data)) {
      if (res) {
        for (auto j = 1u; j < res.size(); ++j) {
          if (j >= 3) {
            break;
          }
          std::print("{}", res[j]);
        }
        std::print("\n");
      }
    }

    auto const end = std::chrono::high_resolution_clock::now();
    total += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
  }

#ifdef PCREPP_JIT
  std::print(std::cerr, "pcrepp JIT : {:15.3f} ns\n", static_cast<double>(total) / COUNT);
#else
  std::print(std::cerr, "pcrepp     : {:15.3f} ns\n", static_cast<double>(total) / COUNT);
#endif
  return 0;
}
