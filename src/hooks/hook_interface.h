#ifndef HOOK_INTERFACE_H
#define HOOK_INTERFACE_H

#include "types.h"

typedef edgeinfer_status_t (*preprocess_fn_t)(const tensor_t *raw_input, tensor_t *prepared_input);
typedef edgeinfer_status_t (*postprocess_fn_t)(const tensor_t *raw_output, tensor_t *formatted_output);
typedef edgeinfer_status_t (*inference_fn_t)(const tensor_t *input, tensor_t *output, const void *model_ctx);

#endif /* HOOK_INTERFACE_H */
