# ZBinary2CArray

[English](README.md) | [简体中文](README.zh-CN.md) | [繁體中文](README.zh-TW.md)

`ZBinary2CArray` converts a binary file into a C/C++ array. It provides a
header-only C++20 library and a command-line program (`zbtca-cli`) for
generating ready-to-include source files.

## Links

- [Telegram](https://t.me/ZCT_Studio)

## Features

- Export binary data as `unsigned char` (u8), `unsigned short` (u16),
  `unsigned int` (u32), or `unsigned long long` (u64) arrays.
- Generate either a self-contained header (`.hpp`) or a source file (`.cpp`)
  with a companion `extern` declaration header.
- Configurable storage specifier (`none`, `static`, `inline`) and const
  qualifier (`none`, `const`, `constexpr`).
- Optional include guard and tidy formatted hexadecimal output.
- Annotation support: tool name, runner name, source file info, size, and
  timestamp — all togglable.
- Configurable numbers per line (`0` = auto by type).
- Automatically creates missing output directories.
- Single executable, no runtime dependencies; static libstdc++/libgcc on
  MinGW and static CRT on MSVC.

## Requirements

- A C++20-compatible compiler (GCC 13+, Clang 17+, MSVC 2022 17.8+).
- CMake 3.28.3 or newer to build the command-line program.

## Project Structure

```
ZBinary2CArray/
├── CMakeLists.txt                    # Build configuration (CLI target)
├── main.cpp                          # CLI entry point (zbtca-cli)
├── ZBinary2CArray/                   # Header-only C++20 library
│   ├── zbtca.h                       # Public umbrella header
│   ├── types.hpp                     # OutputCfg, TypeFlags, AnnotationCfg
│   ├── bin.hpp                       # Binary file reader (ZBTCA_Bin)
│   ├── output.hpp                    # C/C++ array writer (ZBTCA_Output)
│   ├── response.hpp                  # Conversion response (ZBTCA_Response)
│   ├── details.hpp                   # Internal implementation details
│   └── LICENSE.TXT
├── .github/workflows/cmake-multi-platform.yml   # CI/CD pipeline
├── README.md
├── README.zh-CN.md
└── README.zh-TW.md
```

## Build

```sh
cmake -S . -B build
cmake --build build
```

The executable is written to `build/zbtca-cli` (or `build/Debug/zbtca-cli.exe`
for common multi-configuration generators).

## Command-line usage

```text
zbtca-cli <input> [options]
```

Convert `assets/logo.bin` to an includeable header:

```sh
zbtca-cli assets/logo.bin --output generated/logo.hpp --type u8 \
  --storage inline --const constexpr
```

By default, header-only output is generated beside the input file using the
name `<input>.hpp`; for example, `assets/logo.bin.hpp`. Use `--source` to
generate a `.cpp` file and a companion `extern` header:

```sh
zbtca-cli assets/logo.bin --source --output generated/logo.cpp --type u32
```

### Options

| Option | Description |
| --- | --- |
| `-o`, `--output <path>` | Output path. Defaults to a name derived from the input. |
| `-t`, `--type <type>` | Element type: `u8`, `u16`, `u32`, or `u64`; defaults to `u8`. |
| `--header-only` | Generate a header-only output file (default). |
| `--source` | Generate a source file and an `extern` declaration header. |
| `--no-inc-guard` | Do not emit an include guard. |
| `--storage <spec>` | Storage specifier: `none`, `static`, or `inline`. |
| `--const <spec>` | Const specifier: `none`, `const`, or `constexpr`. |
| `-n`, `--nums-per-line <n>` | Elements per line; `0` selects the type-specific default. |
| `--no-tidy` | Disable formatted hexadecimal output. |
| `--no-anno-tool`, `--no-anno-runner`, `--no-anno-file`, `--no-anno-size`, `--no-anno-time` | Disable the corresponding generated-file annotation. |
| `--tool-name <name>` | Override the tool name in annotations. |
| `--runner-name <name>` | Override the runner name in annotations. |
| `-h`, `--help` | Print command help (includes project links). |

## Library usage

Include `ZBinary2CArray/zbtca.h`, load a binary file with `ZBTCA_Bin`, and
write it through `ZBTCA_Output`:

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

The generated identifier is derived from the input filename; characters that
are invalid in C/C++ identifiers are replaced with underscores.

## CI/CD

The GitHub Actions workflow (`.github/workflows/cmake-multi-platform.yml`)
automatically builds and tests the CLI on every push to `master` and every
pull request, producing release artifacts for **8 targets**:

| Runner | Architecture | Artifact |
| --- | --- | --- |
| `windows-latest` | x64 | `.zip` |
| `windows-latest` | x86 | `.zip` |
| `ubuntu-latest` | x64 | `.tar.gz` |
| `ubuntu-latest` | x86 (multilib) | `.tar.gz` |
| `ubuntu-latest` | ARM32 (cross) | `.tar.gz` |
| `ubuntu-24.04-arm` | ARM64 (native) | `.tar.gz` |
| `macos-15-intel` | x64 | `.tar.gz` |
| `macos-14` | ARM64 (Apple Silicon) | `.tar.gz` |

When a tag starting with `v` (e.g., `v1.0.0`, `v1.0.1_p1`) is pushed, all
eight platforms must build successfully before a GitHub Release is created
and all artifacts are attached. Tags containing `-` are marked as prerelease.

## License

This project is released under the [MIT License](LICENSE).
