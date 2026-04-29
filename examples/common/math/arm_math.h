#ifndef _ARM_MATH_H_
#define _ARM_MATH_H_

#ifdef   __cplusplus
extern "C"
{
#endif

#if defined(__riscv)

#include "csi_math.h"

#define arm_sin_f32                     csi_sin_f32
#define arm_cos_f32                     csi_cos_f32
#define arm_sin_q31                     csi_sin_q31
#define arm_cos_q31                     csi_cos_q31

#define arm_rfft_fast_instance_f32      csi_rfft_fast_instance_f32
#define arm_rfft_fast_init_f32          csi_rfft_fast_init_f32
#define arm_rfft_fast_f32               csi_rfft_fast_f32
#define arm_cmplx_mag_f32               csi_cmplx_mag_f32

#else

#include "arm_math_types.h"
#include "arm_math_memory.h"
#include "dsp/fast_math_functions.h"
#include "dsp/complex_math_functions.h"
#include "dsp/transform_functions.h"

#endif

#ifdef   __cplusplus
}
#endif

#endif
