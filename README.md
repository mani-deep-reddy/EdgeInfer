# EdgeInfer

A lightweight, deterministic inference framework for ARM embedded devices.

## Overview

EdgeInfer runs neural network inference on microcontrollers with:
- **Zero dynamic allocation** — all memory is statically pre-allocated
- **Pipeline execution** — preprocess → inference → postprocess
- **User extensions** — plug in custom logic via function pointers
- **Compile-time model** — ONNX models converted to C headers offline

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
src/              — Core framework (pipeline, stages, memory, hooks)
config/           — Compile-time configuration
platform/arm/     — ARM-specific code (startup, linker, UART)
user_extensions/  — User-provided hooks and config
tools/            — Offline utilities (ONNX → C converter)
qemu/             — Emulation scripts
scripts/          — Build entry points
examples/         — Reference implementations
docs/             — Design and integration documentation
```

## Model Conversion

Convert an ONNX model to C headers:

```bash
pip install onnx numpy
python3 tools/onnx_to_c.py your_model.onnx
```

This generates `src/model/model.h` and `src/model/model_meta.h`.

## Extending EdgeInfer

Replace files in `user_extensions/` to customize:
- `preprocess/user_preprocess.c` — Input preparation
- `postprocess/user_postprocess.c` — Output formatting
- `inference_override/user_infer.c` — Custom inference engine
- `config/user_config.h` — Model-specific parameters

## License

See [LICENSE](LICENSE).
