#include "pipeline.h"
#include "model.h"
#include "logger.h"
#include "memory_pool.h"
#include "buffer_layout.h"

extern edgeinfer_status_t stage_preprocess(const tensor_t *raw_input, tensor_t *prepared_input);     /* declared in stages/preprocess.c */
extern edgeinfer_status_t stage_inference(const tensor_t *input, tensor_t *output, float *workspace, uint32_t workspace_len);   /* stages/inference.c */
extern edgeinfer_status_t stage_postprocess(const tensor_t *raw_output, tensor_t *formatted_output);  /* stages/postprocess.c */

static memory_pool_t g_pool;          /* static memory pool shared across pipeline */
static buffer_layout_t g_layout;      /* buffer region layout carved from pool */

edgeinfer_status_t pipeline_init(void) {   /* init logging, memory pool, and buffer layout */
    logger_init();
    edgeinfer_status_t status = memory_pool_init(&g_pool);
    if (status != EDGEINFER_OK) return status;
    buffer_layout_init(&g_layout, g_pool.base, g_pool.total_size);
    return EDGEINFER_OK;
}

edgeinfer_status_t pipeline_run(const tensor_t *input, tensor_t *output) {   /* run preprocess → inference → postprocess, reset pool */
    tensor_t prepared;                        /* model input buffer from layout */
    prepared.data = (float *)buffer_layout_get(&g_layout, BUF_INPUT);
    prepared.shape = g_model_meta.model_input_shape;

    tensor_t inference_out;                   /* model output buffer from layout */
    inference_out.data = (float *)buffer_layout_get(&g_layout, BUF_OUTPUT);
    inference_out.shape = g_model_meta.model_output_shape;

    float *workspace = (float *)buffer_layout_get(&g_layout, BUF_WORKSPACE);   /* scratch for intermediate activations */
    uint32_t workspace_len = g_layout.regions[BUF_WORKSPACE].size / sizeof(float);

    edgeinfer_status_t status;

    status = stage_preprocess(input, &prepared);       /* raw → model input */
    if (status != EDGEINFER_OK) return status;

    status = stage_inference(&prepared, &inference_out, workspace, workspace_len);   /* run model layers */
    if (status != EDGEINFER_OK) return status;

    status = stage_postprocess(&inference_out, output);    /* model output → formatted */
    if (status != EDGEINFER_OK) return status;

    memory_pool_reset(&g_pool);                   /* reclaim scratch for next run */
    return EDGEINFER_OK;
}
