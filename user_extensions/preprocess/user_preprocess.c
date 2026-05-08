#include "types.h"
#include "hook_interface.h"

static edgeinfer_status_t my_preprocess(const tensor_t *raw_input, tensor_t *prepared_input) {
    /* TODO: user preprocess hook — normalize, reshape, etc. */
    prepared_input->data  = raw_input->data;
    prepared_input->shape = raw_input->shape;
    return EDGEINFER_OK;
}

void user_register_hooks(void) {
    extern void hook_register_preprocess(preprocess_fn_t fn);
    hook_register_preprocess(my_preprocess);
}
