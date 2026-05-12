/* GENERATED FILE — Do not edit manually. */
/* Produced by tools/onnx_to_c.py */

#ifndef MODEL_H
#define MODEL_H

#include "model_meta.h"

/* Weight data */
static const float g_weights[] = {    8.486694e-01f,     6.096890e-01f,     -1.966769e+00f,     2.252810e+00f,     -7.896438e-01f,     4.797608e-01f,     2.991168e-01f,     -1.041786e+00f,     -2.902896e+00f,     4.974258e-01f,     -1.882295e+00f,     -2.097469e+00f,     -9.864830e-01f,     6.695497e-01f,     -1.751010e+00f,     8.357269e-01f,     6.510009e-01f,     5.574218e-01f,     6.258053e-01f,     6.316150e-01f,     -3.819066e-01f,     -6.885154e-01f,     2.633763e-01f,     -1.426662e+00f,     -1.718050e+00f,     5.545128e-01f,     7.911681e-01f,     1.171507e+00f,     -1.106124e+00f,     -1.882260e-01f,     3.776608e-01f,     8.153533e-01f,     -9.257652e-01f,     -1.891628e+00f,     1.087019e+00f,     7.389913e-01f,     3.389790e-01f,     -6.451319e-01f,     2.038228e+00f,     5.157370e-01f,     -1.005014e+00f,     6.264015e-01f,     5.628862e-01f,     -1.987489e-01f,     -1.869040e-01f,     -9.474546e-02f,     1.158571e+00f,     -1.498411e-01f,     7.270790e-01f,     -1.753477e+00f,     6.321026e-01f,     7.936898e-01f,     -8.800435e-01f,     1.161731e+00f,     -6.644375e-01f,     7.414293e-01f,     1.981032e+00f,     -5.218369e-01f,     9.729054e-01f,     -8.261641e-01f,     -6.288470e-01f,     2.810727e+00f,     2.817139e-01f,     -2.343725e+00f, 
};
/* Bias data */
static const float g_biases[] = {    0.000000e+00f,     0.000000e+00f,     0.000000e+00f,     0.000000e+00f,     0.000000e+00f,     0.000000e+00f,     0.000000e+00f,     0.000000e+00f,     0.000000e+00f,     0.000000e+00f,     0.000000e+00f,     0.000000e+00f, 
};
/* Memory requirements */
static const memory_requirements_t g_memory_requirements = {
    .total_weights_size     = 304U,
    .max_activations_size   = 32U,
    .max_intermediate_size  = 0U,
};

/* Model metadata */
static const model_meta_t g_model_meta = {
    .layers             = g_layers,
    .num_layers         = 3U,
    .layers_capacity    = 3U,
    .model_input_shape  = {.ndim = 2, .dims = {1, 4, 1, 1}},
    .model_output_shape = {.ndim = 2, .dims = {1, 4, 1, 1}},
};

#endif /* MODEL_H */
