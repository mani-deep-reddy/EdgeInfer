#include "types.h"

edgeinfer_status_t stage_preprocess(const tensor_t *raw_input, tensor_t *prepared_input) {
    /* TODO: call user preprocess hook, fallback to passthrough */
    prepared_input->data  = raw_input->data;
    prepared_input->shape = raw_input->shape;
    return EDGEINFER_OK;
}
