# Simple MLP Example

A minimal multi-layer perceptron example for validating the EdgeInfer pipeline.

## Model

- **Architecture**: 2-layer MLP (4 → 8 → 4)
- **Activation**: ReLU between layers
- **Input**: 4-element float vector
- **Output**: 4-element float vector (logits)

## Files

| File | Description |
|------|-------------|
| `model.onnx` | Source ONNX model (use `tools/onnx_to_c.py` to convert) |
| `input.bin` | Sample 4-element float input tensor |
| `expected_output.txt` | Golden reference output for verification |

## How to Run

1. Convert the model:
   ```bash
   python3 tools/onnx_to_c.py examples/simple_mlp/model.onnx
   ```

2. Build the project:
   ```bash
   make
   ```

3. Run in QEMU:
   ```bash
   ./qemu/run.sh
   ```

4. Compare output against `expected_output.txt`.

## Creating the Model

Generate the ONNX model with this Python snippet:

```python
import onnx
from onnx import helper, numpy_helper, TensorProto
import numpy as np

# Simple 2-layer MLP: 4 -> 8 -> 4
W1 = np.random.randn(8, 4).astype(np.float32)
b1 = np.zeros(8, dtype=np.float32)
W2 = np.random.randn(4, 8).astype(np.float32)
b2 = np.zeros(4, dtype=np.float32)

# ... construct ONNX graph nodes for MatMul + Add + Relu + MatMul + Add
```
