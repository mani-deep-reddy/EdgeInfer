#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <stddef.h>

/* Tensor dimension type */
typedef struct {
    uint32_t dims[MAX_INPUT_DIMS];
    uint8_t  ndim;
} tensor_shape_t;

/* Tensor descriptor */
typedef struct {
    float       *data;
    tensor_shape_t shape;
} tensor_t;

/* Status codes */
typedef enum {
    EDGEINFER_OK        = 0,
    EDGEINFER_ERR       = -1,
    EDGEINFER_ERR_OOM   = -2,
    EDGEINFER_ERR_INPUT = -3,
} edgeinfer_status_t;

/* Layer type enumeration */
typedef enum {
    LAYER_DENSE,
    LAYER_CONV2D,
    LAYER_MAXPOOL,
    LAYER_RELU,
    LAYER_SOFTMAX,
    LAYER_UNKNOWN
} layer_type_t;

#endif /* TYPES_H */
