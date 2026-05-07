# Memory Model

## Design Philosophy

EdgeInfer uses **static allocation with buffer reuse** instead of dynamic memory management. This eliminates:
- Heap fragmentation
- Non-deterministic allocation latency
- Memory leaks
- Out-of-memory errors at runtime

## Memory Pool

The entire working memory is a single statically-declared byte array:

```c
static uint8_t static_pool[MAX_INPUT + MAX_OUTPUT + MAX_WORKSPACE];
```

### Allocation Strategy

- `memory_pool_alloc()` bumps a pointer — O(1), no free list
- `memory_pool_reset()` zeros the used counter between runs
- No individual buffer frees needed — the pool is reset wholesale

### Buffer Layout

| Region    | Purpose                          | Size (default) |
|-----------|----------------------------------|----------------|
| Input     | Preprocessed input tensor        | 1 MB           |
| Output    | Final output tensor              | 1 MB           |
| Workspace | Intermediate activations         | 2 MB           |

Sizes are defined in `config/runtime_config.h`.

## Buffer Reuse

For simple models, input and inference output can share the same buffer when input dimensions match output dimensions. The `buffer_layout_t` descriptor tracks region offsets.
