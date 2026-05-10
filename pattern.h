#pragma once

#include <filesystem>
#include <string>
#include <fstream>
#include <cassert>

// パターン1 : [\s\S]*? 6個
// constexpr const char PATTERN[] = R"re(<li class="item-card[\s\S]*?data-product-id="([0-9]+)"[\s\S]*?data-product-price="([0-9]+)"[\s\S]*?<div class="item-card__title"><a[\s\S]*?href="(https://booth\.pm/ja/items/[0-9]+)">([\s\S]*?)</a>[\s\S]*?<div class="item-card__shop-name">)re";

// パターン2 : [\s\S]*? 5個
// constexpr const char PATTERN[] = R"re(<li class="item-card[\s\S]*?data-product-id="([0-9]+)"[\s\S]*?data-product-price="([0-9]+)"[\s\S]*?<div class="item-card__title"><a[\s\S]*?href="(https://booth\.pm/ja/items/[0-9]+)">([\s\S]*?)</a>)re";

// パターン3 : [\s\S]*? 4個
// constexpr const char PATTERN[] = R"re(<li class="item-card[\s\S]*?data-product-id="([0-9]+)"[\s\S]*?data-product-price="([0-9]+)"[\s\S]*?<div class="item-card__title"><a[\s\S]*?href="(https://booth\.pm/ja/items/[0-9]+)">)re";

// パターン4 : [\s\S]*? 3個
// constexpr const char PATTERN[] = R"re(<li class="item-card[\s\S]*?data-product-id="([0-9]+)"[\s\S]*?data-product-price="([0-9]+)"[\s\S]*?<div class="item-card__title"><a)re";

// パターン5 : [\s\S]*? 2個
constexpr const char PATTERN[] = R"re(<li class="item-card([\s\S]*?)data-product-id="([0-9]+)"[\s\S]*?data-product-price=)re";

auto constexpr COUNT = 1000;

auto read_file_to_string() -> std::string {
  auto result = std::string{};
  result.resize_and_overwrite(std::filesystem::file_size("data/sample.html"), [&](char* buffer, size_t size) {
    std::ifstream("data/sample.html").read(buffer, static_cast<std::streamsize>(size));
    return size;
  });

  // ウォームアップ
  for (int i = 0; i < COUNT; ++i) {
    int sum = 0;
    for (auto const c : result) {
      sum += c;
    }
    (void)sum;
    assert(sum > 0);
  }

  return result;
}
