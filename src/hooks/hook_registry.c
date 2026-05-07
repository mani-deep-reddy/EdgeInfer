#include "hook_interface.h"
#include "logger.h"

static preprocess_fn_t  g_preprocess_fn  = NULL;
static postprocess_fn_t g_postprocess_fn = NULL;
static inference_fn_t   g_inference_fn   = NULL;

void hook_register_preprocess(preprocess_fn_t fn) {
    g_preprocess_fn = fn;
    LOG_INFO("hooks", "Preprocess hook registered");
}

void hook_register_postprocess(postprocess_fn_t fn) {
    g_postprocess_fn = fn;
    LOG_INFO("hooks", "Postprocess hook registered");
}

void hook_register_inference(inference_fn_t fn) {
    g_inference_fn = fn;
    LOG_INFO("hooks", "Inference override registered");
}

preprocess_fn_t  hook_get_preprocess(void)  { return g_preprocess_fn; }
postprocess_fn_t hook_get_postprocess(void) { return g_postprocess_fn; }
inference_fn_t   hook_get_inference(void)   { return g_inference_fn; }
