#ifndef USER_CONFIG_H
#define USER_CONFIG_H

/*
 * User-specific overrides.
 * Modify these values for your model and hardware.
 */

/* Model dimensions — override to match your model */
#define USER_INPUT_DIMS     { 4 }
#define USER_INPUT_NDIM     1
#define USER_OUTPUT_DIMS    { 4 }
#define USER_OUTPUT_NDIM    1

/* Tuning parameters */
#define USER_BATCH_SIZE     1

/* Feature toggles */
#define USER_ENABLE_CUSTOM_PREPROCESS   1
#define USER_ENABLE_CUSTOM_POSTPROCESS  0
#define USER_ENABLE_CUSTOM_INFER        0

#endif /* USER_CONFIG_H */
