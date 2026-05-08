#include "pipeline.h"
#include "logger.h"
#include "memory_pool.h"

extern edgeinfer_status_t stage_preprocess(const tensor_t *raw_input, tensor_t *prepared_input);
extern edgeinfer_status_t stage_inference(const tensor_t *input, tensor_t *output);
extern edgeinfer_status_t stage_postprocess(const tensor_t *raw_output, tensor_t *formatted_output);

static memory_pool_t g_pool;

edgeinfer_status_t pipeline_init(void) {
    /* TODO: read model metadata, init memory pool with computed size, init buffer layout */
    logger_init();
    return memory_pool_init(&g_pool);
}

edgeinfer_status_t pipeline_run(const tensor_t *input, tensor_t *output) {
    /* TODO: preprocess, inference, postprocess stages, reset pool after */
    tensor_t prepared;
    tensor_t inference_out;

    stage_preprocess(input, &prepared);

    inference_out.data  = prepared.data;
    inference_out.shape = prepared.shape;
    stage_inference(&prepared, &inference_out);

    stage_postprocess(&inference_out, output);

    memory_pool_reset(&g_pool);
    return EDGEINFER_OK;
}
