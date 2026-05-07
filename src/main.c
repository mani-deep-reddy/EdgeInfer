#include "pipeline.h"
#include "types.h"
#include "logger.h"

/* Forward declarations for user hooks */
extern void user_register_hooks(void);

/* Simple test input buffer */
static float test_input_data[] = { 1.0f, 2.0f, 3.0f, 4.0f };
static float test_output_data[4] = { 0 };

int main(void) {
    /* Initialize the pipeline */
    edgeinfer_status_t status = pipeline_init();
    if (status != EDGEINFER_OK) {
        LOG_ERROR("main", "Pipeline init failed: %d", status);
        return 1;
    }

    /* Register user-provided hooks */
    user_register_hooks();

    /* Prepare test tensors */
    tensor_t input = {
        .data = test_input_data,
        .shape = { .ndim = 1, .dims = { 4 } }
    };

    tensor_t output = {
        .data = test_output_data,
        .shape = { .ndim = 1, .dims = { 4 } }
    };

    /* Run inference */
    status = pipeline_run(&input, &output);
    if (status != EDGEINFER_OK) {
        LOG_ERROR("main", "Pipeline run failed: %d", status);
        return 1;
    }

    LOG_INFO("main", "Output: [%.4f, %.4f, %.4f, %.4f]",
             output.data[0], output.data[1], output.data[2], output.data[3]);

    return 0;
}
