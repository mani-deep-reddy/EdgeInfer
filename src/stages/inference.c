#include <stdbool.h>
#include "types.h"
#include "model.h"
#include "utils.h"

edgeinfer_status_t stage_inference(const tensor_t *input, tensor_t *output, float *workspace, uint32_t workspace_len) {   /* default engine: iterate layers from g_model_meta */
    const float *current_in = input->data;           /* input for current layer */
    float *current_out = workspace;                  /* output for current layer (default: workspace) */
    uint32_t current_size = tensor_num_elements(&input->shape);    /* element count for current layer input */
    (void)workspace_len;

    for (uint32_t i = 0; i < g_model_meta.num_layers; i++) {
        const layer_meta_t *layer = &g_layers[i];    /* current layer descriptor */
        uint32_t out_size = tensor_num_elements(&layer->output_shape);   /* output element count */
        bool is_last = (i == g_model_meta.num_layers - 1);

        if (is_last) {
            current_out = output->data;              /* final layer writes to caller output */
        }

        switch (layer->type) {
            case LAYER_DENSE: {                      /* y = Wx + b */
                const float *weights = g_weights + layer->weight_offset;
                const float *bias = g_biases + layer->bias_offset;
                for (uint32_t j = 0; j < out_size; j++) {
                    float sum = bias[j];             /* start with bias term */
                    for (uint32_t k = 0; k < current_size; k++) {
                        sum += current_in[k] * weights[j * current_size + k];   /* accumulate weighted input */
                    }
                    current_out[j] = sum;
                }
                break;
            }
            case LAYER_RELU:                         /* max(0, x), in-place safe */
                for (uint32_t j = 0; j < current_size; j++) {
                    current_out[j] = (current_in[j] > 0.0f) ? current_in[j] : 0.0f;
                }
                break;
            default:
                return EDGEINFER_ERR;                /* unsupported layer type */
        }

        current_in = current_out;                    /* chain: this layer's output is next layer's input */
        current_size = out_size;
        if (!is_last) {
            current_out = workspace;                 /* intermediate layers reuse workspace */
        }
    }

    return EDGEINFER_OK;
}
