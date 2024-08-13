#ifndef _ARM_MATH_H_
#define _ARM_MATH_H_

#ifdef   __cplusplus
extern "C"
{
#endif

#if defined(__riscv)

#ifdef __riscv_xthead
#include "csi_instance.h"
#endif

float32_t arm_sin_f32(float32_t x);
float32_t arm_cos_f32(float32_t x);
q31_t arm_sin_q31(q31_t x);
q31_t arm_cos_q31(q31_t x);

#else

#include "arm_math_types.h"
#include "arm_math_memory.h"
#include "dsp/fast_math_functions.h"

#endif

#ifdef   __cplusplus
}
#endif

#endif

