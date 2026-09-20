# ZBinary2CArray

[English](README.md) | [简体中文](README.zh-CN.md) | [繁體中文](README.zh-TW.md)

`ZBinary2CArray` 可將二進位檔案轉換為 C/C++ 陣列。專案提供僅標頭檔的
C++20 函式庫，以及能產生可直接包含之原始碼檔案的命令列程式 `zbtca-cli`。

## 功能

- 支援匯出為 `unsigned char`（u8）、`unsigned short`（u16）、
  `unsigned int`（u32）或 `unsigned long long`（u64）陣列。
- 可產生獨立標頭檔（`.hpp`），或產生原始碼檔（`.cpp`）及附帶 `extern`
  宣告的配套標頭檔。
- 可設定儲存說明符（`none`、`static`、`inline`）與常數限定符
  （`none`、`const`、`constexpr`）。
- 可選的包含保護與整齊格式化的十六進位輸出。
- 註解支援：工具名稱、執行者名稱、來源檔案資訊、大小與時間戳——皆可單獨切換。
- 可設定每行元素數量（`0` = 依類型自動）。
- 自動建立不存在的輸出目錄。
- 單一可執行檔，無執行期依賴；MinGW 靜態連結 libstdc++/libgcc，
  MSVC 靜態連結 CRT。

## 環境需求

- 支援 C++20 的編譯器（GCC 13+、Clang 17+、MSVC 2022 17.8+）。
- 建置命令列程式需要 CMake 3.28.3 或更新版本。

## 專案結構

```
ZBinary2CArray/
├── CMakeLists.txt                    # 建置設定（CLI 目標）
├── main.cpp                          # CLI 入口（zbtca-cli）
├── ZBinary2CArray/                   # 僅標頭檔的 C++20 函式庫
│   ├── zbtca.h                       # 對外總頭檔
│   ├── types.hpp                     # OutputCfg、TypeFlags、AnnotationCfg
│   ├── bin.hpp                       # 二進位檔案讀取器（ZBTCA_Bin）
│   ├── output.hpp                    # C/C++ 陣列寫入器（ZBTCA_Output）
│   ├── response.hpp                  # 轉換回應（ZBTCA_Response）
│   ├── details.hpp                   # 內部實作細節
│   └── LICENSE.TXT
├── .github/workflows/cmake-multi-platform.yml   # CI/CD 管線
├── README.md
├── README.zh-CN.md
└── README.zh-TW.md
```

## 建置

```sh
cmake -S . -B build
cmake --build build
```

執行檔位於 `build/zbtca-cli`；使用常見多組態產生器時，通常位於
`build/Debug/zbtca-cli.exe`。

## 命令列用法

```text
zbtca-cli <input> [options]
```

將 `assets/logo.bin` 轉換為可包含的標頭檔：

```sh
zbtca-cli assets/logo.bin --output generated/logo.hpp --type u8 \
  --storage inline --const constexpr
```

預設會在輸入檔案旁產生僅標頭檔輸出，檔名為 `<input>.hpp`，例如
`assets/logo.bin.hpp`。使用 `--source` 可產生 `.cpp` 檔案與附帶的 `extern`
宣告標頭檔：

```sh
zbtca-cli assets/logo.bin --source --output generated/logo.cpp --type u32
```

### 選項

| 選項 | 說明 |
| --- | --- |
| `-o`, `--output <path>` | 輸出路徑；預設依輸入路徑產生。 |
| `-t`, `--type <type>` | 元素類型：`u8`、`u16`、`u32` 或 `u64`；預設為 `u8`。 |
| `--header-only` | 產生僅標頭檔輸出（預設）。 |
| `--source` | 產生原始碼檔及含 `extern` 宣告的標頭檔。 |
| `--no-inc-guard` | 不產生包含保護。 |
| `--storage <spec>` | 儲存說明符：`none`、`static` 或 `inline`。 |
| `--const <spec>` | 常數說明符：`none`、`const` 或 `constexpr`。 |
| `-n`, `--nums-per-line <n>` | 每行元素數；`0` 使用依類型設定的預設值。 |
| `--no-tidy` | 停用格式化的十六進位輸出。 |
| `--no-anno-tool`、`--no-anno-runner`、`--no-anno-file`、`--no-anno-size`、`--no-anno-time` | 停用對應的產生檔案註解。 |
| `--tool-name <name>` | 覆寫註解中的工具名稱。 |
| `--runner-name <name>` | 覆寫註解中的執行者名稱。 |
| `-h`, `--help` | 顯示命令說明（含專案連結）。 |

## 函式庫用法

包含 `ZBinary2CArray/zbtca.h`，使用 `ZBTCA_Bin` 讀取二進位檔案，再透過
`ZBTCA_Output` 寫出：

```cpp
#include "ZBinary2CArray/zbtca.h"

int main() {
    ZBTCA_Bin binary("assets/logo.bin");
    ZBTCA_Output output(binary);

    auto& config = output.Config();
    config.ExportTypeFlags = ZBTCA_Types::TypeFlags::u8;
    config.HeaderOnly = true;
    config.StorageSpecifier = ZBTCA_Types::OutputCfg::StorageSpecifier_inline;
    config.ConstSpecifier = ZBTCA_Types::OutputCfg::ConstSpecifier_constexpr;

    const ZBTCA_Response response = output("generated/logo.hpp");
    return response.status() ? 0 : 1;
}
```

產生的識別字由輸入檔名衍生；不符合 C/C++ 識別字規則的字元會替換為底線。

## CI/CD

GitHub Actions 工作流（`.github/workflows/cmake-multi-platform.yml`）
會在每次推送到 `master` 與每次 Pull Request 時，自動建置並測試 CLI，
產出涵蓋 **8 個目標平台** 的發行工件：

| 執行環境 | 架構 | 工件格式 |
| --- | --- | --- |
| `windows-latest` | x64 | `.zip` |
| `windows-latest` | x86 | `.zip` |
| `ubuntu-latest` | x64 | `.tar.gz` |
| `ubuntu-latest` | x86（multilib） | `.tar.gz` |
| `ubuntu-latest` | ARM32（交叉編譯） | `.tar.gz` |
| `ubuntu-24.04-arm` | ARM64（原生） | `.tar.gz` |
| `macos-15-intel` | x64 | `.tar.gz` |
| `macos-14` | ARM64（Apple Silicon） | `.tar.gz` |

當推送以 `v` 開頭的標籤（如 `v1.0.0`、`v1.0.1_p1`）時，
八個平台全部建置成功後，會自動建立 GitHub Release 並附加所有工件。
包含 `-` 的標籤會被標記為預發行版本。

## Related Projects & Community

- `https://t.me/ZCT_Studio` — Follow for project updates and announcements.

## 授權條款

本專案採用 [MIT 授權條款](LICENSE)。
