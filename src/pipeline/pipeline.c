#include "pipeline.h"
#include "logger.h"
#include "memory_pool.h"

extern edgeinfer_status_t stage_preprocess(const tensor_t *raw_input, tensor_t *prepared_input);
extern edgeinfer_status_t stage_inference(const tensor_t *input, tensor_t *output);
extern edgeinfer_status_t stage_postprocess(const tensor_t *raw_output, tensor_t *formatted_output);

static memory_pool_t g_pool;

edgeinfer_status_t pipeline_init(void) {
    logger_init();
    LOG_INFO("pipeline", "Initializing pipeline");
    return memory_pool_init(&g_pool);
}

edgeinfer_status_t pipeline_run(const tensor_t *input, tensor_t *output) {
    tensor_t prepared;
    tensor_t inference_out;

    LOG_INFO("pipeline", "Starting pipeline run");

    /* Stage 1: Preprocess */
    edgeinfer_status_t status = stage_preprocess(input, &prepared);
    if (status != EDGEINFER_OK) return status;

    /* Stage 2: Inference */
    inference_out.data  = prepared.data; /* Reuse buffer in simple case */
    inference_out.shape = prepared.shape;
    status = stage_inference(&prepared, &inference_out);
    if (status != EDGEINFER_OK) return status;

    /* Stage 3: Postprocess */
    status = stage_postprocess(&inference_out, output);

    /* Reset memory pool for next run */
    memory_pool_reset(&g_pool);

    LOG_INFO("pipeline", "Pipeline run complete");
    return status;
}
