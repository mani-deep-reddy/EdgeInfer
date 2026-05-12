#include "pipeline.h"
#include "types.h"
#include "logger.h"
#include "uart.h"

extern void user_register_hooks(void);

static float test_input_data[] = { 1.0f, 2.0f, 3.0f, 4.0f };
static float test_output_data[4] = { 0 };

int main(void) {
    edgeinfer_status_t status = pipeline_init();
    if (status != EDGEINFER_OK) {
        return 1;
    }

    user_register_hooks();

    tensor_t input = {
        .data = test_input_data,
        .shape = { .ndim = 1, .dims = { 4 } }
    };

    tensor_t output = {
        .data = test_output_data,
        .shape = { .ndim = 1, .dims = { 4 } }
    };

    status = pipeline_run(&input, &output);
    if (status != EDGEINFER_OK) {
        return 1;
    }

    for (int i = 0; i < 4; i++) {                /* print each output value */
        uart_print_float(output.data[i]);          /* format as "0.000000" */
        uart_puts("\n");
    }

    return 0;
}
