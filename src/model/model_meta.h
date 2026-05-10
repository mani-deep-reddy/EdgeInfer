/* GENERATED FILE — Do not edit manually. */
/* Produced by tools/onnx_to_c.py */

#ifndef MODEL_META_H
#define MODEL_META_H

#include <stdint.h>
#include "types.h"

/* Model sizing macros — usable in static array declarations */
#define MODEL_INPUT_SIZE            16U
#define MODEL_OUTPUT_SIZE           16U
#define MODEL_MAX_ACTIVATIONS_SIZE  32U
#define MODEL_MAX_INTERMEDIATE_SIZE 0U
#define MODEL_NUM_LAYERS            3U

/* Composite type definitions (depends on types.h) */
typedef struct {
    layer_type_t type;
    tensor_shape_t input_shape;
    tensor_shape_t output_shape;
    uint32_t weight_offset;
    uint32_t bias_offset;
} layer_meta_t;

typedef struct {
    const layer_meta_t *layers;
    uint32_t            num_layers;
    uint32_t            layers_capacity;
    tensor_shape_t      model_input_shape;
    tensor_shape_t      model_output_shape;
} model_meta_t;

typedef struct {
    uint32_t total_weights_size;
    uint32_t max_activations_size;
    uint32_t max_intermediate_size;
} memory_requirements_t;

/* Layer topology */
static const layer_meta_t g_layers[MODEL_NUM_LAYERS] = {
    {.type = LAYER_DENSE, .input_shape = {.ndim = 2, .dims = {1, 4, 1, 1}}, .output_shape = {.ndim = 2, .dims = {1, 8, 1, 1}}, .weight_offset = 0U, .bias_offset = 0U},
    {.type = LAYER_RELU, .input_shape = {.ndim = 2, .dims = {1, 8, 1, 1}}, .output_shape = {.ndim = 2, .dims = {1, 8, 1, 1}}, .weight_offset = 0U, .bias_offset = 0U},
    {.type = LAYER_DENSE, .input_shape = {.ndim = 2, .dims = {1, 8, 1, 1}}, .output_shape = {.ndim = 2, .dims = {1, 4, 1, 1}}, .weight_offset = 32U, .bias_offset = 8U},
};

/* Extern declarations */
extern const model_meta_t g_model_meta;
extern const memory_requirements_t g_memory_requirements;

#endif /* MODEL_META_H */
