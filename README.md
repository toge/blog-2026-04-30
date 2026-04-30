# 正規表現ライブラリの実行時間比較

## 概要

正規表現ライブラリの実行時間を比較するためのコードと結果をまとめています。

背景を含めた話を纏めたブログ記事は[こちら](https://toge-blog.com/posts/2026-05-01-regex-bench/)に書いています。

## 想定している利用用途

- 大量のHTMLデータから特定の情報を抽出するようなケースを想定しています。
- 単発のツールを想定しているため正規表現のコンパイル時間も含めて計測しています。

## 検索データ

BOOTHの技術書カテゴリの新着順で検索した結果を使用しています。
以下のコマンドで検索結果を取得して、data/sample.html として保存してください。
このファイル名をハードコードしているので、配置されていないと計測できません。

```
curl 'https://booth.pm/ja/browse/%E6%8A%80%E8%A1%93%E6%9B%B8?sort=new'
```

## 実行結果

### パターン1

> <li class="item-card[\s\S]*?data-product-id="([0-9]+)"[\s\S]*?data-product-price="([0-9]+)"[\s\S]*?<div class="item-card__title"><a[\s\S]*?href="(https://booth\.pm/ja/items/[0-9]+)">([\s\S]*?)</a>[\s\S]*?<div class="item-card__shop-name">

CTREが圧倒的に遅いですが、RE2も地味に遅いです。

| 正規表現ライブラリ | 実行時間 (ns) |
| ------------------ | ------------: |
| std::regex         |   5334772.537 |
| ctre               |  11596293.595 |
| pcre               |   2234861.686 |
| pcre JIT           |    155312.080 |
| re2                |   7926048.861 |

![実行時間の比較](./images/pattern1.png)

### パターン2

> <li class="item-card[\s\S]*?data-product-id="([0-9]+)"[\s\S]*?data-product-price="([0-9]+)"[\s\S]*?<div class="item-card__title"><a[\s\S]*?href="(https://booth\.pm/ja/items/[0-9]+)">([\s\S]*?)</a>


| 正規表現ライブラリ | 実行時間 (ns) |
| ------------------ | ------------: |
| std::regex         |   4998101.433 |
| ctre               |   8819382.618 |
| pcre               |   2108687.606 |
| pcre JIT           |    104397.303 |
| re2                |   5527398.132 |

![実行時間の比較](./images/pattern2.png)

### パターン3

> <li class="item-card[\s\S]*?data-product-id="([0-9]+)"[\s\S]*?data-product-price="([0-9]+)"[\s\S]*?<div class="item-card__title"><a[\s\S]*?href="(https://booth\.pm/ja/items/[0-9]+)">

急にCTREの実行時間が増加します。
RE2もstd::regexに負けはじめます。

| 正規表現ライブラリ | 実行時間 (ns) |
| ------------------ | ------------: |
| std::regex         |   4836680.069 |
| ctre               |   4489967.655 |
| pcre               |   2039201.647 |
| pcre JIT           |    102986.508 |
| re2                |   5074009.584 |

![実行時間の比較](./images/pattern3.png)

### パターン4

> <li class="item-card[\s\S]*?data-product-id="([0-9]+)"[\s\S]*?data-product-price="([0-9]+)"[\s\S]*?<div class="item-card__title"><a


| 正規表現ライブラリ | 実行時間 (ns) |
| ------------------ | ------------: |
| std::regex         |   4652508.014 |
| ctre               |    148269.616 |
| pcre               |   2033980.156 |
| pcre JIT           |     89765.020 |
| re2                |   3538750.908 |

![実行時間の比較](./images/pattern4.png)

### パターン5


> <li class="item-card([\s\S]*?)data-product-id="([0-9]+)"[\s\S]*?data-product-price=

※プレースフォルダの数を合わせるために若干パターンを変更しています。

| 正規表現ライブラリ | 実行時間 (ns) |
| ------------------ | ------------: |
| std::regex         |   3757965.930 |
| ctre               |    132960.980 |
| pcre               |   1659222.615 |
| pcre JIT           |     42428.696 |
| re2                |    723658.063 |

![実行時間の比較](./images/pattern5.png)

## まとめ

1. PCRE2のJITコンパイルは非常に高速で、他のライブラリと比べて圧倒的なパフォーマンスを発揮しています。
2. std::regexは大抵のパターンで最も遅いですが、CTREのように極端に遅くなるパターンはほぼありません。
3. CTREほど極端ではありませんが、RE2もbacktrackingが発生するパターンでは性能低下が見られます。
4. 単純なパターンではCTREはPCRE2のJITに匹敵する性能を発揮します。
