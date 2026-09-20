# ZBinary2CArray

[English](README.md) | [简体中文](README.zh-CN.md) | [繁體中文](README.zh-TW.md)

`ZBinary2CArray` 用于将二进制文件转换为 C/C++ 数组。项目提供仅头文件的
C++20 库，以及可生成可直接包含源码文件的命令行程序 `zbtca-cli`。

## 链接

- [GitHub](https://github.com/ZCT-Studio/ZBinary2CArray)
- [Telegram](https://t.me/ZCT_Studio)

## 特性

- 支持导出为 `unsigned char`、`unsigned short`、`unsigned int` 或
  `unsigned long long` 数组。
- 可生成独立头文件，或生成源文件及带有 `extern` 声明的配套头文件。
- 可配置存储说明符、常量说明符、包含保护、换行数量和生成文件注释。
- 自动创建不存在的输出目录。

## 环境要求

- 支持 C++20 的编译器。
- 构建命令行程序需要 CMake 3.28.3 或更高版本。

## 构建

```sh
cmake -S . -B build
cmake --build build
```

可执行文件位于 `build/zbtca-cli`；使用常见多配置生成器时，通常位于
`build/Debug/zbtca-cli.exe`。

## 命令行用法

```text
zbtca-cli <input> [options]
```

将 `assets/logo.bin` 转换为可包含的头文件：

```sh
zbtca-cli assets/logo.bin --output generated/logo.hpp --type u8 \
  --storage inline --const constexpr
```

默认会在输入文件旁生成仅头文件输出，文件名为 `<input>.hpp`，例如
`assets/logo.bin.hpp`。使用 `--source` 可生成 `.cpp` 文件及配套的 `extern`
声明头文件：

```sh
zbtca-cli assets/logo.bin --source --output generated/logo.cpp --type u32
```

### 选项

| 选项 | 说明 |
| --- | --- |
| `-o`, `--output <path>` | 输出路径；默认根据输入路径生成。 |
| `-t`, `--type <type>` | 元素类型：`u8`、`u16`、`u32` 或 `u64`；默认 `u8`。 |
| `--header-only` | 生成仅头文件输出（默认）。 |
| `--source` | 生成源文件及含 `extern` 声明的头文件。 |
| `--no-inc-guard` | 不生成包含保护。 |
| `--storage <spec>` | 存储说明符：`none`、`static` 或 `inline`。 |
| `--const <spec>` | 常量说明符：`none`、`const` 或 `constexpr`。 |
| `-n`, `--nums-per-line <n>` | 每行元素数；`0` 使用按类型设定的默认值。 |
| `--no-tidy` | 禁用格式化的十六进制输出。 |
| `--no-anno-tool`、`--no-anno-runner`、`--no-anno-file`、`--no-anno-size`、`--no-anno-time` | 禁用对应的生成文件注释。 |
| `--tool-name <name>` | 覆盖注释中的工具名称。 |
| `--runner-name <name>` | 覆盖注释中的运行者名称。 |
| `-h`, `--help` | 显示命令帮助。 |

## 库用法

包含 `ZBinary2CArray/zbtca.h`，使用 `ZBTCA_Bin` 读取二进制文件，再通过
`ZBTCA_Output` 写出：

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

生成的标识符由输入文件名派生；不符合 C/C++ 标识符规则的字符会被替换为下划线。

## 许可证

本项目采用 [MIT 许可证](LICENSE)。
