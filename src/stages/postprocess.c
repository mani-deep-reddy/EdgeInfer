#include "types.h"
#include "hook_interface.h"

edgeinfer_status_t stage_postprocess(const tensor_t *raw_output, tensor_t *formatted_output) {   /* call user hook or passthrough */
    postprocess_fn_t fn = hook_get_postprocess();
    if (fn) return fn(raw_output, formatted_output);
    *formatted_output = *raw_output;                 /* passthrough: same data, same shape */
    return EDGEINFER_OK;
}
