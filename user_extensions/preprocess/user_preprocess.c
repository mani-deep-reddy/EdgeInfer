#include "types.h"
#include "hook_interface.h"
#include "logger.h"

/*
 * User preprocess hook example.
 * Replace or modify this file for custom input preparation.
 */

static edgeinfer_status_t my_preprocess(const tensor_t *raw_input, tensor_t *prepared_input) {
    LOG_INFO("user_preprocess", "Running user preprocess (passthrough)");

    /* Example: normalize input to [0, 1] range */
    uint32_t n = 1;
    for (uint8_t i = 0; i < raw_input->shape.ndim; i++) {
        n *= raw_input->shape.dims[i];
    }

    prepared_input->data  = raw_input->data;
    prepared_input->shape = raw_input->shape;

    /* In a real implementation, you would transform the data here */
    (void)n;

    return EDGEINFER_OK;
}

/* This function is called from main.c to register hooks */
void user_register_hooks(void) {
    extern void hook_register_preprocess(preprocess_fn_t fn);
    extern void hook_register_postprocess(postprocess_fn_t fn);

    hook_register_preprocess(my_preprocess);
    /* Postprocess and inference hooks are optional */
}
