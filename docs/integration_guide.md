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
static edgeinfer_status_t my_postprocess(const tensor_t *raw_output, tensor_t *formatted_output) {
    // Transform raw_output → formatted_output
    // e.g., softmax, argmax, thresholding
    return EDGEINFER_OK;
}
```

## Overriding Inference

For full control, implement `user_extensions/inference_override/user_infer.c` and register via `hook_register_inference()`. This bypasses the default engine entirely.

## Tuning Buffer Sizes

Edit `config/runtime_config.h`:
- `MAX_INPUT_BUFFER_SIZE` — must fit your largest input tensor
- `MAX_OUTPUT_BUFFER_SIZE` — must fit your largest output tensor
- `MAX_WORKSPACE_SIZE` — must fit intermediate activations
