/*
 * MCU_private.h
 *
 * Course Support License -- for instructional use for courses.  Not for
 * government, research, commercial, or other organizational use.
 *
 * Code generation for model "MCU".
 *
 * Model version              : 14.52
 * Simulink Coder version : 24.2 (R2024b) 21-Jun-2024
 * C source code generated on : Fri Jun 20 13:50:42 2025
 *
 * Target selection: cn_mingw.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef MCU_private_h_
#define MCU_private_h_
#include "rtwtypes.h"
#include "builtin_typeid_types.h"
#include "multiword_types.h"
#include "zero_crossing_types.h"
#include "MCU.h"
#include "MCU_types.h"

extern void rt_invd2x2_snf(const real_T u[4], real_T y[4]);
extern real_T rt_atan2d_snf(real_T u0, real_T u1);
extern void anorsimstepinitshmem(SimStruct *rts);
extern void sfun_spssw_discc_DSS(SimStruct *rts);
extern void MCU_Subsystem2(real_T rtu_In1, B_Subsystem2_MCU_T *localB);
extern void MCU_CustomRelay_Init(DW_CustomRelay_MCU_T *localDW,
  P_CustomRelay_MCU_T *localP);
extern void MCU_CustomRelay_Update(B_CustomRelay_MCU_T *localB,
  DW_CustomRelay_MCU_T *localDW);
extern void MCU_CustomRelay(real32_T rtu_Signal, real_T rtu_TH,
  B_CustomRelay_MCU_T *localB, DW_CustomRelay_MCU_T *localDW,
  P_CustomRelay_MCU_T *localP);

#endif                                 /* MCU_private_h_ */
