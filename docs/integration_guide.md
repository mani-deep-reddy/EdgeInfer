# Integration Guide

## Adding Your Model

1. **Convert your model**:
   ```bash
   python3 tools/onnx_to_c.py your_model.onnx
   ```
   This overwrites `src/model/model.h` and `src/model/model_meta.h`.

2. **Update dimensions** in `user_extensions/config/user_config.h`.

3. **Rebuild**:
   ```bash
   make clean && make
   ```

## Writing a Preprocess Hook

Edit `user_extensions/preprocess/user_preprocess.c`:

```c
static edgeinfer_status_t my_preprocess(const tensor_t *raw_input, tensor_t *prepared_input) {
    // Transform raw_input → prepared_input
    // e.g., normalize, resize, channel reordering
    return EDGEINFER_OK;
}
```

Register it in `user_register_hooks()`.

## Writing a Postprocess Hook

Edit `user_extensions/postprocess/user_postprocess.c`:

```c
edgeinfer_status_t my_postprocess(const tensor_t *raw_output, tensor_t *formatted_output) {
    // Transform raw_output → formatted_output
    // e.g., softmax, argmax, thresholding
    return EDGEINFER_OK;
}
```

Register it in `user_register_hooks()`.

## Hook Registration

Both hooks are registered from `user_extensions/preprocess/user_preprocess.c`:

```c
void user_register_hooks(void) {
    hook_register_preprocess(my_preprocess);
    hook_register_postprocess(my_postprocess);
}
```

This function is called from `main()` immediately after `pipeline_init()`.

## Pipeline Stages

The framework calls each stage in order:

```
pipeline_run(input, output)
 ├─ stage_preprocess()   → calls preprocess hook (or passthrough)
 ├─ stage_inference()    → runs default engine (Dense/ReLU)
 └─ stage_postprocess()  → calls postprocess hook (or passthrough)
```

Each stage automatically falls back to a passthrough if no hook is registered.

## Tuning Buffer Sizes

The memory pool is sized automatically from model metadata during model conversion. The generated `model_meta.h` defines these sizing macros:

| Macro                        | Source                      |
|------------------------------|-----------------------------|
| `MODEL_INPUT_SIZE`           | Converted model input shape |
| `MODEL_OUTPUT_SIZE`          | Converted model output shape|
| `MODEL_MAX_ACTIVATIONS_SIZE` | Largest layer activation    |
| `MODEL_MAX_INTERMEDIATE_SIZE`| Intermediate storage        |

You do not need to tune these manually — they are derived from the ONNX model.

For hardware constraints, adjust `config/runtime_config.h`:

- `TARGET_RAM_SIZE` — total RAM available on your target (e.g., 1 MB)
- `POOL_HEADROOM` — reserved for stack, `.data`, `.bss` overhead
