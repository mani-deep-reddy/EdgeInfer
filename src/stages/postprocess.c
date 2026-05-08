#include "types.h"

edgeinfer_status_t stage_postprocess(const tensor_t *raw_output, tensor_t *formatted_output) {
    /* TODO: call user postprocess hook, fallback to passthrough */
    formatted_output->data  = raw_output->data;
    formatted_output->shape = raw_output->shape;
    return EDGEINFER_OK;
}
