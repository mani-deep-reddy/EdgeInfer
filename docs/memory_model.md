# Memory Model

## Design Philosophy

EdgeInfer uses **static allocation with buffer reuse** instead of dynamic memory management. This eliminates:
- Heap fragmentation
- Non-deterministic allocation latency
- Memory leaks
- Out-of-memory errors at runtime

## Memory Pool

The entire working memory is a single statically-declared byte array, sized at compile time from the converted model's metadata:

```c
static uint8_t static_pool[
    MODEL_INPUT_SIZE +
    MODEL_OUTPUT_SIZE +
    MODEL_MAX_ACTIVATIONS_SIZE +
    MODEL_MAX_INTERMEDIATE_SIZE
] __attribute__((aligned(4)));
```

The sizing macros (`MODEL_INPUT_SIZE`, `MODEL_OUTPUT_SIZE`, etc.) are defined in `src/model/model_meta.h`, which is **generated** by `tools/onnx_to_c.py`. This means the pool is exactly as large as the model needs — no wasted space.

### Allocation Strategy

- `memory_pool_alloc()` bumps a pointer — O(1), no free list
- `memory_pool_reset()` zeros the used counter between runs
- No individual buffer frees needed — the pool is reset wholesale

## Buffer Layout

After pool initialization, `buffer_layout_init()` carves the pool into three regions based on the model's input/output shapes from `g_model_meta`:

| Region    | Purpose                          | Source                                |
|-----------|----------------------------------|---------------------------------------|
| Input     | Preprocessed input tensor        | `g_model_meta.model_input_shape`      |
| Output    | Final output tensor              | `g_model_meta.model_output_shape`     |
| Workspace | Intermediate activations         | Remaining pool space                  |

Region sizes are computed from the model's actual tensor shapes (`tensor_num_elements()` × sizeof(float)), not from hardcoded constants. This means the layout automatically adapts to whatever model is loaded.

## Buffer Reuse

For simple models, input and inference output can share the same buffer when input dimensions match output dimensions. The `buffer_layout_t` descriptor tracks region offsets and is used by pipeline stages to locate their working buffers.

## Memory Safety

- `memory_pool_init()` zero-fills the entire pool on startup
- `memory_pool_alloc()` returns NULL on overflow — never silently corrupts
- A compile-time warning is emitted if `sizeof(static_pool)` exceeds `TARGET_RAM_SIZE - POOL_HEADROOM`
- Pool is reset between inference runs, preventing inter-run contamination
