# Architecture

## Pipeline Stages

```
raw_input → [Preprocess] → prepared_input → [Inference] → raw_output → [Postprocess] → formatted_output
```

### Preprocess Stage
Calls the user-registered `preprocess_fn_t` hook. If no hook is registered, input passes through unchanged.

### Inference Stage
Two execution paths:
1. **User override**: If `inference_fn_t` is registered, it runs instead of the default engine.
2. **Default engine**: Reads `model_meta_t` topology and executes layers sequentially using weights from `model.h`.

### Postprocess Stage
Calls the user-registered `postprocess_fn_t` hook. Handles output formatting (softmax, argmax, etc.).

## Memory Model

All buffers drawn from a single static `memory_pool`. Layout:
- Input buffer
- Output buffer
- Workspace (temporary activations)

Pool resets between inference runs.

## Control Flow

```
main()
 └─ pipeline_init()
 │   ├─ logger_init()
 │   └─ memory_pool_init()
 └─ user_register_hooks()
     └─ hook_register_preprocess/postprocess/inference()
 └─ pipeline_run(input, output)
     ├─ stage_preprocess()
     ├─ stage_inference()
     └─ stage_postprocess()
 └─ memory_pool_reset()
```
