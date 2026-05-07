#include "types.h"
#include "hook_interface.h"
#include "logger.h"

extern postprocess_fn_t hook_get_postprocess(void);

edgeinfer_status_t stage_postprocess(const tensor_t *raw_output, tensor_t *formatted_output) {
    postprocess_fn_t fn = hook_get_postprocess();
    if (fn == NULL) {
        LOG_WARN("stages", "No postprocess hook — passing output through");
        formatted_output->data  = raw_output->data;
        formatted_output->shape = raw_output->shape;
        return EDGEINFER_OK;
    }
    return fn(raw_output, formatted_output);
}
