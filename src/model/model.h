#ifndef MODEL_H
#define MODEL_H

#include "model_meta.h"

/* TODO: generated placeholder — real weights from onnx_to_c.py */
static const float g_weights_placeholder[] = { 0.0f };
static const float g_biases_placeholder[]  = { 0.0f };

static const layer_meta_t g_layers_placeholder[] = {
    { .type = LAYER_DENSE,
      .input_shape  = { .ndim = 1, .dims = {4} },
      .output_shape = { .ndim = 1, .dims = {4} },
      .weight_offset = 0,
      .bias_offset   = 0 }
};

const model_meta_t g_model_meta = {
    .layers         = (layer_meta_t *)g_layers_placeholder,
    .num_layers     = 1,
    .layers_capacity = 1,
    .model_input_shape  = { .ndim = 1, .dims = {4} },
    .model_output_shape = { .ndim = 1, .dims = {4} },
};

const memory_requirements_t g_memory_requirements = {
    .total_weights_size     = sizeof(g_weights_placeholder) + sizeof(g_biases_placeholder),
    .max_activations_size   = 4 * sizeof(float),
    .max_intermediate_size  = 4 * sizeof(float),
};

#endif /* MODEL_H */
