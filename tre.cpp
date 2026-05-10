#include <print>
#include <iostream>
#include <chrono>
#include <string_view>

#include "tre/regex.h"

#include "pattern.h"

int main() {
  auto&& data = read_file_to_string();

  auto total = std::int64_t{0};
  for (auto i = 0; i < COUNT; ++i) {
    auto const start = std::chrono::high_resolution_clock::now();

    // TREでのマッチング
    {
      regex_t preg;
      if (regcomp(&preg, PATTERN, REG_EXTENDED) != 0) {
        std::cerr << "regcomp failed\n";
        return 1;
      }

      // ひとまず十分な数のサブマッチを確保する
      auto constexpr nmatch = 10;
      regmatch_t pmatch[nmatch];

      auto const* current_pos = data.c_str();
      while (regexec(&preg, current_pos, nmatch, pmatch, 0) == 0) {
        if (i == 0) {
          auto const sub1_start = pmatch[1].rm_so;
          auto const sub1_end = pmatch[1].rm_eo;
          auto const sub2_start = pmatch[2].rm_so;
          auto const sub2_end = pmatch[2].rm_eo;

          if (sub1_start != -1 && sub2_start != -1) {
            std::print("{}{}\n",
              std::string_view{current_pos + sub1_start, static_cast<size_t>(sub1_end - sub1_start)},
              std::string_view{current_pos + sub2_start, static_cast<size_t>(sub2_end - sub2_start)});
          }
        }

        auto const match_len = pmatch[0].rm_eo;
        current_pos += match_len;
        if (match_len == 0) {
          current_pos++;
        }
        if (*current_pos == '\0') {
          break;
        }
      }
      regfree(&preg);
    }

    auto const end = std::chrono::high_resolution_clock::now();
    total += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
  }

  std::print(std::cerr, "TRE        : {:15.3f} ns\n", static_cast<double>(total) / COUNT);

  return 0;
}
