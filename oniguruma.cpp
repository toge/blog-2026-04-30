#include <print>
#include <iostream>
#include <chrono>
#include <string_view>

#include "oniguruma.h"

#include "pattern.h"

int main() {
  OnigEncoding use_encs[] = { ONIG_ENCODING_UTF8 };
  if (onig_initialize(use_encs, 1) != ONIG_NORMAL) {
    std::cerr << "onig_initialize failed\n";
    return 1;
  }

  auto&& data = read_file_to_string();
  auto const str_start = reinterpret_cast<unsigned char const*>(data.data());
  auto const str_end = str_start + data.size();

  auto total = std::int64_t{0};

  for (auto i = 0; i < COUNT; ++i) {
    auto const start = std::chrono::high_resolution_clock::now();

    // Onigurumaでのマッチング
    {
      regex_t* reg;
      OnigErrorInfo einfo;
      auto const pattern_start = reinterpret_cast<unsigned char const*>(PATTERN);
      auto const pattern_end = pattern_start + std::char_traits<char>::length(PATTERN);

      auto const r_comp = onig_new(&reg, pattern_start, pattern_end, ONIG_OPTION_DEFAULT, ONIG_ENCODING_UTF8, ONIG_SYNTAX_DEFAULT, &einfo);
      if (r_comp != ONIG_NORMAL) {
        unsigned char s[ONIG_MAX_ERROR_MESSAGE_LEN];
        onig_error_code_to_str(s, r_comp, &einfo);
        std::cerr << "onig_new failed: " << s << "\n";
        onig_end();
        return 1;
      }

      auto* const region = onig_region_new();

      auto const* current_pos = str_start;
      while (current_pos < str_end) {
        auto const r = onig_search(reg, str_start, str_end, current_pos, str_end, region, ONIG_OPTION_NONE);
        if (r >= 0) {
          if (i == 0) {
            auto const sub1_beg = region->beg[1];
            auto const sub1_end = region->end[1];
            auto const sub2_beg = region->beg[2];
            auto const sub2_end = region->end[2];
            std::print("{}{}\n",
              std::string_view{reinterpret_cast<char const*>(str_start + sub1_beg), static_cast<size_t>(sub1_end - sub1_beg)},
              std::string_view{reinterpret_cast<char const*>(str_start + sub2_beg), static_cast<size_t>(sub2_end - sub2_beg)});
          }
          current_pos = str_start + region->end[0];
          if (region->beg[0] == region->end[0]) {
            current_pos++;
          }
        } else if (r == ONIG_MISMATCH) {
          break;
        } else {
          unsigned char s[ONIG_MAX_ERROR_MESSAGE_LEN];
          onig_error_code_to_str(s, r);
          std::cerr << "onig_search failed: " << s << "\n";
          break;
        }
      }
      onig_region_free(region, 1);
      onig_free(reg);
    }

    auto const end = std::chrono::high_resolution_clock::now();
    total += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
  }

  std::print(std::cerr, "oniguruma  : {:15.3f} ns\n", static_cast<double>(total) / COUNT);

  onig_end();
  return 0;
}
