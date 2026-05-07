#ifndef PIPELINE_H
#define PIPELINE_H

#include "types.h"

edgeinfer_status_t pipeline_init(void);
edgeinfer_status_t pipeline_run(const tensor_t *input, tensor_t *output);

#endif /* PIPELINE_H */
