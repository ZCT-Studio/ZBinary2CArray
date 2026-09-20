# ZBinary2CArray

[English](README.md) | [简体中文](README.zh-CN.md) | [繁體中文](README.zh-TW.md)

`ZBinary2CArray` 用于将二进制文件转换为 C/C++ 数组。项目提供仅头文件的
C++20 库，以及可生成可直接包含源码文件的命令行程序 `zbtca-cli`。

## 链接

- [Telegram](https://t.me/ZCT_Studio)

## 特性

- 支持导出为 `unsigned char`（u8）、`unsigned short`（u16）、
  `unsigned int`（u32）或 `unsigned long long`（u64）数组。
- 可生成独立头文件（`.hpp`），或生成源文件（`.cpp`）及带有 `extern`
  声明的配套头文件。
- 可配置存储说明符（`none`、`static`、`inline`）和常量限定符
  （`none`、`const`、`constexpr`）。
- 可选的包含保护及整洁的十六进制格式化输出。
- 注释支持：工具名称、运行者名称、源文件信息、大小及时间戳——均可单独开关。
- 可配置每行元素数量（`0` = 按类型自动）。
- 自动创建不存在的输出目录。
- 单文件可执行程序，无运行时依赖；MinGW 静态链接 libstdc++/libgcc，
  MSVC 静态链接 CRT。

## 环境要求

- 支持 C++20 的编译器（GCC 13+、Clang 17+、MSVC 2022 17.8+）。
- 构建命令行程序需要 CMake 3.28.3 或更高版本。

## 项目结构

```
ZBinary2CArray/
├── CMakeLists.txt                    # 构建配置（CLI 目标）
├── main.cpp                          # CLI 入口（zbtca-cli）
├── ZBinary2CArray/                   # 仅头文件的 C++20 库
│   ├── zbtca.h                       # 对外总码头文件
│   ├── types.hpp                     # OutputCfg、TypeFlags、AnnotationCfg
│   ├── bin.hpp                       # 二进制文件读取器（ZBTCA_Bin）
│   ├── output.hpp                    # C/C++ 数组写入器（ZBTCA_Output）
│   ├── response.hpp                  # 转换响应（ZBTCA_Response）
│   ├── details.hpp                   # 内部实现细节
│   └── LICENSE.TXT
├── .github/workflows/cmake-multi-platform.yml   # CI/CD 流水线
├── README.md
├── README.zh-CN.md
└── README.zh-TW.md
```

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
| `-h`, `--help` | 显示命令帮助（含项目链接）。 |

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

## CI/CD

GitHub Actions 工作流（`.github/workflows/cmake-multi-platform.yml`）
会在每次推送到 `master` 和每次 Pull Request 时，自动构建并测试 CLI，
产出覆盖 **8 个目标平台** 的发布工件：

| 运行环境 | 架构 | 工件格式 |
| --- | --- | --- |
| `windows-latest` | x64 | `.zip` |
| `windows-latest` | x86 | `.zip` |
| `ubuntu-latest` | x64 | `.tar.gz` |
| `ubuntu-latest` | x86（multilib） | `.tar.gz` |
| `ubuntu-latest` | ARM32（交叉编译） | `.tar.gz` |
| `ubuntu-24.04-arm` | ARM64（原生） | `.tar.gz` |
| `macos-15-intel` | x64 | `.tar.gz` |
| `macos-14` | ARM64（Apple Silicon） | `.tar.gz` |

当推送以 `v` 开头的标签（如 `v1.0.0`、`v1.0.1_p1`）时，
八个平台全部构建成功后，会自动创建 GitHub Release 并附加所有工件。
包含 `-` 的标签会被标记为预发布版本。

## 许可证

本项目采用 [MIT 许可证](LICENSE)。
