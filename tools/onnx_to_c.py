#!/usr/bin/env python3
"""
onnx_to_c.py — Convert an ONNX model to C header files for EdgeInfer.

Generates:
  src/model/model.h      — Weight and bias arrays
  src/model/model_meta.h — Layer topology and execution parameters

Usage:
  python3 onnx_to_c.py <model.onnx> [--output-dir src/model]

Dependencies:
  pip install onnx numpy
"""

import sys
import os
import argparse


def check_dependencies():
    missing = []
    try:
        import onnx
    except ImportError:
        missing.append("onnx")
    try:
        import numpy as np
    except ImportError:
        missing.append("numpy")

    if missing:
        print(f"Error: Missing required Python packages: {', '.join(missing)}")
        print(f"Install with: pip install {' '.join(missing)}")
        sys.exit(1)


def parse_args():
    parser = argparse.ArgumentParser(description="Convert ONNX model to C headers")
    parser.add_argument("model", help="Path to ONNX model file (.onnx)")
    parser.add_argument("--output-dir", default="src/model", help="Output directory for generated headers")
    return parser.parse_args()


def generate_model_h(onnx_model, output_dir):
    """Generate model.h with weight arrays."""
    import onnx
    import numpy as np

    model = onnx.load(onnx_model)
    output_path = os.path.join(output_dir, "model.h")

    with open(output_path, "w") as f:
        f.write("/* GENERATED FILE — Do not edit manually. */\n")
        f.write("/* Produced by tools/onnx_to_c.py */\n\n")
        f.write("#ifndef MODEL_H\n#define MODEL_H\n\n")
        f.write('#include "model_meta.h"\n\n')

        # Extract initializers (weights and biases)
        for init in model.graph.initializer:
            data = np.frombuffer(init.raw_data, dtype=np.float32)
            name = init.name.replace("/", "_").replace(".", "_")

            f.write(f"static const float g_{name}[] = {{\n")
            for i, val in enumerate(data):
                if i > 0 and i % 8 == 0:
                    f.write("\n")
                f.write(f"    {val:.6e}f, ")
            f.write("\n};\n\n")

        f.write("#endif /* MODEL_H */\n")

    print(f"Generated: {output_path}")


def generate_model_meta_h(onnx_model, output_dir):
    """Generate model_meta.h with layer topology."""
    import onnx

    model = onnx.load(onnx_model)
    output_path = os.path.join(output_dir, "model_meta.h")

    with open(output_path, "w") as f:
        f.write("/* GENERATED FILE — Do not edit manually. */\n")
        f.write("/* Produced by tools/onnx_to_c.py */\n\n")
        f.write("#ifndef MODEL_META_H\n#define MODEL_META_H\n\n")
        f.write('#include "types.h"\n')
        f.write('#include "runtime_config.h"\n\n')

        # Model input shape
        if model.graph.input:
            inp = model.graph.input[0]
            shape = [d.dim_value for d in inp.type.tensor_type.shape.dim]
            f.write(f"/* Model input shape: {shape} */\n")

        # Model output shape
        if model.graph.output:
            out = model.graph.output[0]
            shape = [d.dim_value for d in out.type.tensor_type.shape.dim]
            f.write(f"/* Model output shape: {shape} */\n")

        f.write("\n#endif /* MODEL_META_H */\n")

    print(f"Generated: {output_path}")


def main():
    check_dependencies()
    args = parse_args()

    if not os.path.exists(args.model):
        print(f"Error: Model file not found: {args.model}")
        sys.exit(1)

    os.makedirs(args.output_dir, exist_ok=True)

    print(f"Converting: {args.model}")
    generate_model_h(args.model, args.output_dir)
    generate_model_meta_h(args.model, args.output_dir)
    print("Done.")


if __name__ == "__main__":
    main()
