# EdgeInfer

Embedded inference framework for ARM Cortex-M microcontrollers — a from-scratch C framework that compiles neural network models into deterministic, statically-allocated inference pipelines. No OS, no heap, no stdio. Runs bare-metal on QEMU mps2-an385 (Cortex-M3).

## Key Features

| Feature | Description |
|---------|-------------|
| **Static Memory Pool** | Zero `malloc`/`free` — pool sized at compile time from model metadata. O(1) bump allocator with overflow detection. |
| **Pipeline Architecture** | Preprocess → inference → postprocess stages with error propagation and automatic buffer management. |
| **User Extension Hooks** | Custom pre/post processing injected via function pointers. No framework modification needed. |
| **Offline Model Compilation** | ONNX models converted to C headers via `tools/onnx_to_c.py`. Zero runtime dependencies. |
| **Custom UART Engine** | printf-style formatting (`%s`, `%d`, `%u`, floats) with no libc dependency. Bare-metal serial output. |
| **QEMU Emulation** | Run and debug on emulated mps2-an385 (Cortex-M3). GDB server on port 1234. |
| **Golden-Output Verification** | Automated CI-style check: build → QEMU run → float extraction → diff against reference. |

## Prerequisites

- `arm-none-eabi-gcc` (ARM cross-compiler toolchain)
- `qemu-system-arm` (for emulation)
- Python 3 with `onnx` and `numpy` (for model conversion)

## Quick Start

```bash
# Build and run
make -f scripts/Makefile
./qemu/run.sh

# Or one step
./scripts/demo.sh

# Automated verification
bash scripts/verify_qemu.sh
```

Expected output (2-layer MLP: 4 → 8 → 4 with ReLU):

```
11.946224
0.678968
-3.791145
1.372610
```

## What Runs by Default

The default simulation executes inference on a simple MLP, validating the full pipeline:

1. `pipeline_init()` — initializes logger (UART), static memory pool, and buffer layout from generated model metadata
2. `user_register_hooks()` — registers user preprocess and postprocess hooks
3. `pipeline_run()` — orchestrates all three stages:
   - `stage_preprocess()` — calls user preprocess hook (passthrough if unregistered)
   - `stage_inference()` — default engine iterates Dense/ReLU layers from model topology
   - `stage_postprocess()` — calls user postprocess hook (passthrough if unregistered)
4. Output tensor printed via UART float formatting
5. Pool reset between runs — deterministic, reusable

## Project Structure

```
src/                     Core framework source
├── common/              Types (tensor_t, tensor_shape_t), logger, utilities
├── pipeline/            Pipeline orchestration (init + run)
├── stages/              Per-stage logic: preprocess, inference engine, postprocess
├── memory/              Static memory pool + buffer layout manager
├── hooks/               Hook interface typedefs + registry
├── model/               Generated model headers (weights, topology, macros)
└── main.c               Entry point — init, register hooks, run pipeline

config/                  Compile-time configuration (TARGET_RAM_SIZE, logging levels)
platform/arm/            ARM-specific: startup.s, linker.ld, UART driver
user_extensions/         User-provided preprocessing, postprocessing, config
tools/                   onnx_to_c.py — ONNX → C header converter
qemu/                    QEMU run/debug scripts
scripts/                 Build and verification entry points
examples/simple_mlp/     Reference MLP model with golden output
docs/                    Architecture, memory model, and integration documentation
```

## Architecture

```
   ┌──────────────┐
   │  model.onnx  │
   └──────┬───────┘
          │ python3 tools/onnx_to_c.py
          ▼
   ┌──────────────┐     ┌──────────────────┐
   │  model.h     │     │  model_meta.h    │
   │  (weights,   │     │  (layer topology,│
   │   biases,    │     │   sizing macros) │
   │   metadata)  │     └──────────────────┘
   └──────────────┘
          │
          ▼  arm-none-eabi-gcc
   ┌──────────────┐
   │  build/      │
   │  app.elf     │
   └──────┬───────┘
          │ qemu-system-arm
          ▼
   ┌──────────────────┐
   │  Cortex-M3       │
   │  mps2-an385      │
   │  pre→inf→post    │
   └──────────────────┘
```

## Adding a New Model

```bash
# 1. Convert ONNX to C headers
python3 tools/onnx_to_c.py your_model.onnx

# 2. Update user config dimensions in user_extensions/config/user_config.h

# 3. Rebuild
make -f scripts/Makefile
```

## Customizing Pre/Post Processing

```c
// user_extensions/preprocess/user_preprocess.c
static edgeinfer_status_t my_preprocess(
    const tensor_t *raw_input, tensor_t *prepared_input)
{
    // Normalize, resize, reorder channels...
    return EDGEINFER_OK;
}
```

Both hooks registered from `user_register_hooks()` in `user_preprocess.c`. Hooks fall through to passthrough if unregistered.

## Debugging

```bash
# Terminal 1: start QEMU with GDB server
./qemu/debug.sh

# Terminal 2: connect GDB
arm-none-eabi-gdb build/app.elf
(gdb) target remote :1234
(gdb) break main
(gdb) continue
```

## Documentation

- [Architecture](docs/architecture.md) — Module dependencies, type system, control flow, build system
- [Memory Model](docs/memory_model.md) — Static pool design, buffer layout, memory safety
- [Integration Guide](docs/integration_guide.md) — How to add models, write hooks, and tune for your target

## License

MIT. See [LICENSE](LICENSE).
