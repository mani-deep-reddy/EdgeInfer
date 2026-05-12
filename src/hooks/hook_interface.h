#ifndef HOOK_INTERFACE_H
#define HOOK_INTERFACE_H

#include "types.h"

typedef edgeinfer_status_t (*preprocess_fn_t)(const tensor_t *raw_input, tensor_t *prepared_input);    /* user raw input → model input */
typedef edgeinfer_status_t (*postprocess_fn_t)(const tensor_t *raw_output, tensor_t *formatted_output);  /* model output → user format */
typedef edgeinfer_status_t (*inference_fn_t)(const tensor_t *input, tensor_t *output, const void *model_ctx);  /* full override (reserved) */

void hook_register_preprocess(preprocess_fn_t fn);    /* register user preprocess hook */
void hook_register_postprocess(postprocess_fn_t fn);  /* register user postprocess hook */
void hook_register_inference(inference_fn_t fn);       /* register inference override (reserved) */

preprocess_fn_t  hook_get_preprocess(void);    /* get registered preprocess hook (NULL if none) */
postprocess_fn_t hook_get_postprocess(void);   /* get registered postprocess hook (NULL if none) */
inference_fn_t   hook_get_inference(void);     /* get registered inference hook (NULL if none) */

#endif /* HOOK_INTERFACE_H */
