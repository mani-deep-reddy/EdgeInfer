#include "types.h"
#include "model_meta.h"
#include "logger.h"

extern inference_fn_t hook_get_inference(void);

edgeinfer_status_t stage_inference(const tensor_t *input, tensor_t *output) {
    inference_fn_t override_fn = hook_get_inference();

    if (override_fn != NULL) {
        LOG_INFO("stages", "Using user inference override");
        return override_fn(input, output, &g_model_meta);
    }

    /* Default inference engine — placeholder */
    LOG_INFO("stages", "Running default inference engine");
    LOG_WARN("stages", "Default engine not yet implemented — returning zeros");

    uint32_t out_elements = 1;
    for (uint8_t i = 0; i < output->shape.ndim; i++) {
        out_elements *= output->shape.dims[i];
    }

    for (uint32_t i = 0; i < out_elements; i++) {
        output->data[i] = 0.0f;
    }

    return EDGEINFER_OK;
}
