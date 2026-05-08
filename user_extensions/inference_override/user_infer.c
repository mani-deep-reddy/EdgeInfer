#include "types.h"
#include "hook_interface.h"

static edgeinfer_status_t my_inference(const tensor_t *input, tensor_t *output, const void *model_ctx) {
    /* TODO: user inference override — layer-by-layer or custom engine */
    output->shape = input->shape;
    (void)model_ctx;
    return EDGEINFER_OK;
}
