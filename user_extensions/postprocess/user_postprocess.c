#include "types.h"
#include "hook_interface.h"

edgeinfer_status_t my_postprocess(const tensor_t *raw_output, tensor_t *formatted_output) {   /* example: passthrough postprocess */
    formatted_output->data  = raw_output->data;
    formatted_output->shape = raw_output->shape;
    return EDGEINFER_OK;
}
