# EdgeInfer

[![CI](https://github.com/mani-deep-reddy/EdgeInfer/actions/workflows/ci.yml/badge.svg)](https://github.com/mani-deep-reddy/EdgeInfer/actions/workflows/ci.yml)

A lightweight, deterministic inference framework for ARM microcontrollers
(Cortex-M3, QEMU mps2-an385).

## Features

- **Zero dynamic allocation** — static memory pool sized at compile time
- **Pipeline execution** — preprocess → inference → postprocess with error propagation
- **User extension hooks** — plug in custom pre/post processing via function pointers
- **Compile-time model** — ONNX models converted to C headers offline
- **Custom UART output** — no stdio dependency, float printing for embedded targets
- **QEMU emulation** — run and debug without hardware (mps2-an385)
- **CI-verified** — GitHub Actions builds and golden-output validation on every push

## Prerequisites

- `arm-none-eabi-gcc` (ARM cross-compiler toolchain)
- `qemu-system-arm` (for emulation)
- Python 3 with `onnx` and `numpy` (for model conversion)

## Quick Start

```bash
# Build
make -f scripts/Makefile

# Run in QEMU
./qemu/run.sh

# Build + run in one step
./scripts/demo.sh
```

## Project Structure

```
src/                     Core framework
├── common/              Types, logger, utilities
├── pipeline/            Pipeline orchestration (init + run)
├── stages/              Preprocess, inference, postprocess
├── memory/              Static pool + buffer layout
├── hooks/               Hook interface + registry
├── model/               Generated model data (model.h, model_meta.h)
└── main.c               Application entry point

config/                  Compile-time configuration
platform/arm/            ARM-specific code (startup, linker, UART)
user_extensions/         User-provided hooks and config
├── preprocess/          Input preparation hook
├── postprocess/         Output formatting hook
└── config/              Model-specific parameters

tools/                   Offline utilities (ONNX → C converter)
qemu/                    Emulation scripts (run + debug)
scripts/                 Build entry points + verification
examples/                Reference implementations
docs/                    Design and integration documentation
```

## Model Conversion

Convert an ONNX model to C headers:

```bash
pip install onnx numpy
python3 tools/onnx_to_c.py your_model.onnx
```

This generates `src/model/model.h` (weights, biases, metadata) and
`src/model/model_meta.h` (layer topology, sizing macros).

## Extending EdgeInfer

Replace files in `user_extensions/` to customize:

| File | Purpose |
|------|---------|
| `preprocess/user_preprocess.c` | Input preparation (normalize, resize, etc.) |
| `postprocess/user_postprocess.c` | Output formatting (softmax, argmax, etc.) |
| `config/user_config.h` | Model-specific parameters |

Hooks are registered from `user_register_hooks()` in `user_preprocess.c`.

## Debugging with QEMU

```bash
# Start QEMU in GDB server mode (port 1234)
./qemu/debug.sh

# In another terminal, connect GDB
arm-none-eabi-gdb build/app.elf
(gdb) target remote :1234
(gdb) break main
(gdb) continue
```

## Verification

```bash
# Build, run in QEMU, compare output against golden reference
bash scripts/verify_qemu.sh
```

This script:
1. Clean builds the project
2. Runs in QEMU with a 5-second timeout
3. Extracts float output from serial
4. Diffs against `examples/simple_mlp/expected_output.txt`

## Supported Layer Types

| Layer    | Status      |
|----------|-------------|
| Dense    | Implemented |
| ReLU     | Implemented |
| Conv2D   | Reserved    |
| MaxPool  | Reserved    |
| Softmax  | Reserved    |

## License

See [LICENSE](LICENSE).
