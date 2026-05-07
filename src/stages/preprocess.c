#include "types.h"
#include "hook_interface.h"
#include "logger.h"

extern preprocess_fn_t hook_get_preprocess(void);

edgeinfer_status_t stage_preprocess(const tensor_t *raw_input, tensor_t *prepared_input) {
    preprocess_fn_t fn = hook_get_preprocess();
    if (fn == NULL) {
        LOG_WARN("stages", "No preprocess hook — passing input through");
        prepared_input->data  = raw_input->data;
        prepared_input->shape = raw_input->shape;
        return EDGEINFER_OK;
    }
    return fn(raw_input, prepared_input);
}
