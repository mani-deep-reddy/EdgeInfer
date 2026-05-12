#include "types.h"
#include "hook_interface.h"

edgeinfer_status_t stage_preprocess(const tensor_t *raw_input, tensor_t *prepared_input) {   /* call user hook or passthrough */
    preprocess_fn_t fn = hook_get_preprocess();
    if (fn) return fn(raw_input, prepared_input);
    *prepared_input = *raw_input;          /* passthrough: same data, same shape */
    return EDGEINFER_OK;
}
