#include <print>
#include <iostream>
#include <chrono>
#include <string_view>

#define PCRE2_CODE_UNIT_WIDTH 8
#include "pcre2.h"

#include "pattern.h"

int main() {
  auto const data = read_file_to_string();

  auto total = std::int64_t{0};

  for (auto i = 0; i < COUNT; ++i) {
    auto const start = std::chrono::high_resolution_clock::now();

    // PCRE2でのマッチング
    {
      auto ec = 0;
      auto erroroffset = PCRE2_SIZE{};
      auto const re = std::unique_ptr<pcre2_code, decltype([](auto p) {
        pcre2_code_free(p);
      })>{
        pcre2_compile(reinterpret_cast<PCRE2_SPTR>(PATTERN), PCRE2_ZERO_TERMINATED, 0, &ec, &erroroffset, nullptr)
      };
      if (!re) {
        return 1;
      }

#ifdef PCRE2_JIT
      if (pcre2_jit_compile(re.get(), PCRE2_JIT_COMPLETE) != 0) {
        return 1;
      }
#endif
      auto const match_data = std::unique_ptr<pcre2_match_data, decltype([](auto p) {
        pcre2_match_data_free(p);
      })>{pcre2_match_data_create_from_pattern(re.get(), nullptr)};

      auto start_offset = PCRE2_SIZE{0};
      auto const subject = reinterpret_cast<PCRE2_SPTR>(data.data());
      auto const subject_length = data.size();

      while (true) {
#ifdef PCRE2_JIT
        auto const rc = pcre2_jit_match(re.get(), subject, subject_length, start_offset, 0, match_data.get(), nullptr);
#else
        auto const rc = pcre2_match(re.get(), subject, subject_length, start_offset, 0, match_data.get(), nullptr);
#endif
        if (rc < 0) {
          break;
        }

        auto const ovector = pcre2_get_ovector_pointer(match_data.get());
        for (auto j = 1; j < rc; ++j) {
          if (j >= 3) {
            break;
          }
          auto const start = ovector[2 * j];
          auto const end = ovector[2 * j + 1];
          std::print("{}", std::string_view{data}.substr(start, end - start));
        }
        std::print("\n");
        start_offset = ovector[1];
      }
    }

    auto const end = std::chrono::high_resolution_clock::now();
    total += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
  }

#ifdef PCRE2_JIT
  std::print(std::cerr, "pcre JIT   : {:15.3f} ns\n", static_cast<double>(total) / COUNT);
#else
  std::print(std::cerr, "pcre       : {:15.3f} ns\n", static_cast<double>(total) / COUNT);
#endif
  return 0;
}
