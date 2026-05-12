# EdgeInfer

**Embedded inference framework for ARM Cortex-M microcontrollers.**

EdgeInfer is a from-scratch C framework that compiles neural network models into
deterministic, statically-allocated inference pipelines for ARM Cortex-M3 targets.
No OS, no heap, no stdio — runs bare-metal on QEMU mps2-an385.

## Highlights

- **Static memory pool** — zero `malloc`/`free`. Pool sized at compile time from model metadata. O(1) bump allocator with overflow detection.
- **Pipeline architecture** — preprocess → inference → postprocess stages with error propagation and automatic buffer management.
- **User extension hooks** — custom pre/post processing injected via function pointers. No framework modification needed.
- **Offline model compilation** — ONNX models converted to C headers via `tools/onnx_to_c.py`. Zero runtime dependencies.
- **Custom UART engine** — printf-style formatting (`%s`, `%d`, `%u`, floats) with no libc dependency. Bare-metal serial output.
- **QEMU support** — runs on emulated mps2-an385 (Cortex-M3). GDB debug server available on port 1234.

## Quick Start

```bash
# Prerequisites
sudo apt install gcc-arm-none-eabi qemu-system-arm
pip install onnx numpy

# Build and run
make -f scripts/Makefile
./qemu/run.sh
```

Expected output (inference on a 2-layer MLP: 4→8→4 with ReLU):

```
11.946224
0.678968
-3.791145
1.372610
```

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

config/                  Compile-time configuration (TARGET_RAM_SIZE, logging)
platform/arm/            ARM-specific: startup.s, linker.ld, UART driver
user_extensions/         User-provided preprocessing, postprocessing, config
tools/                   onnx_to_c.py — ONNX → C header converter
qemu/                    QEMU run/debug scripts
scripts/                 Build and verification entry points
examples/simple_mlp/     Reference MLP model with golden output
docs/                    Architecture, memory model, integration guide
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
                             │ qemu-system-arm (or real hardware)
                             ▼
                      ┌──────────────┐
                      │  Cortex-M3   │
                      │  Pipeline    │
                      │  pre→inf→post│
                      └──────────────┘
```

## Adding a New Model

```bash
# 1. Convert ONNX to C headers
python3 tools/onnx_to_c.py your_model.onnx

# 2. Update user config dimensions
#    (edit user_extensions/config/user_config.h)

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

Register hooks in `user_register_hooks()`. Fall through to passthrough if unregistered.

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

## Verification

```bash
bash scripts/verify_qemu.sh
```

Automated CI-style check: clean build → QEMU run → float output extraction → diff against golden reference.

## Supported Layers

| Layer    | Status      |
|----------|-------------|
| Dense    | Implemented |
| ReLU     | Implemented |
| Conv2D   | Planned     |
| MaxPool  | Planned     |
| Softmax  | Planned     |

## License

MIT. See [LICENSE](LICENSE).
