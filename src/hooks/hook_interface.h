#ifndef HOOK_INTERFACE_H
#define HOOK_INTERFACE_H

#include "types.h"

/* User-defined preprocess hook signature */
typedef edgeinfer_status_t (*preprocess_fn_t)(const tensor_t *raw_input, tensor_t *prepared_input);

/* User-defined postprocess hook signature */
typedef edgeinfer_status_t (*postprocess_fn_t)(const tensor_t *raw_output, tensor_t *formatted_output);

/* Optional user-defined inference override */
typedef edgeinfer_status_t (*inference_fn_t)(const tensor_t *input, tensor_t *output, const void *model_ctx);

#endif /* HOOK_INTERFACE_H */
