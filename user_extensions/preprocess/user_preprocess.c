#include "types.h"
#include "hook_interface.h"

static edgeinfer_status_t my_preprocess(const tensor_t *raw_input, tensor_t *prepared_input) {   /* example: passthrough preprocess */
    prepared_input->data  = raw_input->data;
    prepared_input->shape = raw_input->shape;
    return EDGEINFER_OK;
}

extern edgeinfer_status_t my_postprocess(const tensor_t *raw_output, tensor_t *formatted_output);   /* defined in user_extensions/postprocess/ */

void user_register_hooks(void) {   /* register all user-defined hooks with the hook registry */
    extern void hook_register_preprocess(preprocess_fn_t fn);
    extern void hook_register_postprocess(postprocess_fn_t fn);
    hook_register_preprocess(my_preprocess);
    hook_register_postprocess(my_postprocess);
}
