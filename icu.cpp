#include <print>
#include <iostream>
#include <chrono>

#include "unicode/unistr.h"
#include "unicode/regex.h"

#include "pattern.h"

int main() {
  auto&& data = read_file_to_string();
  auto const input = icu::UnicodeString::fromUTF8(data);
  auto const pattern = icu::UnicodeString::fromUTF8(PATTERN);

  auto total = std::int64_t{0};
  for (auto i = 0; i < COUNT; ++i) {
    auto const start = std::chrono::high_resolution_clock::now();

    // ICUでのマッチング
    {
      UErrorCode status = U_ZERO_ERROR;
      icu::RegexMatcher matcher(pattern, input, 0, status);
      if (U_FAILURE(status)) {
        std::cerr << "Regex compilation failed: " << u_errorName(status) << "\n";
        return 1;
      }

      matcher.reset();
      while (matcher.find(status) && U_SUCCESS(status)) {
        if (i == 0) {
          auto const sub1 = matcher.group(1, status);
          auto const sub2 = matcher.group(2, status);

          std::string s1, s2;
          sub1.toUTF8String(s1);
          sub2.toUTF8String(s2);
          std::print("{}{}\n", s1, s2);
        }
      }
    }

    auto const end = std::chrono::high_resolution_clock::now();
    total += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
  }

  std::print(std::cerr, "ICU        : {:15.3f} ns\n", static_cast<double>(total) / COUNT);

  return 0;
}
