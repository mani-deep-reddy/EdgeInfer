# EdgeInfer — Overview

## Motivation

Deploying neural network inference on resource-constrained microcontrollers requires predictable memory usage, deterministic timing, and minimal runtime overhead. EdgeInfer addresses this by providing a static, compile-time-driven inference framework.

## Goals

- **Deterministic memory**: No `malloc`/`free`. All buffers pre-allocated at compile time.
- **Pipeline execution**: Clear separation of preprocess, inference, and postprocess stages.
- **User extensibility**: Custom logic injected via function pointers — no framework modification needed.
- **Offline model conversion**: Models compiled into C headers; zero dependencies on-device.

## Non-Goals

- Dynamic model loading at runtime
- GPU or accelerator backends
- Training capabilities
- Support for every ONNX operator (start with Dense/Conv2D/ReLU)
