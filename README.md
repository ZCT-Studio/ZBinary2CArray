# ZBinary2CArray

[简体中文](README.zh-CN.md) · [繁體中文](README.zh-TW.md) · **English**

`ZBinary2CArray` converts a binary file into a C/C++ array. It provides a
header-only C++20 library and a command-line program (`zbtca-cli`) for
generating ready-to-include source files.

## Features

- Export binary data as `unsigned char`, `unsigned short`, `unsigned int`, or
  `unsigned long long` arrays.
- Generate either a self-contained header or a source file with an `extern`
  declaration header.
- Configure storage and const specifiers, include guards, line wrapping, and
  generated-file annotations.
- Automatically creates missing output directories.

## Requirements

- A C++20-compatible compiler.
- CMake 3.31.6 or newer to build the command-line program.

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
| `-h`, `--help` | Print command help. |

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

## License

This project is released under the [MIT License](LICENSE).
