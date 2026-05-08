#include "types.h"

edgeinfer_status_t stage_inference(const tensor_t *input, tensor_t *output) {
    /* TODO: call inference hook or default engine (iterate layers from model) */
    output->shape = input->shape;
    return EDGEINFER_OK;
}
