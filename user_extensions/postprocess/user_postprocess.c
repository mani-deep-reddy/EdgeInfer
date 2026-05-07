#include "types.h"
#include "hook_interface.h"
#include "logger.h"

/*
 * User postprocess hook example.
 * Replace or modify this file for custom output formatting.
 */

static edgeinfer_status_t my_postprocess(const tensor_t *raw_output, tensor_t *formatted_output) {
    LOG_INFO("user_postprocess", "Running user postprocess (passthrough)");

    formatted_output->data  = raw_output->data;
    formatted_output->shape = raw_output->shape;

    /* In a real implementation, you might apply softmax, argmax, etc. */

    return EDGEINFER_OK;
}

void user_register_postprocess_hook(postprocess_fn_t fn);

void user_register_postprocess_hook(postprocess_fn_t fn) {
    extern void hook_register_postprocess(postprocess_fn_t fn);
    hook_register_postprocess(fn);
}

/* Register from user_register_hooks in preprocess file */
