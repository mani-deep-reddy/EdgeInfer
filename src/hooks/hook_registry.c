#include "hook_interface.h"

/* TODO: implement hook registration with runtime_config ENABLE guards */

preprocess_fn_t  g_preprocess_fn  = NULL;
postprocess_fn_t g_postprocess_fn = NULL;
inference_fn_t   g_inference_fn   = NULL;

void hook_register_preprocess(preprocess_fn_t fn) {
    g_preprocess_fn = fn;
}

void hook_register_postprocess(postprocess_fn_t fn) {
    g_postprocess_fn = fn;
}

void hook_register_inference(inference_fn_t fn) {
    g_inference_fn = fn;
}

preprocess_fn_t  hook_get_preprocess(void)  { return g_preprocess_fn; }
postprocess_fn_t hook_get_postprocess(void) { return g_postprocess_fn; }
inference_fn_t   hook_get_inference(void)   { return g_inference_fn; }
