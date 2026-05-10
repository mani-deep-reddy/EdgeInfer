#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <stddef.h>

#define TENSOR_MAX_DIMS  4

#define ARRAY_SIZE(x)       (sizeof(x) / sizeof((x)[0]))   /* element count of static array */
#define MIN(a, b)           (((a) < (b)) ? (a) : (b))      /* safe min macro */
#define MAX(a, b)           (((a) > (b)) ? (a) : (b))      /* safe max macro */
#define ROUND_UP_ALIGN(x, a) (((x) + (a) - 1) & ~((a) - 1)) /* round up to alignment */

typedef struct {
    uint32_t dims[TENSOR_MAX_DIMS];
    uint8_t  ndim;
} tensor_shape_t;

typedef struct {
    float       *data;
    tensor_shape_t shape;
} tensor_t;

typedef enum {
    EDGEINFER_OK        = 0,
    EDGEINFER_ERR       = -1,
    EDGEINFER_ERR_OOM   = -2,
    EDGEINFER_ERR_INPUT = -3,
} edgeinfer_status_t;

typedef enum {
    LAYER_DENSE,
    LAYER_CONV2D,
    LAYER_MAXPOOL,
    LAYER_RELU,
    LAYER_SOFTMAX,
    LAYER_UNKNOWN
} layer_type_t;

#endif /* TYPES_H */
