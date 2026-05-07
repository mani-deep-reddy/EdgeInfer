#include "types.h"
#include "hook_interface.h"
#include "logger.h"

/*
 * Optional user inference override.
 * Implement this to bypass the default inference engine entirely.
 * Register via hook_register_inference() to activate.
 */

static edgeinfer_status_t my_inference(const tensor_t *input, tensor_t *output, const void *model_ctx) {
    LOG_INFO("user_infer", "Running user inference override");

    /* Example: simple identity transform */
    uint32_t n = 1;
    for (uint8_t i = 0; i < input->shape.ndim; i++) {
        n *= input->shape.dims[i];
    }

    for (uint32_t i = 0; i < n; i++) {
        output->data[i] = input->data[i];
    }

    (void)model_ctx;
    return EDGEINFER_OK;
}

/* Call hook_register_inference(my_inference) to activate this override */
