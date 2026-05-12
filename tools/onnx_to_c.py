#!/usr/bin/env python3
"""
onnx_to_c.py — Convert an ONNX model to C header files for EdgeInfer.

Generates:
  src/model/model_meta.h — Type definitions, #define macros, layer topology
  src/model/model.h      — Weight/bias arrays, model_meta instance, memory_requirements

Usage:
  python3 onnx_to_c.py <model.onnx> [--output-dir src/model]

Dependencies:
  pip install onnx numpy
"""

import sys
import os
import argparse


_TENSOR_MAX_DIMS = 4


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


def onnx_op_to_layer_type(op_type):
    mapping = {
        "Gemm": "LAYER_DENSE",
        "MatMul": "LAYER_DENSE",
        "Relu": "LAYER_RELU",
        "Conv": "LAYER_CONV2D",
        "MaxPool": "LAYER_MAXPOOL",
        "Softmax": "LAYER_SOFTMAX",
    }
    if op_type in mapping:
        return mapping[op_type]
    print(f"Warning: Unknown op type '{op_type}' mapped to LAYER_UNKNOWN", file=sys.stderr)
    return "LAYER_UNKNOWN"


def tensor_num_elements(dims):
    count = 1
    for d in dims:
        count *= d
    return count


def format_tensor_shape(dims):
    ndim = min(len(dims), _TENSOR_MAX_DIMS)
    padded = list(dims[:_TENSOR_MAX_DIMS]) + [1] * (_TENSOR_MAX_DIMS - ndim)
    dims_str = ", ".join(str(d) for d in padded)
    return f"{{.ndim = {ndim}, .dims = {{{dims_str}}}}}"


def compute_activation_size(dims):
    return tensor_num_elements(dims) * 4


def build_initializer_map(model):
    import onnx
    result = {}
    for init in model.graph.initializer:
        arr = onnx.numpy_helper.to_array(init)
        result[init.name] = arr
    return result


def infer_matmul_output_shape(a_shape, b_shape):
    if len(a_shape) == 1 and len(b_shape) == 2:
        return [b_shape[1] if b_shape[0] == a_shape[0] else b_shape[0]]
    elif len(a_shape) == 2 and len(b_shape) == 2:
        if a_shape[1] == b_shape[0]:
            return [a_shape[0], b_shape[1]]
        elif a_shape[1] == b_shape[1]:
            return [a_shape[0], b_shape[0]]
        else:
            return [a_shape[0], max(b_shape[0], b_shape[1])]
    return [1]


def make_float_array_literal(data, name):
    lines = []
    lines.append(f"static const float {name}[] = {{")
    for i, val in enumerate(data):
        if i > 0 and i % 8 == 0:
            lines.append("")
        lines.append(f"    {val:.6e}f, ")
    lines.append("\n};\n")
    return "".join(lines)


