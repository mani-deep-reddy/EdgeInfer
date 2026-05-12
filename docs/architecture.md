# Architecture

## Module Dependency Graph

```
main.c
  └─ pipeline.h → pipeline.c
       ├─ model.h (generated)
       │    ├─ model_meta.h (generated)
       │    │    └─ types.h
       │    └─ g_weights, g_biases, g_model_meta
       ├─ types.h           (common/)
       ├─ logger.h → logger.c
       │    └─ uart.h → uart.c  (platform/)
       ├─ memory_pool.h → memory_pool.c
       │    └─ model_meta.h, runtime_config.h
       ├─ buffer_layout.h → buffer_layout.c
       │    └─ model.h, utils.h
       ├─ stage_preprocess (stages/preprocess.c)
       │    └─ hook_interface.h → hook_registry.c
       ├─ stage_inference  (stages/inference.c)
       │    └─ model.h, utils.h
       └─ stage_postprocess (stages/postprocess.c)
            └─ hook_interface.h → hook_registry.c
```

## Toolchain Flow

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
   ┌──────────────┐
   │  QEMU        │
   │  mps2-an385  │
   │  (Cortex-M3) │
   └──────────────┘
```

## Pipeline Stages

```
raw_input → [Preprocess] → prepared_input → [Inference] → raw_output → [Postprocess] → formatted_output
```

### Preprocess Stage

Calls the user-registered `preprocess_fn_t` hook via `hook_get_preprocess()`. If no hook is registered, input passes through unchanged (pointer copy).

### Inference Stage

Default engine reads `g_model_meta` layer topology and executes layers sequentially:

1. **DENSE** (fully connected): `y = Wx + b` — matrix multiply with bias accumulation
2. **RELU**: `y = max(0, x)` — element-wise, in-place safe

The engine chains activations through the workspace buffer, writing the final layer directly to the output buffer.

### Postprocess Stage

Calls the user-registered `postprocess_fn_t` hook via `hook_get_postprocess()`. Handles output formatting (softmax, argmax, thresholding). Falls back to passthrough if unregistered.

## Control Flow

```
main()
 ├─ pipeline_init()
 │    ├─ logger_init()
 │    │    └─ uart_init()
 │    └─ memory_pool_init()
 │         └─ zero-fills static pool
 ├─ user_register_hooks()
 │    ├─ hook_register_preprocess()
 │    └─ hook_register_postprocess()
 └─ pipeline_run(input, output)
      ├─ stage_preprocess()
      ├─ stage_inference()
      ├─ stage_postprocess()
      └─ memory_pool_reset()
```

## Type System

### Core Types (`src/common/types.h`)

| Type | Description |
|------|-------------|
| `tensor_shape_t` | N-dimensional shape (`ndim` + `dims[4]`) |
| `tensor_t` | Float tensor (`data` pointer + `shape`) |
| `layer_type_t` | Enum: `LAYER_DENSE`, `LAYER_RELU`, `LAYER_CONV2D`, `LAYER_MAXPOOL`, `LAYER_SOFTMAX`, `LAYER_UNKNOWN` |
| `edgeinfer_status_t` | Error code enum |

### Error Codes

| Code | Value | Meaning |
|------|-------|---------|
| `EDGEINFER_OK` | 0 | Success |
| `EDGEINFER_ERR` | -1 | Generic error |
| `EDGEINFER_ERR_OOM` | -2 | Out of memory |
| `EDGEINFER_ERR_INPUT` | -3 | Invalid input |

### Model Metadata (`src/model/model_meta.h`)

| Type | Description |
|------|-------------|
| `layer_meta_t` | Layer descriptor (type, shapes, weight/bias offsets) |
| `model_meta_t` | Model topology (layer array, input/output shapes) |
| `memory_requirements_t` | Sizing summary (total weights, max activations) |

### Hook Interface (`src/hooks/hook_interface.h`)

| Signature | Purpose |
|-----------|---------|
| `preprocess_fn_t` | `edgeinfer_status_t (*)(const tensor_t *raw, tensor_t *prepared)` |
| `postprocess_fn_t` | `edgeinfer_status_t (*)(const tensor_t *raw, tensor_t *formatted)` |
| `inference_fn_t` | `edgeinfer_status_t (*)(const tensor_t *in, tensor_t *out, const void *ctx)` — reserved |

## Build System

The Makefile at `scripts/Makefile` compiles for ARM Cortex-M3:

| Component | Detail |
|-----------|--------|
| Target CPU | `cortex-m3`, Thumb mode |
| Toolchain | `arm-none-eabi-gcc` |
| Linker script | `platform/arm/linker.ld` (FLASH: 4 MB, RAM: 1 MB) |
| Startup code | `platform/arm/startup.s` (.data copy + .bss zero) |
| Binary output | `build/app.elf` |

Source files are auto-discovered by wildcard — no need to update the Makefile when adding new `.c` files within existing directories.