def generate(onnx_path, output_dir):
    import onnx
    import numpy as np

    model = onnx.load(onnx_path)
    init_map = build_initializer_map(model)
    init_names = set(init_map.keys())

    model_input = model.graph.input[0]
    input_dims = [d.dim_value for d in model_input.type.tensor_type.shape.dim]

    model_output = model.graph.output[0]
    output_dims = [d.dim_value for d in model_output.type.tensor_type.shape.dim]

    nodes = list(model.graph.node)

    layers = []
    weight_arrays = []
    bias_arrays = []
    weight_offset = 0
    bias_offset = 0

    tensor_shapes = {}
    tensor_shapes[model_input.name] = input_dims

    i = 0
    while i < len(nodes):
        node = nodes[i]
        op_type = node.op_type
        inputs = node.input
        outputs = node.output

        if op_type == "MatMul":
            in_name = inputs[0]
            in_shape = tensor_shapes.get(in_name, [])
            weight_name = inputs[1]
            weight_arr = init_map.get(weight_name)
            w_off = weight_offset
            b_off = bias_offset
            has_bias = False

            if weight_arr is not None:
                weight_offset += weight_arr.size
                weight_arrays.append((weight_arr, w_off))

            if weight_arr is not None:
                out_shape = infer_matmul_output_shape(in_shape, weight_arr.shape)
            else:
                out_shape = in_shape

            if (i + 1 < len(nodes) and nodes[i + 1].op_type == "Add"):
                add_node = nodes[i + 1]
                bias_name = None
                for inp in add_node.input:
                    if inp in init_names and inp != node.output[0]:
                        bias_name = inp
                        break
                if bias_name is not None:
                    bias_arr = init_map[bias_name]
                    b_off = bias_offset
                    bias_offset += bias_arr.size
                    bias_arrays.append((bias_arr, b_off))
                    has_bias = True
                    tensor_shapes[add_node.output[0]] = out_shape
                    i += 2
                else:
                    i += 1
            else:
                i += 1

            tensor_shapes[outputs[0]] = out_shape
            layers.append({
                "type": "LAYER_DENSE",
                "input_shape": in_shape,
                "output_shape": out_shape,
                "weight_offset": w_off,
                "bias_offset": b_off if has_bias else 0,
                "has_bias": has_bias,
            })

        elif op_type == "Relu":
            in_name = inputs[0]
            in_shape = tensor_shapes.get(in_name, [])
            out_shape = list(in_shape)
            tensor_shapes[outputs[0]] = out_shape
            layers.append({
                "type": "LAYER_RELU",
                "input_shape": in_shape,
                "output_shape": out_shape,
                "weight_offset": 0,
                "bias_offset": 0,
                "has_bias": False,
            })
            i += 1

        elif op_type == "Softmax":
            in_name = inputs[0]
            in_shape = tensor_shapes.get(in_name, [])
            out_shape = list(in_shape)
            tensor_shapes[outputs[0]] = out_shape
            layers.append({
                "type": "LAYER_SOFTMAX",
                "input_shape": in_shape,
                "output_shape": out_shape,
                "weight_offset": 0,
                "bias_offset": 0,
                "has_bias": False,
            })
            i += 1

        else:
            if op_type == "Add":
                in_name = inputs[0]
                in_shape = tensor_shapes.get(in_name, [])
                out_shape = list(in_shape) if in_shape else [1]
                tensor_shapes[outputs[0]] = out_shape
            else:
                in_name = inputs[0] if inputs else ""
                in_shape = tensor_shapes.get(in_name, [])
                out_shape = list(in_shape) if in_shape else [1]
                tensor_shapes[outputs[0]] = out_shape
            lt = onnx_op_to_layer_type(op_type)
            layers.append({
                "type": lt,
                "input_shape": in_shape,
                "output_shape": out_shape,
                "weight_offset": 0,
                "bias_offset": 0,
                "has_bias": False,
            })
            i += 1

    if output_dims and output_dims != [1]:
        pass
    elif layers:
        output_dims = layers[-1]["output_shape"]

    max_activations = 0
    for layer in layers:
        sz = compute_activation_size(layer["output_shape"])
        if sz > max_activations:
            max_activations = sz

    flat_weights = []
    for arr, off in weight_arrays:
        flat_weights.append((arr.flatten(), off))
    total_weights_elements = sum(arr.size for arr, _ in weight_arrays)

    flat_biases = []
    for arr, off in bias_arrays:
        flat_biases.append((arr.flatten(), off))
    total_biases_elements = sum(arr.size for arr, _ in bias_arrays)

    total_weights_bytes = (total_weights_elements + total_biases_elements) * 4

    input_size = compute_activation_size(input_dims)
    output_size = compute_activation_size(output_dims)

    return {
        "input_dims": input_dims,
        "output_dims": output_dims,
        "layers": layers,
        "flat_weights": flat_weights,
        "flat_biases": flat_biases,
        "total_weights_elements": total_weights_elements,
        "total_biases_elements": total_biases_elements,
        "total_weights_bytes": total_weights_bytes,
        "max_activations": max_activations,
        "max_intermediate": 0,
        "input_size": input_size,
        "output_size": output_size,
    }


def write_model_meta_h(output_dir, info):   # generate model_meta.h: types, sizing macros, layer topology
    path = os.path.join(output_dir, "model_meta.h")
    with open(path, "w") as f:
        f.write("/* GENERATED FILE — Do not edit manually. */\n")
        f.write("/* Produced by tools/onnx_to_c.py */\n\n")
        f.write("#ifndef MODEL_META_H\n#define MODEL_META_H\n\n")
        f.write("#include <stdint.h>\n")
        f.write('#include "types.h"\n\n')

        f.write("/* Model sizing macros — usable in static array declarations */\n")
        f.write(f"#define MODEL_INPUT_SIZE            {info['input_size']}U\n")
        f.write(f"#define MODEL_OUTPUT_SIZE           {info['output_size']}U\n")
        f.write(f"#define MODEL_MAX_ACTIVATIONS_SIZE  {info['max_activations']}U\n")
        f.write(f"#define MODEL_MAX_INTERMEDIATE_SIZE {info['max_intermediate']}U\n")
        f.write(f"#define MODEL_NUM_LAYERS            {len(info['layers'])}U\n\n")

        f.write("/* Composite type definitions (depends on types.h) */\n")

        f.write("typedef struct {\n")
        f.write("    layer_type_t type;\n")
        f.write("    tensor_shape_t input_shape;\n")
        f.write("    tensor_shape_t output_shape;\n")
        f.write("    uint32_t weight_offset;\n")
        f.write("    uint32_t bias_offset;\n")
        f.write("} layer_meta_t;\n\n")

        f.write("typedef struct {\n")
        f.write("    const layer_meta_t *layers;\n")
        f.write("    uint32_t            num_layers;\n")
        f.write("    uint32_t            layers_capacity;\n")
        f.write("    tensor_shape_t      model_input_shape;\n")
        f.write("    tensor_shape_t      model_output_shape;\n")
        f.write("} model_meta_t;\n\n")

        f.write("typedef struct {\n")
        f.write("    uint32_t total_weights_size;\n")
        f.write("    uint32_t max_activations_size;\n")
        f.write("    uint32_t max_intermediate_size;\n")
        f.write("} memory_requirements_t;\n\n")

        f.write("/* Layer topology */\n")
        num_layers = len(info["layers"])
        f.write(f"static const layer_meta_t g_layers[MODEL_NUM_LAYERS] = {{\n")
        for layer in info["layers"]:
            in_shape = format_tensor_shape(layer["input_shape"])
            out_shape = format_tensor_shape(layer["output_shape"])
            f.write(f"    {{.type = {layer['type']}, .input_shape = {in_shape}, .output_shape = {out_shape}, .weight_offset = {layer['weight_offset']}U, .bias_offset = {layer['bias_offset']}U}},\n")
        f.write("};\n\n")



        f.write("#endif /* MODEL_META_H */\n")

    print(f"Generated: {path}")


def write_model_h(output_dir, info):   # generate model.h: weight/bias arrays, model_meta and memory_requirements instances
    path = os.path.join(output_dir, "model.h")
    with open(path, "w") as f:
        f.write("/* GENERATED FILE — Do not edit manually. */\n")
        f.write("/* Produced by tools/onnx_to_c.py */\n\n")
        f.write("#ifndef MODEL_H\n#define MODEL_H\n\n")
        f.write('#include "model_meta.h"\n\n')

        f.write("/* Weight data */\n")
        if info["flat_weights"]:
            all_w = []
            runtime_w_off = 0
            for arr, off in info["flat_weights"]:
                for val in arr:
                    all_w.append(val)
            f.write(make_float_array_literal(all_w, "g_weights"))
        else:
            f.write("static const float g_weights[] = { 0.0f };\n\n")

        f.write("/* Bias data */\n")
        if info["flat_biases"]:
            all_b = []
            for arr, off in info["flat_biases"]:
                for val in arr:
                    all_b.append(val)
            f.write(make_float_array_literal(all_b, "g_biases"))
        else:
            f.write("static const float g_biases[] = { 0.0f };\n\n")

        f.write("/* Memory requirements */\n")
        f.write("static const memory_requirements_t g_memory_requirements = {\n")
        f.write(f"    .total_weights_size     = {info['total_weights_bytes']}U,\n")
        f.write(f"    .max_activations_size   = {info['max_activations']}U,\n")
        f.write(f"    .max_intermediate_size  = {info['max_intermediate']}U,\n")
        f.write("};\n\n")

        f.write("/* Model metadata */\n")
        in_shape = format_tensor_shape(info["input_dims"])
        out_shape = format_tensor_shape(info["output_dims"])
        num_layers = len(info["layers"])
        f.write("static const model_meta_t g_model_meta = {\n")
        f.write("    .layers             = g_layers,\n")
        f.write(f"    .num_layers         = {num_layers}U,\n")
        f.write(f"    .layers_capacity    = {num_layers}U,\n")
        f.write(f"    .model_input_shape  = {in_shape},\n")
        f.write(f"    .model_output_shape = {out_shape},\n")
        f.write("};\n\n")

        f.write("#endif /* MODEL_H */\n")

    print(f"Generated: {path}")


def main():
    check_dependencies()
    args = parse_args()

    if not os.path.exists(args.model):
        print(f"Error: Model file not found: {args.model}")
        sys.exit(1)

    os.makedirs(args.output_dir, exist_ok=True)

    print(f"Converting: {args.model}")
    info = generate(args.model, args.output_dir)
    write_model_meta_h(args.output_dir, info)
    write_model_h(args.output_dir, info)
    print("Done.")


if __name__ == "__main__":
    main()
