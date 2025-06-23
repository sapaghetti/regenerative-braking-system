/*
 * MCU.c
 *
 * Course Support License -- for instructional use for courses.  Not for
 * government, research, commercial, or other organizational use.
 *
 * Code generation for model "MCU".
 *
 * Model version              : 14.53
 * Simulink Coder version : 24.2 (R2024b) 21-Jun-2024
 * C source code generated on : Mon Jun 23 10:58:58 2025
 *
 * Target selection: cn_mingw.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "MCU.h"
#include "rtwtypes.h"
#include "MCU_private.h"
#include <math.h>
#include <emmintrin.h>
#include "rt_nonfinite.h"
#include "rt_logging_mmi.h"
#include "MCU_capi.h"
#include <string.h>
#include "rt_defines.h"

/* Block signals (default storage) */
B_MCU_T MCU_B;

/* Block states (default storage) */
DW_MCU_T MCU_DW;

/* Real-time model */
static RT_MODEL_MCU_T MCU_M_;
RT_MODEL_MCU_T *const MCU_M = &MCU_M_;
static void rate_scheduler(void);

/*
 *         This function updates active task flag for each subrate.
 *         The function is called at model base rate, hence the
 *         generated code self-manages all its subrates.
 */
static void rate_scheduler(void)
{
  /* Compute which subrates run during the next base time step.  Subrates
   * are an integer multiple of the base rate counter.  Therefore, the subtask
   * counter is reset when it reaches its limit (zero means run).
   */
  (MCU_M->Timing.TaskCounters.TID[1])++;
  if ((MCU_M->Timing.TaskCounters.TID[1]) > 1999) {/* Sample time: [0.01s, 0.0s] */
    MCU_M->Timing.TaskCounters.TID[1] = 0;
  }

  MCU_M->Timing.sampleHits[1] = (MCU_M->Timing.TaskCounters.TID[1] == 0) ? 1 : 0;
}

/*
 * Output and update for atomic system:
 *    '<Root>/Subsystem'
 *    '<Root>/Subsystem2'
 *    '<Root>/Subsystem3'
 */
void MCU_Subsystem(real_T rtu_In1, B_Subsystem_MCU_T *localB)
{
  /* SignalConversion generated from: '<S5>/In1' */
  localB->In1 = rtu_In1;
}

/*
 * System initialize for atomic system:
 *    '<S16>/Custom Relay'
 *    '<S16>/Custom Relay1'
 *    '<S16>/Custom Relay2'
 */
void MCU_CustomRelay_Init(DW_CustomRelay_MCU_T *localDW, P_CustomRelay_MCU_T
  *localP)
{
  /* InitializeConditions for Delay: '<S45>/Delay' */
  localDW->Delay_DSTATE = localP->Delay_InitialCondition;
}

/*
 * Outputs for atomic system:
 *    '<S16>/Custom Relay'
 *    '<S16>/Custom Relay1'
 *    '<S16>/Custom Relay2'
 */
void MCU_CustomRelay(real32_T rtu_Signal, real_T rtu_TH, B_CustomRelay_MCU_T
                     *localB, DW_CustomRelay_MCU_T *localDW, P_CustomRelay_MCU_T
                     *localP)
{
  /* MultiPortSwitch: '<S45>/Index Vector' incorporates:
   *  Constant: '<S45>/offset'
   *  Constant: '<S45>/rev'
   *  Product: '<S45>/Product'
   *  RelationalOperator: '<S45>/Relational Operator'
   *  RelationalOperator: '<S45>/Relational Operator1'
   *  Sum: '<S45>/Add'
   */
  switch ((int32_T)((localP->offset_Value - (real_T)(rtu_TH >= rtu_Signal)) +
                    (real_T)(rtu_Signal >= rtu_TH * localP->rev_Value))) {
   case 0:
    /* MultiPortSwitch: '<S45>/Index Vector' incorporates:
     *  Constant: '<S45>/OFF'
     */
    localB->IndexVector = localP->OFF_Value;
    break;

   case 1:
    /* MultiPortSwitch: '<S45>/Index Vector' incorporates:
     *  Delay: '<S45>/Delay'
     */
    localB->IndexVector = localDW->Delay_DSTATE;
    break;

   default:
    /* MultiPortSwitch: '<S45>/Index Vector' incorporates:
     *  Constant: '<S45>/ON'
     */
    localB->IndexVector = localP->ON_Value;
    break;
  }

  /* End of MultiPortSwitch: '<S45>/Index Vector' */
}

/*
 * Update for atomic system:
 *    '<S16>/Custom Relay'
 *    '<S16>/Custom Relay1'
 *    '<S16>/Custom Relay2'
 */
void MCU_CustomRelay_Update(B_CustomRelay_MCU_T *localB, DW_CustomRelay_MCU_T
  *localDW)
{
  /* Update for Delay: '<S45>/Delay' */
  localDW->Delay_DSTATE = localB->IndexVector;
}

void rt_invd2x2_snf(const real_T u[4], real_T y[4])
{
  if (fabs(u[1]) > fabs(u[0])) {
    real_T r;
    real_T t;
    r = u[0] / u[1];
    t = 1.0 / (r * u[3] - u[2]);
    y[0] = u[3] / u[1] * t;
    y[1] = -t;
    y[2] = -u[2] / u[1] * t;
    y[3] = r * t;
  } else {
    real_T r;
    real_T t;
    r = u[1] / u[0];
    t = 1.0 / (u[3] - r * u[2]);
    y[0] = u[3] / u[0] * t;
    y[1] = -r * t;
    y[2] = -u[2] / u[0] * t;
    y[3] = t;
  }
}

real_T rt_atan2d_snf(real_T u0, real_T u1)
{
  real_T y;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = (rtNaN);
  } else if (rtIsInf(u0) && rtIsInf(u1)) {
    int32_T tmp;
    int32_T tmp_0;
    if (u0 > 0.0) {
      tmp = 1;
    } else {
      tmp = -1;
    }

    if (u1 > 0.0) {
      tmp_0 = 1;
    } else {
      tmp_0 = -1;
    }

    y = atan2(tmp, tmp_0);
  } else if (u1 == 0.0) {
    if (u0 > 0.0) {
      y = RT_PI / 2.0;
    } else if (u0 < 0.0) {
      y = -(RT_PI / 2.0);
    } else {
      y = 0.0;
    }
  } else {
    y = atan2(u0, u1);
  }

  return y;
}

/* Model output function */
static void MCU_output(void)
{
  __m128d tmp_4;
  __m128d tmp_5;
  real_T rtb_Product1_g[10];
  real_T rtb_Sum5_0[10];
  real_T rtb_u3L[10];
  real_T rtb_TmpSignalConversionAtProduc[5];
  real_T rtb_Sum5[4];
  real_T rtb_inversion[4];
  real_T tmp_0[4];
  real_T tmp_1[2];
  real_T tmp_2[2];
  real_T rtb_AdX_idx_1;
  real_T rtb_CastToDouble;
  real_T rtb_CoulombCounter;
  real_T rtb_Gain4;
  real_T rtb_Gain4_g;
  real_T rtb_Gain4_k;
  real_T rtb_Gain_m;
  real_T rtb_Integrator;
  real_T rtb_Product1_k;
  real_T rtb_Product4_0;
  real_T rtb_Product4_idx_0;
  real_T rtb_Product4_idx_1;
  real_T rtb_Product4_idx_2;
  real_T rtb_inversion_0;
  real_T rtb_inversion_1;
  real_T rtb_inversion_2;
  int32_T first2columnsofD_tmp;
  int32_T i;
  int32_T tmp_3;
  boolean_T rtb_Halleffectsignalh_a;
  boolean_T rtb_Halleffectsignalh_b;
  boolean_T rtb_Halleffectsignalh_c;
  boolean_T rtb_LogicalOperator1;
  boolean_T rtb_LogicalOperator2_d;
  boolean_T rtb_LogicalOperator6;
  boolean_T tmp;

  /* S-Function (anorsimstepinitshmem): '<S4>/S-Function' */

  /* Level2 S-Function Block: '<S4>/S-Function' (anorsimstepinitshmem) */
  {
    SimStruct *rts = MCU_M->childSfunctions[0];
    sfcnOutputs(rts,0);
  }

  for (i = 0; i < 6; i++) {
    /* Constant: '<S44>/Constant' */
    MCU_B.Constant[i] = MCU_P.Constant_Value_o[i];
  }

  /* Saturate: '<S23>/Saturation' incorporates:
   *  UnitDelay: '<S23>/Unit Delay'
   */
  if (MCU_DW.UnitDelay_DSTATE > MCU_P.Saturation_UpperSat) {
    /* Saturate: '<S23>/Saturation' */
    MCU_B.VoltageV = MCU_P.Saturation_UpperSat;
  } else if (MCU_DW.UnitDelay_DSTATE < MCU_P.Saturation_LowerSat) {
    /* Saturate: '<S23>/Saturation' */
    MCU_B.VoltageV = MCU_P.Saturation_LowerSat;
  } else {
    /* Saturate: '<S23>/Saturation' */
    MCU_B.VoltageV = MCU_DW.UnitDelay_DSTATE;
  }

  /* End of Saturate: '<S23>/Saturation' */

  /* Gain: '<S36>/Tustin: Ts//2 BE : Ts' incorporates:
   *  Constant: '<S32>/I'
   *  Gain: '<S32>/-R//L'
   *  Product: '<S36>/Product4'
   */
  rtb_Product4_0 = MCU_P.RL_Gain * MCU_P.I_Value[0] * MCU_P.TustinTs2BETs_Gain;
  rtb_Product4_idx_0 = rtb_Product4_0;

  /* Sum: '<S36>/Sum1' incorporates:
   *  Constant: '<S36>/u5'
   *  Product: '<S36>/Product4'
   */
  tmp_0[0] = MCU_P.u5_Value[0] - rtb_Product4_0;

  /* Gain: '<S36>/Tustin: Ts//2 BE : Ts' incorporates:
   *  Constant: '<S32>/I'
   *  Gain: '<S32>/-R//L'
   *  Product: '<S36>/Product4'
   */
  rtb_Product4_0 = MCU_P.RL_Gain * MCU_P.I_Value[1] * MCU_P.TustinTs2BETs_Gain;
  rtb_Product4_idx_1 = rtb_Product4_0;

  /* Sum: '<S36>/Sum1' incorporates:
   *  Constant: '<S36>/u5'
   *  Product: '<S36>/Product4'
   */
  tmp_0[1] = MCU_P.u5_Value[1] - rtb_Product4_0;

  /* Gain: '<S36>/Tustin: Ts//2 BE : Ts' incorporates:
   *  Constant: '<S32>/I'
   *  Gain: '<S32>/-R//L'
   *  Product: '<S36>/Product4'
   */
  rtb_Product4_0 = MCU_P.RL_Gain * MCU_P.I_Value[2] * MCU_P.TustinTs2BETs_Gain;
  rtb_Product4_idx_2 = rtb_Product4_0;

  /* Sum: '<S36>/Sum1' incorporates:
   *  Constant: '<S36>/u5'
   *  Product: '<S36>/Product4'
   */
  tmp_0[2] = MCU_P.u5_Value[2] - rtb_Product4_0;

  /* Gain: '<S36>/Tustin: Ts//2 BE : Ts' incorporates:
   *  Constant: '<S32>/I'
   *  Gain: '<S32>/-R//L'
   *  Product: '<S36>/Product4'
   */
  rtb_Product4_0 = MCU_P.RL_Gain * MCU_P.I_Value[3] * MCU_P.TustinTs2BETs_Gain;

  /* Sum: '<S36>/Sum1' incorporates:
   *  Constant: '<S36>/u5'
   *  Product: '<S36>/Product4'
   */
  tmp_0[3] = MCU_P.u5_Value[3] - rtb_Product4_0;

  /* Product: '<S36>/inversion' */
  rt_invd2x2_snf(tmp_0, rtb_inversion);

  /* Product: '<S36>/Product2' incorporates:
   *  Constant: '<S32>/I'
   *  Product: '<S36>/inversion'
   */
  rtb_Gain_m = MCU_P.I_Value[2];
  rtb_Gain4 = MCU_P.I_Value[0];
  rtb_Product1_k = MCU_P.I_Value[3];
  rtb_Gain4_k = MCU_P.I_Value[1];
  for (i = 0; i <= 0; i += 2) {
    first2columnsofD_tmp = (i + 1) << 1;
    tmp_3 = i << 1;
    tmp_4 = _mm_set_pd(rtb_inversion[first2columnsofD_tmp + 1],
                       rtb_inversion[tmp_3 + 1]);
    tmp_5 = _mm_set_pd(rtb_inversion[first2columnsofD_tmp], rtb_inversion[tmp_3]);
    _mm_storeu_pd(&tmp_2[0], _mm_add_pd(_mm_mul_pd(tmp_4, _mm_set1_pd(rtb_Gain_m)),
      _mm_mul_pd(tmp_5, _mm_set1_pd(rtb_Gain4))));
    tmp_0[tmp_3] = tmp_2[0];
    tmp_0[first2columnsofD_tmp] = tmp_2[1];
    _mm_storeu_pd(&tmp_2[0], _mm_add_pd(_mm_mul_pd(tmp_4, _mm_set1_pd
      (rtb_Product1_k)), _mm_mul_pd(tmp_5, _mm_set1_pd(rtb_Gain4_k))));
    tmp_0[tmp_3 + 1] = tmp_2[0];
    tmp_0[first2columnsofD_tmp + 1] = tmp_2[1];
  }

  /* End of Product: '<S36>/Product2' */

  /* Gain: '<S36>/wbase3' */
  tmp_5 = _mm_set1_pd(MCU_P.wbase3_Gain);

  /* Gain: '<S36>/wbase3' incorporates:
   *  Sum: '<S36>/Sum5'
   */
  tmp_4 = _mm_mul_pd(tmp_5, _mm_loadu_pd(&tmp_0[0]));
  _mm_storeu_pd(&rtb_Sum5[0], tmp_4);
  tmp_4 = _mm_mul_pd(tmp_5, _mm_loadu_pd(&tmp_0[2]));
  _mm_storeu_pd(&rtb_Sum5[2], tmp_4);

  /* Gain: '<S32>/1//(3*L)' incorporates:
   *  Constant: '<S32>/M(2,5)'
   */
  for (i = 0; i <= 8; i += 2) {
    _mm_storeu_pd(&rtb_u3L[i], _mm_mul_pd(_mm_set1_pd(MCU_P.u3L_Gain),
      _mm_loadu_pd(&MCU_P.M25_Value[i])));
  }

  /* End of Gain: '<S32>/1//(3*L)' */

  /* Product: '<S36>/Product3' incorporates:
   *  Gain: '<S32>/1//(3*L)'
   *  Sum: '<S36>/Sum5'
   */
  rtb_Gain_m = rtb_Sum5[2];
  rtb_Gain4 = rtb_Sum5[0];
  rtb_Product1_k = rtb_Sum5[3];
  rtb_Gain4_k = rtb_Sum5[1];
  for (i = 0; i <= 2; i += 2) {
    first2columnsofD_tmp = (i + 1) << 1;
    tmp_3 = i << 1;
    tmp_4 = _mm_set_pd(rtb_u3L[first2columnsofD_tmp + 1], rtb_u3L[tmp_3 + 1]);
    tmp_5 = _mm_set_pd(rtb_u3L[first2columnsofD_tmp], rtb_u3L[tmp_3]);
    _mm_storeu_pd(&tmp_2[0], _mm_add_pd(_mm_mul_pd(tmp_4, _mm_set1_pd(rtb_Gain_m)),
      _mm_mul_pd(tmp_5, _mm_set1_pd(rtb_Gain4))));
    rtb_Sum5_0[tmp_3] = tmp_2[0];
    rtb_Sum5_0[first2columnsofD_tmp] = tmp_2[1];
    _mm_storeu_pd(&tmp_2[0], _mm_add_pd(_mm_mul_pd(tmp_4, _mm_set1_pd
      (rtb_Product1_k)), _mm_mul_pd(tmp_5, _mm_set1_pd(rtb_Gain4_k))));
    rtb_Sum5_0[tmp_3 + 1] = tmp_2[0];
    rtb_Sum5_0[first2columnsofD_tmp + 1] = tmp_2[1];
  }

  for (i = 4; i < 5; i++) {
    first2columnsofD_tmp = i << 1;
    _mm_storeu_pd(&rtb_Sum5_0[first2columnsofD_tmp], _mm_add_pd(_mm_mul_pd
      (_mm_set1_pd(rtb_u3L[first2columnsofD_tmp + 1]), _mm_set_pd(rtb_Product1_k,
      rtb_Gain_m)), _mm_mul_pd(_mm_set1_pd(rtb_u3L[first2columnsofD_tmp]),
      _mm_set_pd(rtb_Gain4_k, rtb_Gain4))));
  }

  /* End of Product: '<S36>/Product3' */

  /* Gain: '<S36>/Tustin: 1//2 BE : 1.0' incorporates:
   *  Product: '<S36>/Product1'
   */
  for (i = 0; i <= 8; i += 2) {
    tmp_4 = _mm_loadu_pd(&rtb_Sum5_0[i]);
    _mm_storeu_pd(&rtb_Product1_g[i], _mm_mul_pd(_mm_set1_pd
      (MCU_P.Tustin12BE10_Gain), tmp_4));
  }

  /* End of Gain: '<S36>/Tustin: 1//2 BE : 1.0' */

  /* Fcn: '<S39>/Fcn' incorporates:
   *  DiscreteIntegrator: '<S34>/Discrete-Time Integrator1'
   *  Trigonometry: '<S42>/Trigonometric Function1'
   */
  rtb_Gain_m = cos(MCU_DW.DiscreteTimeIntegrator1_DSTATE);

  /* Saturate: '<S39>/Saturation' incorporates:
   *  Fcn: '<S39>/Fcn'
   */
  if (rtb_Gain_m > MCU_P.Saturation_UpperSat_n) {
    rtb_CastToDouble = MCU_P.Saturation_UpperSat_n;
  } else if (rtb_Gain_m < MCU_P.Saturation_LowerSat_g) {
    rtb_CastToDouble = MCU_P.Saturation_LowerSat_g;
  } else {
    rtb_CastToDouble = rtb_Gain_m;
  }

  /* Gain: '<S39>/Gain4' incorporates:
   *  Saturate: '<S39>/Saturation'
   */
  rtb_Gain4 = MCU_P.Gain4_Gain * rtb_CastToDouble;

  /* Gain: '<S34>/Gain' incorporates:
   *  DiscreteIntegrator: '<S34>/Discrete-Time Integrator'
   */
  MCU_B.Gain = MCU_P.Gain_Gain * MCU_DW.DiscreteTimeIntegrator_DSTATE;

  /* Product: '<S39>/Product1' */
  rtb_Product1_k = rtb_Gain4 * MCU_B.Gain;

  /* Fcn: '<S40>/Fcn' incorporates:
   *  DiscreteIntegrator: '<S34>/Discrete-Time Integrator1'
   */
  rtb_Gain4_g = cos(MCU_DW.DiscreteTimeIntegrator1_DSTATE - 2.0943951023931953);

  /* Saturate: '<S40>/Saturation' */
  if (rtb_Gain4_g > MCU_P.Saturation_UpperSat_i) {
    rtb_Gain4_g = MCU_P.Saturation_UpperSat_i;
  } else if (rtb_Gain4_g < MCU_P.Saturation_LowerSat_d) {
    rtb_Gain4_g = MCU_P.Saturation_LowerSat_d;
  }

  /* Gain: '<S40>/Gain4' incorporates:
   *  Saturate: '<S40>/Saturation'
   */
  rtb_Gain4_k = MCU_P.Gain4_Gain_l * rtb_Gain4_g;

  /* Product: '<S40>/Product1' */
  rtb_Integrator = rtb_Gain4_k * MCU_B.Gain;

  /* Fcn: '<S41>/Fcn' incorporates:
   *  DiscreteIntegrator: '<S34>/Discrete-Time Integrator1'
   */
  rtb_Gain4_g = cos(MCU_DW.DiscreteTimeIntegrator1_DSTATE + 2.0943951023931953);

  /* Saturate: '<S41>/Saturation' */
  if (rtb_Gain4_g > MCU_P.Saturation_UpperSat_f) {
    rtb_Gain4_g = MCU_P.Saturation_UpperSat_f;
  } else if (rtb_Gain4_g < MCU_P.Saturation_LowerSat_f) {
    rtb_Gain4_g = MCU_P.Saturation_LowerSat_f;
  }

  /* Gain: '<S41>/Gain4' incorporates:
   *  Saturate: '<S41>/Saturation'
   */
  rtb_Gain4_g *= MCU_P.Gain4_Gain_j;

  /* Product: '<S41>/Product1' */
  rtb_CastToDouble = rtb_Gain4_g * MCU_B.Gain;

  /* UnitDelay: '<S38>/Unit Delay' incorporates:
   *  Product: '<S38>/Product2'
   */
  rtb_AdX_idx_1 = MCU_DW.UnitDelay_DSTATE_n[1];
  rtb_CoulombCounter = MCU_DW.UnitDelay_DSTATE_n[0];
  for (i = 0; i < 2; i++) {
    /* Sum: '<S38>/Add2' incorporates:
     *  Product: '<S36>/Product1'
     *  Product: '<S38>/Product2'
     *  Product: '<S38>/Product3'
     *  Selector: '<S32>/last 3 columns of D'
     *  SignalConversion generated from: '<S38>/Product3'
     *  Sum: '<S36>/Sum5'
     *  UnitDelay: '<S38>/Unit Delay'
     */
    MCU_B.Add2[i] = ((rtb_Product1_g[i + 4] * rtb_Product1_k + rtb_Product1_g[i
                      + 6] * rtb_Integrator) + rtb_Product1_g[i + 8] *
                     rtb_CastToDouble) + (rtb_Sum5[i + 2] * rtb_AdX_idx_1 +
      rtb_Sum5[i] * rtb_CoulombCounter);

    /* Selector: '<S32>/first 2 columns of D' */
    first2columnsofD_tmp = i << 1;

    /* Selector: '<S32>/first 2 columns of D' incorporates:
     *  Product: '<S36>/Product1'
     */
    MCU_B.first2columnsofD[first2columnsofD_tmp] =
      rtb_Product1_g[first2columnsofD_tmp];
    MCU_B.first2columnsofD[first2columnsofD_tmp + 1] =
      rtb_Product1_g[first2columnsofD_tmp + 1];
  }

  /* Sum: '<S48>/Sum' incorporates:
   *  Selector: '<S32>/first 2 columns of D'
   *  UnitDelay: '<S48>/Unit Delay'
   */
  tmp_4 = _mm_sub_pd(_mm_loadu_pd(&MCU_B.first2columnsofD[0]), _mm_loadu_pd
                     (&MCU_DW.UnitDelay_DSTATE_g[0]));

  /* Sum: '<S48>/Sum' */
  _mm_storeu_pd(&MCU_B.Sum[0], tmp_4);

  /* Sum: '<S48>/Sum' incorporates:
   *  Selector: '<S32>/first 2 columns of D'
   *  UnitDelay: '<S48>/Unit Delay'
   */
  tmp_4 = _mm_sub_pd(_mm_loadu_pd(&MCU_B.first2columnsofD[2]), _mm_loadu_pd
                     (&MCU_DW.UnitDelay_DSTATE_g[2]));

  /* Sum: '<S48>/Sum' */
  _mm_storeu_pd(&MCU_B.Sum[2], tmp_4);

  /* S-Function (sfun_spssw_discc_DSS): '<S48>/State-Space' */

  /* Level2 S-Function Block: '<S48>/State-Space' (sfun_spssw_discc_DSS) */
  {
    SimStruct *rts = MCU_M->childSfunctions[1];
    sfcnOutputs(rts,0);
  }

  /* Gain: '<Root>/Gain2' incorporates:
   *  DiscreteIntegrator: '<S34>/Discrete-Time Integrator'
   */
  MCU_B.Gain2 = MCU_P.Gain2_Gain * MCU_DW.DiscreteTimeIntegrator_DSTATE;
  tmp = (MCU_M->Timing.TaskCounters.TID[1] == 0);
  if (tmp) {
    /* Outputs for Atomic SubSystem: '<Root>/Subsystem' */
    MCU_Subsystem(MCU_B.Gain2, &MCU_B.Subsystem);

    /* End of Outputs for SubSystem: '<Root>/Subsystem' */

    /* S-Function (sysvarout): '<S12>/S-Function' */
    if (cnSetSystemVariableValues(MCU_DW.SFunction_SysVar, 1,
         &MCU_B.Subsystem.In1, 0) != 0) {
      rtmSetErrorStatus(MCU_M, cnGetErrorMessage());
    }
  }

  /* Gain: '<S42>/rad2deg' incorporates:
   *  DiscreteIntegrator: '<S34>/Discrete-Time Integrator1'
   *  Trigonometry: '<S42>/Trigonometric Function'
   *  Trigonometry: '<S42>/Trigonometric Function2'
   */
  rtb_Gain_m = MCU_P.rad2deg_Gain * rt_atan2d_snf(sin
    (MCU_DW.DiscreteTimeIntegrator1_DSTATE), rtb_Gain_m);

  /* DataTypeConversion: '<Root>/Data Type Conversion10' incorporates:
   *  Constant: '<S37>/Constant4'
   *  Constant: '<S37>/Constant5'
   *  Logic: '<S37>/Logical Operator2'
   *  RelationalOperator: '<S37>/Relational Operator5'
   *  RelationalOperator: '<S37>/Relational Operator6'
   */
  rtb_Halleffectsignalh_c = ((rtb_Gain_m >= MCU_P.Constant4_Value) &&
    (rtb_Gain_m <= MCU_P.Constant5_Value));

  /* SignalConversion generated from: '<S38>/Product1' */
  rtb_TmpSignalConversionAtProduc[0] = MCU_B.StateSpace_o1[7];
  rtb_TmpSignalConversionAtProduc[1] = MCU_B.StateSpace_o1[8];
  rtb_TmpSignalConversionAtProduc[2] = rtb_Product1_k;
  rtb_TmpSignalConversionAtProduc[3] = rtb_Integrator;
  rtb_TmpSignalConversionAtProduc[4] = rtb_CastToDouble;

  /* Sum: '<S38>/Add1' incorporates:
   *  Gain: '<S32>/1//(3*L)'
   *  Gain: '<S38>/BE=1 Tustin=1//2'
   *  Product: '<S38>/Product8'
   *  UnitDelay: '<S38>/Unit Delay'
   */
  for (i = 0; i < 2; i++) {
    rtb_CastToDouble = 0.0;
    for (first2columnsofD_tmp = 0; first2columnsofD_tmp < 5;
         first2columnsofD_tmp++) {
      rtb_CastToDouble += rtb_u3L[(first2columnsofD_tmp << 1) + i] *
        (MCU_P.BE1Tustin12_Gain *
         rtb_TmpSignalConversionAtProduc[first2columnsofD_tmp]);
    }

    tmp_1[i] = MCU_DW.UnitDelay_DSTATE_n[i] + rtb_CastToDouble;
  }

  /* End of Sum: '<S38>/Add1' */

  /* Product: '<S38>/Product6' incorporates:
   *  Gain: '<S38>/Ts'
   *  Product: '<S36>/inversion'
   */
  tmp_4 = _mm_mul_pd(_mm_add_pd(_mm_mul_pd(_mm_set_pd(tmp_1[0], rtb_inversion[0]),
    _mm_set_pd(rtb_inversion[1], tmp_1[0])), _mm_mul_pd(_mm_set1_pd(tmp_1[1]),
    _mm_loadu_pd(&rtb_inversion[2]))), _mm_set1_pd(MCU_P.Ts_Gain));
  _mm_storeu_pd(&tmp_2[0], tmp_4);

  /* Gain: '<S38>/Ts' */
  rtb_Integrator = tmp_2[0];
  rtb_AdX_idx_1 = tmp_2[1];

  /* Sum: '<S32>/Add4' */
  rtb_CoulombCounter = (0.0 - tmp_2[0]) - tmp_2[1];

  /* S-Function (sysvarin): '<S9>/S-Function' */
  if (cnGetSystemVariableValues(MCU_DW.SFunction_SysVar_k, 1, &MCU_B.SFunction_j,
       0) != 0) {
    rtmSetErrorStatus(MCU_M, cnGetErrorMessage());
  }

  /* Outputs for Atomic SubSystem: '<Root>/speed controller' */
  /* Sum: '<S18>/Add3' */
  rtb_CastToDouble = MCU_B.SFunction_j - MCU_B.Gain2;

  /* Gain: '<S87>/Integral Gain' */
  MCU_B.IntegralGain = MCU_P.DiscretePIDController_I * rtb_CastToDouble;

  /* Sum: '<S99>/Sum' incorporates:
   *  DiscreteIntegrator: '<S90>/Integrator'
   *  Gain: '<S95>/Proportional Gain'
   */
  rtb_Product1_k = MCU_P.DiscretePIDController_P * rtb_CastToDouble +
    MCU_DW.Integrator_DSTATE;

  /* End of Outputs for SubSystem: '<Root>/speed controller' */

  /* DataTypeConversion: '<Root>/Data Type Conversion8' incorporates:
   *  Constant: '<S37>/Constant'
   *  Constant: '<S37>/Constant1'
   *  Logic: '<S37>/Logical Operator'
   *  RelationalOperator: '<S37>/Relational Operator1'
   *  RelationalOperator: '<S37>/Relational Operator2'
   */
  rtb_Halleffectsignalh_a = ((rtb_Gain_m >= MCU_P.Constant_Value_l) &&
    (rtb_Gain_m <= MCU_P.Constant1_Value));

  /* DataTypeConversion: '<Root>/Data Type Conversion9' incorporates:
   *  Constant: '<S37>/Constant2'
   *  Constant: '<S37>/Constant3'
   *  Logic: '<S37>/Logical Operator1'
   *  RelationalOperator: '<S37>/Relational Operator3'
   *  RelationalOperator: '<S37>/Relational Operator4'
   */
  rtb_Halleffectsignalh_b = ((rtb_Gain_m >= MCU_P.Constant2_Value) ||
    (rtb_Gain_m <= MCU_P.Constant3_Value));

  /* Outputs for Atomic SubSystem: '<Root>/Decoder' */
  /* Logic: '<S2>/NOT1' incorporates:
   *  Constant: '<S37>/Constant2'
   *  Constant: '<S37>/Constant3'
   *  Logic: '<S37>/Logical Operator1'
   *  RelationalOperator: '<S37>/Relational Operator3'
   *  RelationalOperator: '<S37>/Relational Operator4'
   */
  rtb_LogicalOperator2_d = ((!(rtb_Gain_m >= MCU_P.Constant2_Value)) &&
    (!(rtb_Gain_m <= MCU_P.Constant3_Value)));

  /* Logic: '<S2>/NOT2' */
  rtb_LogicalOperator6 = !rtb_Halleffectsignalh_c;

  /* Logic: '<S2>/NOT' incorporates:
   *  Constant: '<S37>/Constant'
   *  Constant: '<S37>/Constant1'
   *  Logic: '<S37>/Logical Operator'
   *  RelationalOperator: '<S37>/Relational Operator1'
   *  RelationalOperator: '<S37>/Relational Operator2'
   */
  rtb_LogicalOperator1 = ((!(rtb_Gain_m >= MCU_P.Constant_Value_l)) ||
    (!(rtb_Gain_m <= MCU_P.Constant1_Value)));

  /* End of Outputs for SubSystem: '<Root>/Decoder' */

  /* S-Function (sysvarin): '<S10>/S-Function' */
  if (cnGetSystemVariableValues(MCU_DW.SFunction_SysVar_a, 1, &MCU_B.SFunction_m,
       0) != 0) {
    rtmSetErrorStatus(MCU_M, cnGetErrorMessage());
  }

  /* MultiPortSwitch: '<Root>/Index Vector' incorporates:
   *  Constant: '<Root>/AUTO'
   *  Constant: '<Root>/Regen_Mode1'
   *  Constant: '<Root>/Regen_Mode2'
   *  Constant: '<Root>/Regen_Mode3'
   */
  switch ((int32_T)MCU_B.SFunction_m) {
   case 0:
    rtb_Gain_m = MCU_P.AUTO_Value;
    break;

   case 1:
    rtb_Gain_m = MCU_P.Regen_Mode1_Value;
    break;

   case 2:
    rtb_Gain_m = MCU_P.Regen_Mode2_Value;
    break;

   default:
    rtb_Gain_m = MCU_P.Regen_Mode3_Value;
    break;
  }

  /* End of MultiPortSwitch: '<Root>/Index Vector' */

  /* Product: '<S36>/Product1' incorporates:
   *  Gain: '<S32>/1//(3*L)'
   *  Product: '<S36>/inversion'
   */
  rtb_CastToDouble = rtb_inversion[2];
  rtb_inversion_0 = rtb_inversion[0];
  rtb_inversion_1 = rtb_inversion[3];
  rtb_inversion_2 = rtb_inversion[1];
  for (i = 0; i <= 2; i += 2) {
    first2columnsofD_tmp = (i + 1) << 1;
    tmp_3 = i << 1;
    tmp_4 = _mm_set_pd(rtb_u3L[first2columnsofD_tmp + 1], rtb_u3L[tmp_3 + 1]);
    tmp_5 = _mm_set_pd(rtb_u3L[first2columnsofD_tmp], rtb_u3L[tmp_3]);
    _mm_storeu_pd(&tmp_2[0], _mm_add_pd(_mm_mul_pd(tmp_4, _mm_set1_pd
      (rtb_CastToDouble)), _mm_mul_pd(tmp_5, _mm_set1_pd(rtb_inversion_0))));
    rtb_Sum5_0[tmp_3] = tmp_2[0];
    rtb_Sum5_0[first2columnsofD_tmp] = tmp_2[1];
    _mm_storeu_pd(&tmp_2[0], _mm_add_pd(_mm_mul_pd(tmp_4, _mm_set1_pd
      (rtb_inversion_1)), _mm_mul_pd(tmp_5, _mm_set1_pd(rtb_inversion_2))));
    rtb_Sum5_0[tmp_3 + 1] = tmp_2[0];
    rtb_Sum5_0[first2columnsofD_tmp + 1] = tmp_2[1];
  }

  for (i = 4; i < 5; i++) {
    first2columnsofD_tmp = i << 1;
    _mm_storeu_pd(&rtb_Sum5_0[first2columnsofD_tmp], _mm_add_pd(_mm_mul_pd
      (_mm_set1_pd(rtb_u3L[first2columnsofD_tmp + 1]), _mm_set_pd
       (rtb_inversion_1, rtb_CastToDouble)), _mm_mul_pd(_mm_set1_pd
      (rtb_u3L[first2columnsofD_tmp]), _mm_set_pd(rtb_inversion_2,
      rtb_inversion_0))));
  }

  /* End of Product: '<S36>/Product1' */

  /* Product: '<S36>/Product5' incorporates:
   *  Constant: '<S36>/u1'
   *  Constant: '<S36>/u5'
   *  Product: '<S36>/Product4'
   *  Sum: '<S36>/Sum5'
   */
  tmp_4 = _mm_set1_pd(MCU_P.u1_Value);
  _mm_storeu_pd(&tmp_2[0], _mm_add_pd(_mm_mul_pd(_mm_set_pd(rtb_Product4_idx_1,
    rtb_Product4_idx_0), tmp_4), _mm_loadu_pd(&MCU_P.u5_Value[0])));

  /* Sum: '<S36>/Sum5' */
  rtb_Product4_idx_0 = tmp_2[0];
  rtb_Product4_idx_1 = tmp_2[1];

  /* Gain: '<S36>/Tustin: Ts//2 BE : Ts' incorporates:
   *  Constant: '<S36>/u5'
   *  Product: '<S36>/Product4'
   *  Product: '<S36>/Product5'
   *  Sum: '<S36>/Sum5'
   */
  _mm_storeu_pd(&tmp_2[0], _mm_add_pd(_mm_mul_pd(_mm_set_pd(rtb_Product4_0,
    rtb_Product4_idx_2), tmp_4), _mm_loadu_pd(&MCU_P.u5_Value[2])));

  /* Sum: '<S36>/Sum5' */
  rtb_Product4_0 = tmp_2[0];
  rtb_Product4_idx_2 = tmp_2[1];
  for (i = 0; i < 2; i++) {
    /* Product: '<S36>/Product4' incorporates:
     *  Product: '<S36>/inversion'
     */
    rtb_CastToDouble = rtb_inversion[i + 2];
    rtb_inversion_0 = rtb_inversion[i];

    /* Product: '<S38>/Product1' */
    rtb_inversion_1 = 0.0;
    for (first2columnsofD_tmp = 0; first2columnsofD_tmp < 5;
         first2columnsofD_tmp++) {
      rtb_inversion_1 += rtb_Sum5_0[(first2columnsofD_tmp << 1) + i] *
        rtb_TmpSignalConversionAtProduc[first2columnsofD_tmp];
    }

    /* Sum: '<S38>/Add' incorporates:
     *  Product: '<S36>/Product4'
     *  Product: '<S38>/Product1'
     *  Product: '<S38>/Product5'
     *  UnitDelay: '<S38>/Unit Delay'
     */
    MCU_B.Add[i] = ((rtb_CastToDouble * rtb_Product4_idx_2 + rtb_inversion_0 *
                     rtb_Product4_0) * MCU_DW.UnitDelay_DSTATE_n[1] +
                    (rtb_CastToDouble * rtb_Product4_idx_1 + rtb_inversion_0 *
                     rtb_Product4_idx_0) * MCU_DW.UnitDelay_DSTATE_n[0]) +
      rtb_inversion_1;
  }

  /* Signum: '<S43>/Sign' incorporates:
   *  DiscreteIntegrator: '<S34>/Discrete-Time Integrator'
   */
  if (rtIsNaN(MCU_DW.DiscreteTimeIntegrator_DSTATE)) {
    rtb_CastToDouble = (rtNaN);
  } else if (MCU_DW.DiscreteTimeIntegrator_DSTATE < 0.0) {
    rtb_CastToDouble = -1.0;
  } else {
    rtb_CastToDouble = (MCU_DW.DiscreteTimeIntegrator_DSTATE > 0.0);
  }

  /* Gain: '<S34>/Gain2' incorporates:
   *  Constant: '<Root>/Constant'
   *  DiscreteIntegrator: '<S34>/Discrete-Time Integrator'
   *  Gain: '<S32>/Gain3'
   *  Gain: '<S43>/Gain'
   *  Gain: '<S43>/Gain1'
   *  Product: '<S32>/Product'
   *  Signum: '<S43>/Sign'
   *  Sum: '<S32>/Sum2'
   *  Sum: '<S34>/Sum'
   *  Sum: '<S43>/Sum'
   */
  MCU_B.Gain2_i = ((((rtb_Integrator * rtb_Gain4 + rtb_AdX_idx_1 * rtb_Gain4_k)
                     + rtb_CoulombCounter * rtb_Gain4_g) * MCU_P.Gain3_Gain -
                    MCU_P.Constant_Value_h) -
                   (MCU_P.CoulombViscousFriction_offset * rtb_CastToDouble +
                    MCU_P.CoulombViscousFriction_gain *
                    MCU_DW.DiscreteTimeIntegrator_DSTATE)) * MCU_P.Gain2_Gain_l;

  /* Outputs for Atomic SubSystem: '<Root>/current controller' */
  /* Outputs for Atomic SubSystem: '<S16>/Custom Relay' */
  /* Outputs for Atomic SubSystem: '<Root>/Decoder' */
  /* Sum: '<S16>/Add1' incorporates:
   *  DataTypeConversion: '<Root>/Data Type Conversion3'
   *  DataTypeConversion: '<Root>/Data Type Conversion7'
   *  DataTypeConversion: '<S2>/Data Type Conversion'
   *  DataTypeConversion: '<S2>/Data Type Conversion3'
   *  Logic: '<S2>/Logical Operator1'
   *  Logic: '<S2>/Logical Operator4'
   *  Product: '<Root>/Product2'
   *  Sum: '<S2>/Subtract'
   */
  MCU_CustomRelay((real32_T)rtb_Product1_k * ((real32_T)(rtb_Halleffectsignalh_a
    && rtb_LogicalOperator2_d) - (real32_T)(rtb_LogicalOperator1 &&
    rtb_Halleffectsignalh_b)) - (real32_T)rtb_Integrator, rtb_Gain_m,
                  &MCU_B.CustomRelay, &MCU_DW.CustomRelay, &MCU_P.CustomRelay);

  /* End of Outputs for SubSystem: '<S16>/Custom Relay' */

  /* Outputs for Atomic SubSystem: '<S16>/Custom Relay1' */
  /* Sum: '<S16>/Add2' incorporates:
   *  DataTypeConversion: '<Root>/Data Type Conversion3'
   *  DataTypeConversion: '<Root>/Data Type Conversion6'
   *  DataTypeConversion: '<S2>/Data Type Conversion1'
   *  DataTypeConversion: '<S2>/Data Type Conversion4'
   *  Logic: '<S2>/Logical Operator3'
   *  Logic: '<S2>/Logical Operator5'
   *  Product: '<Root>/Product2'
   *  Sum: '<S2>/Subtract1'
   */
  MCU_CustomRelay((real32_T)rtb_Product1_k * ((real32_T)(rtb_Halleffectsignalh_b
    && rtb_LogicalOperator6) - (real32_T)(rtb_LogicalOperator2_d &&
    rtb_Halleffectsignalh_c)) - (real32_T)rtb_AdX_idx_1, rtb_Gain_m,
                  &MCU_B.CustomRelay1, &MCU_DW.CustomRelay1, &MCU_P.CustomRelay1);

  /* End of Outputs for SubSystem: '<S16>/Custom Relay1' */

  /* Outputs for Atomic SubSystem: '<S16>/Custom Relay2' */
  /* Sum: '<S16>/Add4' incorporates:
   *  DataTypeConversion: '<Root>/Data Type Conversion11'
   *  DataTypeConversion: '<Root>/Data Type Conversion3'
   *  DataTypeConversion: '<S2>/Data Type Conversion2'
   *  DataTypeConversion: '<S2>/Data Type Conversion5'
   *  Logic: '<S2>/Logical Operator2'
   *  Logic: '<S2>/Logical Operator6'
   *  Product: '<Root>/Product2'
   *  Sum: '<S2>/Subtract2'
   */
  MCU_CustomRelay((real32_T)rtb_Product1_k * ((real32_T)(rtb_Halleffectsignalh_c
    && rtb_LogicalOperator1) - (real32_T)(rtb_LogicalOperator6 &&
    rtb_Halleffectsignalh_a)) - (real32_T)rtb_CoulombCounter, rtb_Gain_m,
                  &MCU_B.CustomRelay2, &MCU_DW.CustomRelay2, &MCU_P.CustomRelay2);

  /* End of Outputs for SubSystem: '<S16>/Custom Relay2' */
  /* End of Outputs for SubSystem: '<Root>/Decoder' */

  /* DataTypeConversion: '<S16>/Data Type Conversion1' incorporates:
   *  DataTypeConversion: '<S16>/Data Type Conversion'
   *  Logic: '<S16>/Logical Operator'
   */
  MCU_B.DataTypeConversion1 = !(MCU_B.CustomRelay.IndexVector != 0.0);

  /* DataTypeConversion: '<S16>/Data Type Conversion3' incorporates:
   *  DataTypeConversion: '<S16>/Data Type Conversion2'
   *  Logic: '<S16>/Logical Operator1'
   */
  MCU_B.DataTypeConversion3 = !(MCU_B.CustomRelay1.IndexVector != 0.0);

  /* DataTypeConversion: '<S16>/Data Type Conversion5' incorporates:
   *  DataTypeConversion: '<S16>/Data Type Conversion4'
   *  Logic: '<S16>/Logical Operator2'
   */
  MCU_B.DataTypeConversion5 = !(MCU_B.CustomRelay2.IndexVector != 0.0);

  /* End of Outputs for SubSystem: '<Root>/current controller' */

  /* Gain: '<S20>/do not delete this gain' */
  MCU_B.donotdeletethisgain = MCU_P.donotdeletethisgain_Gain *
    MCU_B.StateSpace_o1[6];

  /* DataTypeConversion: '<S23>/Data Type Conversion2' incorporates:
   *  Constant: '<S25>/Constant'
   *  RelationalOperator: '<S25>/Compare'
   */
  MCU_B.DataTypeConversion2 = (MCU_B.donotdeletethisgain >
    MCU_P.Constant_Value_g);

  /* DiscreteIntegrator: '<S23>/Coulomb Counter' incorporates:
   *  Memory: '<S23>/Initial it'
   */
  if (MCU_DW.CoulombCounter_IC_LOADING != 0) {
    MCU_DW.CoulombCounter_DSTATE = MCU_DW.Initialit_PreviousInput;
    if (MCU_DW.CoulombCounter_DSTATE > MCU_P.CoulombCounter_UpperSat) {
      MCU_DW.CoulombCounter_DSTATE = MCU_P.CoulombCounter_UpperSat;
    } else if (MCU_DW.CoulombCounter_DSTATE < MCU_P.CoulombCounter_LowerSat) {
      MCU_DW.CoulombCounter_DSTATE = MCU_P.CoulombCounter_LowerSat;
    }
  }

  if ((MCU_B.DataTypeConversion2 > 0.0) && (MCU_DW.CoulombCounter_PrevResetState
       <= 0)) {
    MCU_DW.CoulombCounter_DSTATE = MCU_DW.Initialit_PreviousInput;
    if (MCU_DW.CoulombCounter_DSTATE > MCU_P.CoulombCounter_UpperSat) {
      MCU_DW.CoulombCounter_DSTATE = MCU_P.CoulombCounter_UpperSat;
    } else if (MCU_DW.CoulombCounter_DSTATE < MCU_P.CoulombCounter_LowerSat) {
      MCU_DW.CoulombCounter_DSTATE = MCU_P.CoulombCounter_LowerSat;
    }
  }

  /* Gain: '<S23>/Gain1' incorporates:
   *  DiscreteIntegrator: '<S23>/Coulomb Counter'
   */
  rtb_Gain4_k = MCU_P.Gain1_Gain * MCU_DW.CoulombCounter_DSTATE;

  /* RelationalOperator: '<S24>/Compare' incorporates:
   *  Constant: '<S24>/Constant'
   */
  rtb_Halleffectsignalh_c = (MCU_B.donotdeletethisgain < MCU_P.Constant_Value);

  /* MATLAB Function: '<S23>/MATLAB Function' incorporates:
   *  Constant: '<S23>/Constant1'
   *  Constant: '<S23>/capacity'
   */
  /* MATLAB Function 'Battery/Model/Discrete/MATLAB Function': '<S27>:1' */
  /* '<S27>:1:3' */
  rtb_Gain4_g = 0.9999 * MCU_P.capacity_Value * 0.99999999999999978;
  rtb_Gain4 = fmin(fmax(rtb_Gain4_k, 0.0), rtb_Gain4_g);
  if (rtb_Gain4_g <= rtb_Gain4) {
    rtb_Gain4 = rtb_Gain4_g;
  }

  if (rtb_Halleffectsignalh_c) {
    rtb_Gain4_k = fmin(fmax(rtb_Gain4_k, -0.9999 * MCU_P.capacity_Value *
      0.99999999999999978 * MCU_P.Constant1_Value_f), rtb_Gain4_g);
  } else {
    rtb_Gain4_k = rtb_Gain4;
  }

  /* Gain: '<S28>/Gain1' incorporates:
   *  Constant: '<S23>/capacity'
   *  Fcn: '<S28>/Fcn'
   *  MATLAB Function: '<S23>/MATLAB Function'
   */
  /* '<S27>:1:3' */
  rtb_Gain4_g = (MCU_P.capacity_Value - rtb_Gain4) / MCU_P.capacity_Value *
    MCU_P.Gain1_Gain_c;

  /* Saturate: '<S28>/Saturation' */
  if (rtb_Gain4_g > MCU_P.Saturation_UpperSat_d) {
    rtb_Gain4_g = MCU_P.Saturation_UpperSat_d;
  } else if (rtb_Gain4_g < MCU_P.Saturation_LowerSat_e) {
    rtb_Gain4_g = MCU_P.Saturation_LowerSat_e;
  }

  /* DataTypeConversion: '<Root>/Data Type Conversion' incorporates:
   *  Saturate: '<S28>/Saturation'
   */
  rtb_CastToDouble = floor(rtb_Gain4_g);
  if (rtIsNaN(rtb_CastToDouble) || rtIsInf(rtb_CastToDouble)) {
    rtb_CastToDouble = 0.0;
  } else {
    rtb_CastToDouble = fmod(rtb_CastToDouble, 256.0);
  }

  if (rtb_CastToDouble < 0.0) {
    /* DataTypeConversion: '<Root>/Data Type Conversion' */
    MCU_B.DataTypeConversion = (uint8_T)-(int8_T)(uint8_T)-rtb_CastToDouble;
  } else {
    /* DataTypeConversion: '<Root>/Data Type Conversion' */
    MCU_B.DataTypeConversion = (uint8_T)rtb_CastToDouble;
  }

  /* End of DataTypeConversion: '<Root>/Data Type Conversion' */
  if (tmp) {
    /* Outputs for Atomic SubSystem: '<Root>/Subsystem1' */
    /* SignalConversion generated from: '<S6>/In1' */
    MCU_B.In1 = MCU_B.DataTypeConversion;

    /* End of Outputs for SubSystem: '<Root>/Subsystem1' */

    /* S-Function (sysvarout): '<S11>/S-Function' */
    if (cnSetSystemVariableValues(MCU_DW.SFunction_SysVar_f, 1, &MCU_B.In1, 3)
        != 0) {
      rtmSetErrorStatus(MCU_M, cnGetErrorMessage());
    }

    /* Outputs for Atomic SubSystem: '<Root>/Subsystem2' */
    MCU_Subsystem(MCU_B.donotdeletethisgain, &MCU_B.Subsystem2);

    /* End of Outputs for SubSystem: '<Root>/Subsystem2' */

    /* S-Function (sysvarout): '<S13>/S-Function' */
    if (cnSetSystemVariableValues(MCU_DW.SFunction_SysVar_m, 1,
         &MCU_B.Subsystem2.In1, 0) != 0) {
      rtmSetErrorStatus(MCU_M, cnGetErrorMessage());
    }

    /* Outputs for Atomic SubSystem: '<Root>/Subsystem3' */
    MCU_Subsystem(MCU_B.VoltageV, &MCU_B.Subsystem3);

    /* End of Outputs for SubSystem: '<Root>/Subsystem3' */

    /* S-Function (sysvarout): '<S14>/S-Function' */
    if (cnSetSystemVariableValues(MCU_DW.SFunction_SysVar_kc, 1,
         &MCU_B.Subsystem3.In1, 0) != 0) {
      rtmSetErrorStatus(MCU_M, cnGetErrorMessage());
    }
  }

  /* DiscreteTransferFcn: '<S23>/Current Filter' */
  MCU_B.CurrentFilter = MCU_P.CurrentFilter_NumCoef *
    MCU_DW.CurrentFilter_states;

  /* MATLAB Function: '<S30>/MATLAB Function' incorporates:
   *  Constant: '<S23>/capacity'
   *  Constant: '<S23>/constantA'
   *  Constant: '<S23>/constantB'
   *  Constant: '<S23>/constantK'
   */
  /* MATLAB Function 'Battery/Model/Discrete/Voltage Loss Model/Lithium Ion/MATLAB Function': '<S31>:1' */
  /* '<S31>:1:3' */
  if (rtb_Halleffectsignalh_c) {
    if (rtb_Gain4_k < 0.0) {
      rtb_CastToDouble = 0.0;
    } else {
      rtb_CastToDouble = rtb_Gain4_k;
    }

    rtb_Gain4_k = (MCU_P.capacity_Value / (0.1 * MCU_P.capacity_Value +
      rtb_Gain4_k) * -MCU_P.constantK_Value * MCU_B.CurrentFilter -
                   MCU_P.capacity_Value / (MCU_P.capacity_Value - rtb_Gain4_k) *
                   MCU_P.constantK_Value * rtb_Gain4_k) + exp
      (-MCU_P.constantB_Value * rtb_CastToDouble) * MCU_P.constantA_Value;
  } else {
    rtb_Gain4_k = MCU_P.capacity_Value / (MCU_P.capacity_Value - rtb_Gain4_k) *
      -MCU_P.constantK_Value * (rtb_Gain4_k + MCU_B.CurrentFilter) + exp
      (-MCU_P.constantB_Value * rtb_Gain4_k) * MCU_P.constantA_Value;
  }

  /* End of MATLAB Function: '<S30>/MATLAB Function' */

  /* Sum: '<S23>/Add' incorporates:
   *  Constant: '<S23>/Constant'
   *  Gain: '<S23>/Gain'
   */
  MCU_B.Add_c = (rtb_Gain4_k - MCU_P.Gain_Gain_e * MCU_B.donotdeletethisgain) +
    MCU_P.Constant_Value_f;

  /* Gain: '<S26>/Gain' incorporates:
   *  Abs: '<S26>/Abs'
   */
  rtb_Gain_m = MCU_P.Gain_Gain_k * fabs(MCU_B.donotdeletethisgain);

  /* Sum: '<S26>/Add1' incorporates:
   *  DataTypeConversion: '<S26>/Cast To Double'
   *  DiscreteIntegrator: '<S26>/Discrete-Time Integrator'
   *  Gain: '<S26>/Gain1'
   *  Product: '<S26>/Product'
   *  Product: '<S26>/Product1'
   */
  MCU_B.Add1 = MCU_P.Gain1_Gain_g * (real_T)rtb_Halleffectsignalh_c * rtb_Gain_m
    - MCU_DW.DiscreteTimeIntegrator_DSTATE_h * rtb_Gain_m;

  /* Gain: '<S23>/Gain2' incorporates:
   *  MATLAB Function: '<S23>/MATLAB Function'
   */
  MCU_B.Gain2_a = MCU_P.Gain2_Gain_i * rtb_Gain4;
}

/* Model update function */
static void MCU_update(void)
{
  /* Update for S-Function (anorsimstepinitshmem): '<S4>/S-Function' */
  /* Level2 S-Function Block: '<S4>/S-Function' (anorsimstepinitshmem) */
  {
    SimStruct *rts = MCU_M->childSfunctions[0];
    sfcnUpdate(rts,0);
    if (ssGetErrorStatus(rts) != (NULL))
      return;
  }

  /* Update for UnitDelay: '<S23>/Unit Delay' */
  MCU_DW.UnitDelay_DSTATE = MCU_B.Add_c;

  /* Update for UnitDelay: '<S38>/Unit Delay' */
  MCU_DW.UnitDelay_DSTATE_n[0] = MCU_B.Add[0];
  MCU_DW.UnitDelay_DSTATE_n[1] = MCU_B.Add[1];

  /* Update for DiscreteIntegrator: '<S34>/Discrete-Time Integrator1' */
  MCU_DW.DiscreteTimeIntegrator1_DSTATE += MCU_P.DiscreteTimeIntegrator1_gainval
    * MCU_B.Gain;

  /* Update for DiscreteIntegrator: '<S34>/Discrete-Time Integrator' */
  MCU_DW.DiscreteTimeIntegrator_DSTATE += MCU_P.DiscreteTimeIntegrator_gainval *
    MCU_B.Gain2_i;

  /* Update for UnitDelay: '<S48>/Unit Delay' incorporates:
   *  Selector: '<S32>/first 2 columns of D'
   */
  MCU_DW.UnitDelay_DSTATE_g[0] = MCU_B.first2columnsofD[0];
  MCU_DW.UnitDelay_DSTATE_g[1] = MCU_B.first2columnsofD[1];
  MCU_DW.UnitDelay_DSTATE_g[2] = MCU_B.first2columnsofD[2];
  MCU_DW.UnitDelay_DSTATE_g[3] = MCU_B.first2columnsofD[3];

  /* Update for S-Function (sfun_spssw_discc_DSS): '<S48>/State-Space' */
  /* Level2 S-Function Block: '<S48>/State-Space' (sfun_spssw_discc_DSS) */
  {
    SimStruct *rts = MCU_M->childSfunctions[1];
    sfcnUpdate(rts,0);
    if (ssGetErrorStatus(rts) != (NULL))
      return;
  }

  /* Update for Atomic SubSystem: '<Root>/speed controller' */
  /* Update for DiscreteIntegrator: '<S90>/Integrator' */
  MCU_DW.Integrator_DSTATE += MCU_P.Integrator_gainval * MCU_B.IntegralGain;

  /* End of Update for SubSystem: '<Root>/speed controller' */

  /* Update for Atomic SubSystem: '<Root>/current controller' */
  /* Update for Atomic SubSystem: '<S16>/Custom Relay' */
  MCU_CustomRelay_Update(&MCU_B.CustomRelay, &MCU_DW.CustomRelay);

  /* End of Update for SubSystem: '<S16>/Custom Relay' */

  /* Update for Atomic SubSystem: '<S16>/Custom Relay1' */
  MCU_CustomRelay_Update(&MCU_B.CustomRelay1, &MCU_DW.CustomRelay1);

  /* End of Update for SubSystem: '<S16>/Custom Relay1' */

  /* Update for Atomic SubSystem: '<S16>/Custom Relay2' */
  MCU_CustomRelay_Update(&MCU_B.CustomRelay2, &MCU_DW.CustomRelay2);

  /* End of Update for SubSystem: '<S16>/Custom Relay2' */
  /* End of Update for SubSystem: '<Root>/current controller' */

  /* Update for Memory: '<S23>/Initial it' */
  MCU_DW.Initialit_PreviousInput = MCU_B.Gain2_a;

  /* Update for DiscreteIntegrator: '<S23>/Coulomb Counter' */
  MCU_DW.CoulombCounter_IC_LOADING = 0U;
  MCU_DW.CoulombCounter_DSTATE += MCU_P.CoulombCounter_gainval *
    MCU_B.CurrentFilter;
  if (MCU_DW.CoulombCounter_DSTATE > MCU_P.CoulombCounter_UpperSat) {
    MCU_DW.CoulombCounter_DSTATE = MCU_P.CoulombCounter_UpperSat;
  } else if (MCU_DW.CoulombCounter_DSTATE < MCU_P.CoulombCounter_LowerSat) {
    MCU_DW.CoulombCounter_DSTATE = MCU_P.CoulombCounter_LowerSat;
  }

  if (MCU_B.DataTypeConversion2 > 0.0) {
    MCU_DW.CoulombCounter_PrevResetState = 1;
  } else if (MCU_B.DataTypeConversion2 < 0.0) {
    MCU_DW.CoulombCounter_PrevResetState = -1;
  } else if (MCU_B.DataTypeConversion2 == 0.0) {
    MCU_DW.CoulombCounter_PrevResetState = 0;
  } else {
    MCU_DW.CoulombCounter_PrevResetState = 2;
  }

  /* End of Update for DiscreteIntegrator: '<S23>/Coulomb Counter' */

  /* Update for DiscreteIntegrator: '<S26>/Discrete-Time Integrator' */
  MCU_DW.DiscreteTimeIntegrator_DSTATE_h +=
    MCU_P.DiscreteTimeIntegrator_gainva_b * MCU_B.Add1;

  /* Update for DiscreteTransferFcn: '<S23>/Current Filter' */
  MCU_DW.CurrentFilter_states = (MCU_B.donotdeletethisgain -
    MCU_P.CurrentFilter_DenCoef[1] * MCU_DW.CurrentFilter_states) /
    MCU_P.CurrentFilter_DenCoef[0];

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   * Timer of this task consists of two 32 bit unsigned integers.
   * The two integers represent the low bits Timing.clockTick0 and the high bits
   * Timing.clockTickH0. When the low bit overflows to 0, the high bits increment.
   */
  if (!(++MCU_M->Timing.clockTick0)) {
    ++MCU_M->Timing.clockTickH0;
  }

  MCU_M->Timing.t[0] = MCU_M->Timing.clockTick0 * MCU_M->Timing.stepSize0 +
    MCU_M->Timing.clockTickH0 * MCU_M->Timing.stepSize0 * 4294967296.0;
  if (MCU_M->Timing.TaskCounters.TID[1] == 0) {
    /* Update absolute timer for sample time: [0.01s, 0.0s] */
    /* The "clockTick1" counts the number of times the code of this task has
     * been executed. The absolute time is the multiplication of "clockTick1"
     * and "Timing.stepSize1". Size of "clockTick1" ensures timer will not
     * overflow during the application lifespan selected.
     * Timer of this task consists of two 32 bit unsigned integers.
     * The two integers represent the low bits Timing.clockTick1 and the high bits
     * Timing.clockTickH1. When the low bit overflows to 0, the high bits increment.
     */
    if (!(++MCU_M->Timing.clockTick1)) {
      ++MCU_M->Timing.clockTickH1;
    }

    MCU_M->Timing.t[1] = MCU_M->Timing.clockTick1 * MCU_M->Timing.stepSize1 +
      MCU_M->Timing.clockTickH1 * MCU_M->Timing.stepSize1 * 4294967296.0;
  }

  rate_scheduler();
}

/* Model initialize function */
static void MCU_initialize(void)
{
  /* Start for S-Function (anorsimstepinitshmem): '<S4>/S-Function' */
  /* Level2 S-Function Block: '<S4>/S-Function' (anorsimstepinitshmem) */
  {
    SimStruct *rts = MCU_M->childSfunctions[0];
    sfcnStart(rts);
    if (ssGetErrorStatus(rts) != (NULL))
      return;
  }

  /* Start for S-Function (sfun_spssw_discc_DSS): '<S48>/State-Space' */
  /* Level2 S-Function Block: '<S48>/State-Space' (sfun_spssw_discc_DSS) */
  {
    SimStruct *rts = MCU_M->childSfunctions[1];
    sfcnStart(rts);
    if (ssGetErrorStatus(rts) != (NULL))
      return;
  }

  {
    real_T Initialit_PreviousInput_tmp;

    /* InitializeConditions for UnitDelay: '<S23>/Unit Delay' */
    MCU_DW.UnitDelay_DSTATE = MCU_P.UnitDelay_InitialCondition;

    /* InitializeConditions for UnitDelay: '<S38>/Unit Delay' */
    MCU_DW.UnitDelay_DSTATE_n[0] = MCU_P.UnitDelay_InitialCondition_k[0];
    MCU_DW.UnitDelay_DSTATE_n[1] = MCU_P.UnitDelay_InitialCondition_k[1];

    /* InitializeConditions for DiscreteIntegrator: '<S34>/Discrete-Time Integrator1' */
    MCU_DW.DiscreteTimeIntegrator1_DSTATE = MCU_P.DiscreteTimeIntegrator1_IC;

    /* InitializeConditions for DiscreteIntegrator: '<S34>/Discrete-Time Integrator' */
    MCU_DW.DiscreteTimeIntegrator_DSTATE = MCU_P.DiscreteTimeIntegrator_IC;

    /* InitializeConditions for UnitDelay: '<S48>/Unit Delay' */
    MCU_DW.UnitDelay_DSTATE_g[0] = MCU_P.UnitDelay_InitialCondition_o;
    MCU_DW.UnitDelay_DSTATE_g[1] = MCU_P.UnitDelay_InitialCondition_o;
    MCU_DW.UnitDelay_DSTATE_g[2] = MCU_P.UnitDelay_InitialCondition_o;
    MCU_DW.UnitDelay_DSTATE_g[3] = MCU_P.UnitDelay_InitialCondition_o;

    /* InitializeConditions for S-Function (sfun_spssw_discc_DSS): '<S48>/State-Space' */
    /* Level2 S-Function Block: '<S48>/State-Space' (sfun_spssw_discc_DSS) */
    {
      SimStruct *rts = MCU_M->childSfunctions[1];
      sfcnInitializeConditions(rts);
      if (ssGetErrorStatus(rts) != (NULL))
        return;
    }

    /* InitializeConditions for Memory: '<S23>/Initial it' incorporates:
     *  DiscreteIntegrator: '<S26>/Discrete-Time Integrator'
     */
    Initialit_PreviousInput_tmp = 1.0 - MCU_P.Battery_SOC / 100.0;
    MCU_DW.Initialit_PreviousInput = Initialit_PreviousInput_tmp *
      93.239999999999071 * 0.9009009009009098 * 3600.0;

    /* InitializeConditions for DiscreteIntegrator: '<S23>/Coulomb Counter' */
    MCU_DW.CoulombCounter_PrevResetState = 2;
    MCU_DW.CoulombCounter_IC_LOADING = 1U;

    /* InitializeConditions for DiscreteIntegrator: '<S26>/Discrete-Time Integrator' */
    MCU_DW.DiscreteTimeIntegrator_DSTATE_h = exp(Initialit_PreviousInput_tmp *
      -0.95409408130916984 * 93.239999999999071) * 0.83744872582453345;

    /* InitializeConditions for DiscreteTransferFcn: '<S23>/Current Filter' */
    MCU_DW.CurrentFilter_states = MCU_P.CurrentFilter_InitialStates;

    /* SystemInitialize for Atomic SubSystem: '<Root>/speed controller' */
    /* InitializeConditions for DiscreteIntegrator: '<S90>/Integrator' */
    MCU_DW.Integrator_DSTATE = MCU_P.DiscretePIDController_InitialCo;

    /* End of SystemInitialize for SubSystem: '<Root>/speed controller' */

    /* SystemInitialize for Atomic SubSystem: '<Root>/current controller' */
    /* SystemInitialize for Atomic SubSystem: '<S16>/Custom Relay' */
    MCU_CustomRelay_Init(&MCU_DW.CustomRelay, &MCU_P.CustomRelay);

    /* End of SystemInitialize for SubSystem: '<S16>/Custom Relay' */

    /* SystemInitialize for Atomic SubSystem: '<S16>/Custom Relay1' */
    MCU_CustomRelay_Init(&MCU_DW.CustomRelay1, &MCU_P.CustomRelay1);

    /* End of SystemInitialize for SubSystem: '<S16>/Custom Relay1' */

    /* SystemInitialize for Atomic SubSystem: '<S16>/Custom Relay2' */
    MCU_CustomRelay_Init(&MCU_DW.CustomRelay2, &MCU_P.CustomRelay2);

    /* End of SystemInitialize for SubSystem: '<S16>/Custom Relay2' */
    /* End of SystemInitialize for SubSystem: '<Root>/current controller' */
  }
}

/* Model terminate function */
static void MCU_terminate(void)
{
  /* Terminate for S-Function (anorsimstepinitshmem): '<S4>/S-Function' */
  /* Level2 S-Function Block: '<S4>/S-Function' (anorsimstepinitshmem) */
  {
    SimStruct *rts = MCU_M->childSfunctions[0];
    sfcnTerminate(rts);
  }

  /* Terminate for S-Function (sfun_spssw_discc_DSS): '<S48>/State-Space' */
  /* Level2 S-Function Block: '<S48>/State-Space' (sfun_spssw_discc_DSS) */
  {
    SimStruct *rts = MCU_M->childSfunctions[1];
    sfcnTerminate(rts);
  }

  /* Terminate for S-Function (sysvarout): '<S12>/S-Function' */
  if (NULL != MCU_DW.SFunction_SysVar) {
    cnReleaseSystemVariable(MCU_DW.SFunction_SysVar);
    MCU_DW.SFunction_SysVar = NULL;
  }

  /* Terminate for S-Function (sysvarin): '<S9>/S-Function' */
  if (NULL != MCU_DW.SFunction_SysVar_k) {
    cnReleaseSystemVariable(MCU_DW.SFunction_SysVar_k);
    MCU_DW.SFunction_SysVar_k = NULL;
  }

  /* Terminate for S-Function (sysvarin): '<S10>/S-Function' */
  if (NULL != MCU_DW.SFunction_SysVar_a) {
    cnReleaseSystemVariable(MCU_DW.SFunction_SysVar_a);
    MCU_DW.SFunction_SysVar_a = NULL;
  }

  /* Terminate for S-Function (sysvarout): '<S11>/S-Function' */
  if (NULL != MCU_DW.SFunction_SysVar_f) {
    cnReleaseSystemVariable(MCU_DW.SFunction_SysVar_f);
    MCU_DW.SFunction_SysVar_f = NULL;
  }

  /* Terminate for S-Function (sysvarout): '<S13>/S-Function' */
  if (NULL != MCU_DW.SFunction_SysVar_m) {
    cnReleaseSystemVariable(MCU_DW.SFunction_SysVar_m);
    MCU_DW.SFunction_SysVar_m = NULL;
  }

  /* Terminate for S-Function (sysvarout): '<S14>/S-Function' */
  if (NULL != MCU_DW.SFunction_SysVar_kc) {
    cnReleaseSystemVariable(MCU_DW.SFunction_SysVar_kc);
    MCU_DW.SFunction_SysVar_kc = NULL;
  }
}

/*========================================================================*
 * Start of Classic call interface                                        *
 *========================================================================*/
void MdlOutputs(int_T tid)
{
  MCU_output();
  UNUSED_PARAMETER(tid);
}

void MdlUpdate(int_T tid)
{
  MCU_update();
  UNUSED_PARAMETER(tid);
}

void MdlInitializeSizes(void)
{
}

void MdlInitializeSampleTimes(void)
{
}

void MdlInitialize(void)
{
}

void MdlStart(void)
{
  MCU_initialize();
}

void MdlTerminate(void)
{
  MCU_terminate();
}

/* Registration function */
RT_MODEL_MCU_T *MCU(void)
{
  /* Registration code */

  /* initialize real-time model */
  (void) memset((void *)MCU_M, 0,
                sizeof(RT_MODEL_MCU_T));
  rtsiSetSolverName(&MCU_M->solverInfo,"FixedStepDiscrete");
  MCU_M->solverInfoPtr = (&MCU_M->solverInfo);

  /* Initialize timing info */
  {
    int_T *mdlTsMap = MCU_M->Timing.sampleTimeTaskIDArray;
    mdlTsMap[0] = 0;
    mdlTsMap[1] = 1;
    MCU_M->Timing.sampleTimeTaskIDPtr = (&mdlTsMap[0]);
    MCU_M->Timing.sampleTimes = (&MCU_M->Timing.sampleTimesArray[0]);
    MCU_M->Timing.offsetTimes = (&MCU_M->Timing.offsetTimesArray[0]);

    /* task periods */
    MCU_M->Timing.sampleTimes[0] = (5.0E-6);
    MCU_M->Timing.sampleTimes[1] = (0.01);

    /* task offsets */
    MCU_M->Timing.offsetTimes[0] = (0.0);
    MCU_M->Timing.offsetTimes[1] = (0.0);
  }

  rtmSetTPtr(MCU_M, &MCU_M->Timing.tArray[0]);

  {
    int_T *mdlSampleHits = MCU_M->Timing.sampleHitArray;
    mdlSampleHits[0] = 1;
    mdlSampleHits[1] = 1;
    MCU_M->Timing.sampleHits = (&mdlSampleHits[0]);
  }

  rtmSetTFinal(MCU_M, 5.0);
  MCU_M->Timing.stepSize0 = 5.0E-6;
  MCU_M->Timing.stepSize1 = 0.01;

  /* Setup for data logging */
  {
    static RTWLogInfo rt_DataLoggingInfo;
    rt_DataLoggingInfo.loggingInterval = (NULL);
    MCU_M->rtwLogInfo = &rt_DataLoggingInfo;
  }

  /* Setup for data logging */
  {
    rtliSetLogXSignalInfo(MCU_M->rtwLogInfo, (NULL));
    rtliSetLogXSignalPtrs(MCU_M->rtwLogInfo, (NULL));
    rtliSetLogT(MCU_M->rtwLogInfo, "tout");
    rtliSetLogX(MCU_M->rtwLogInfo, "");
    rtliSetLogXFinal(MCU_M->rtwLogInfo, "");
    rtliSetLogVarNameModifier(MCU_M->rtwLogInfo, "rt_");
    rtliSetLogFormat(MCU_M->rtwLogInfo, 0);
    rtliSetLogMaxRows(MCU_M->rtwLogInfo, 1000);
    rtliSetLogDecimation(MCU_M->rtwLogInfo, 1);
    rtliSetLogY(MCU_M->rtwLogInfo, "");
    rtliSetLogYSignalInfo(MCU_M->rtwLogInfo, (NULL));
    rtliSetLogYSignalPtrs(MCU_M->rtwLogInfo, (NULL));
  }

  MCU_M->solverInfoPtr = (&MCU_M->solverInfo);
  MCU_M->Timing.stepSize = (5.0E-6);
  rtsiSetFixedStepSize(&MCU_M->solverInfo, 5.0E-6);
  rtsiSetSolverMode(&MCU_M->solverInfo, SOLVER_MODE_SINGLETASKING);

  /* block I/O */
  MCU_M->blockIO = ((void *) &MCU_B);
  (void) memset(((void *) &MCU_B), 0,
                sizeof(B_MCU_T));

  /* parameters */
  MCU_M->defaultParam = ((real_T *)&MCU_P);

  /* states (dwork) */
  MCU_M->dwork = ((void *) &MCU_DW);
  (void) memset((void *)&MCU_DW, 0,
                sizeof(DW_MCU_T));

  /* Initialize DataMapInfo substructure containing ModelMap for C API */
  MCU_InitializeDataMapInfo();

  /* child S-Function registration */
  {
    RTWSfcnInfo *sfcnInfo = &MCU_M->NonInlinedSFcns.sfcnInfo;
    MCU_M->sfcnInfo = (sfcnInfo);
    rtssSetErrorStatusPtr(sfcnInfo, (&rtmGetErrorStatus(MCU_M)));
    MCU_M->Sizes.numSampTimes = (2);
    rtssSetNumRootSampTimesPtr(sfcnInfo, &MCU_M->Sizes.numSampTimes);
    MCU_M->NonInlinedSFcns.taskTimePtrs[0] = (&rtmGetTPtr(MCU_M)[0]);
    MCU_M->NonInlinedSFcns.taskTimePtrs[1] = (&rtmGetTPtr(MCU_M)[1]);
    rtssSetTPtrPtr(sfcnInfo,MCU_M->NonInlinedSFcns.taskTimePtrs);
    rtssSetTStartPtr(sfcnInfo, &rtmGetTStart(MCU_M));
    rtssSetTFinalPtr(sfcnInfo, &rtmGetTFinal(MCU_M));
    rtssSetTimeOfLastOutputPtr(sfcnInfo, &rtmGetTimeOfLastOutput(MCU_M));
    rtssSetStepSizePtr(sfcnInfo, &MCU_M->Timing.stepSize);
    rtssSetStopRequestedPtr(sfcnInfo, &rtmGetStopRequested(MCU_M));
    rtssSetDerivCacheNeedsResetPtr(sfcnInfo, &MCU_M->derivCacheNeedsReset);
    rtssSetZCCacheNeedsResetPtr(sfcnInfo, &MCU_M->zCCacheNeedsReset);
    rtssSetContTimeOutputInconsistentWithStateAtMajorStepPtr(sfcnInfo,
      &MCU_M->CTOutputIncnstWithState);
    rtssSetSampleHitsPtr(sfcnInfo, &MCU_M->Timing.sampleHits);
    rtssSetPerTaskSampleHitsPtr(sfcnInfo, &MCU_M->Timing.perTaskSampleHits);
    rtssSetSimModePtr(sfcnInfo, &MCU_M->simMode);
    rtssSetSolverInfoPtr(sfcnInfo, &MCU_M->solverInfoPtr);
  }

  MCU_M->Sizes.numSFcns = (2);

  /* register each child */
  {
    (void) memset((void *)&MCU_M->NonInlinedSFcns.childSFunctions[0], 0,
                  2*sizeof(SimStruct));
    MCU_M->childSfunctions = (&MCU_M->NonInlinedSFcns.childSFunctionPtrs[0]);
    MCU_M->childSfunctions[0] = (&MCU_M->NonInlinedSFcns.childSFunctions[0]);
    MCU_M->childSfunctions[1] = (&MCU_M->NonInlinedSFcns.childSFunctions[1]);

    /* Level2 S-Function Block: MCU/<S4>/S-Function (anorsimstepinitshmem) */
    {
      SimStruct *rts = MCU_M->childSfunctions[0];

      /* timing info */
      time_T *sfcnPeriod = MCU_M->NonInlinedSFcns.Sfcn0.sfcnPeriod;
      time_T *sfcnOffset = MCU_M->NonInlinedSFcns.Sfcn0.sfcnOffset;
      int_T *sfcnTsMap = MCU_M->NonInlinedSFcns.Sfcn0.sfcnTsMap;
      (void) memset((void*)sfcnPeriod, 0,
                    sizeof(time_T)*1);
      (void) memset((void*)sfcnOffset, 0,
                    sizeof(time_T)*1);
      ssSetSampleTimePtr(rts, &sfcnPeriod[0]);
      ssSetOffsetTimePtr(rts, &sfcnOffset[0]);
      ssSetSampleTimeTaskIDPtr(rts, sfcnTsMap);

      {
        ssSetBlkInfo2Ptr(rts, &MCU_M->NonInlinedSFcns.blkInfo2[0]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &MCU_M->NonInlinedSFcns.inputOutputPortInfo2[0]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, MCU_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts, &MCU_M->NonInlinedSFcns.methods2[0]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts, &MCU_M->NonInlinedSFcns.methods3[0]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts, &MCU_M->NonInlinedSFcns.methods4[0]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts, &MCU_M->NonInlinedSFcns.statesInfo2[0]);
        ssSetPeriodicStatesInfo(rts, &MCU_M->NonInlinedSFcns.periodicStatesInfo
          [0]);
      }

      /* outputs */
      {
        ssSetPortInfoForOutputs(rts,
          &MCU_M->NonInlinedSFcns.Sfcn0.outputPortInfo[0]);
        ssSetPortInfoForOutputs(rts,
          &MCU_M->NonInlinedSFcns.Sfcn0.outputPortInfo[0]);
        _ssSetNumOutputPorts(rts, 1);
        _ssSetPortInfo2ForOutputUnits(rts,
          &MCU_M->NonInlinedSFcns.Sfcn0.outputPortUnits[0]);
        ssSetOutputPortUnit(rts, 0, 0);
        _ssSetPortInfo2ForOutputCoSimAttribute(rts,
          &MCU_M->NonInlinedSFcns.Sfcn0.outputPortCoSimAttribute[0]);
        ssSetOutputPortIsContinuousQuantity(rts, 0, 0);

        /* port 0 */
        {
          _ssSetOutputPortNumDimensions(rts, 0, 1);
          ssSetOutputPortWidthAsInt(rts, 0, 1);
          ssSetOutputPortSignal(rts, 0, ((real_T *) &MCU_B.SFunction));
        }
      }

      /* path info */
      ssSetModelName(rts, "S-Function");
      ssSetPath(rts, "MCU/Simulation Step/S-Function");
      ssSetRTModel(rts,MCU_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **) &MCU_M->NonInlinedSFcns.Sfcn0.params;
        ssSetSFcnParamsCount(rts, 1);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)MCU_P.SFunction_P1_Size);
      }

      /* registration */
      anorsimstepinitshmem(rts);
      sfcnInitializeSizes(rts);

      /* adjust sample time */
      ssSetSampleTime(rts, 0, 5.0E-6);
      ssSetOffsetTime(rts, 0, 0.0);
      sfcnTsMap[0] = 0;

      /* set compiled values of dynamic vector attributes */
      ssSetNumNonsampledZCsAsInt(rts, 0);

      /* Update connectivity flags for each port */
      _ssSetOutputPortConnected(rts, 0, 0);
      _ssSetOutputPortBeingMerged(rts, 0, 0);

      /* Update the BufferDstPort flags for each input port */
      /* Update port-based sample time attributes */
      _ssSetOutputPortSampleTimeIndex(rts, 0, 0);
      ssSetOutputPortSampleTime(rts, 0, 5.0E-6);
      ssSetOutputPortOffsetTime(rts, 0, 0.0);
      sfcnInitializeSampleTimes(rts);
    }

    /* Level2 S-Function Block: MCU/<S48>/State-Space (sfun_spssw_discc_DSS) */
    {
      SimStruct *rts = MCU_M->childSfunctions[1];

      /* timing info */
      time_T *sfcnPeriod = MCU_M->NonInlinedSFcns.Sfcn1.sfcnPeriod;
      time_T *sfcnOffset = MCU_M->NonInlinedSFcns.Sfcn1.sfcnOffset;
      int_T *sfcnTsMap = MCU_M->NonInlinedSFcns.Sfcn1.sfcnTsMap;
      (void) memset((void*)sfcnPeriod, 0,
                    sizeof(time_T)*1);
      (void) memset((void*)sfcnOffset, 0,
                    sizeof(time_T)*1);
      ssSetSampleTimePtr(rts, &sfcnPeriod[0]);
      ssSetOffsetTimePtr(rts, &sfcnOffset[0]);
      ssSetSampleTimeTaskIDPtr(rts, sfcnTsMap);

      {
        ssSetBlkInfo2Ptr(rts, &MCU_M->NonInlinedSFcns.blkInfo2[1]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &MCU_M->NonInlinedSFcns.inputOutputPortInfo2[1]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, MCU_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts, &MCU_M->NonInlinedSFcns.methods2[1]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts, &MCU_M->NonInlinedSFcns.methods3[1]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts, &MCU_M->NonInlinedSFcns.methods4[1]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts, &MCU_M->NonInlinedSFcns.statesInfo2[1]);
        ssSetPeriodicStatesInfo(rts, &MCU_M->NonInlinedSFcns.periodicStatesInfo
          [1]);
      }

      /* inputs */
      {
        _ssSetNumInputPorts(rts, 3);
        ssSetPortInfoForInputs(rts, &MCU_M->NonInlinedSFcns.Sfcn1.inputPortInfo
          [0]);
        ssSetPortInfoForInputs(rts, &MCU_M->NonInlinedSFcns.Sfcn1.inputPortInfo
          [0]);
        _ssSetPortInfo2ForInputUnits(rts,
          &MCU_M->NonInlinedSFcns.Sfcn1.inputPortUnits[0]);
        ssSetInputPortUnit(rts, 0, 0);
        ssSetInputPortUnit(rts, 1, 0);
        ssSetInputPortUnit(rts, 2, 0);
        _ssSetPortInfo2ForInputCoSimAttribute(rts,
          &MCU_M->NonInlinedSFcns.Sfcn1.inputPortCoSimAttribute[0]);
        ssSetInputPortIsContinuousQuantity(rts, 0, 0);
        ssSetInputPortIsContinuousQuantity(rts, 1, 0);
        ssSetInputPortIsContinuousQuantity(rts, 2, 0);

        /* port 0 */
        {
          real_T const **sfcnUPtrs = (real_T const **)
            &MCU_M->NonInlinedSFcns.Sfcn1.UPtrs0;

          {
            int_T i1;
            const real_T *u0 = &MCU_B.Constant[0];
            for (i1=0; i1 < 6; i1++) {
              sfcnUPtrs[i1] = &u0[i1];
            }

            sfcnUPtrs[6] = &MCU_B.VoltageV;
            sfcnUPtrs[7] = &MCU_B.Add2[0];
            sfcnUPtrs[8] = &MCU_B.Add2[1];
          }

          ssSetInputPortSignalPtrs(rts, 0, (InputPtrsType)&sfcnUPtrs[0]);
          _ssSetInputPortNumDimensions(rts, 0, 1);
          ssSetInputPortWidthAsInt(rts, 0, 9);
        }

        /* port 1 */
        {
          real_T const **sfcnUPtrs = (real_T const **)
            &MCU_M->NonInlinedSFcns.Sfcn1.UPtrs1;
          sfcnUPtrs[0] = &MCU_B.CustomRelay.IndexVector;
          sfcnUPtrs[1] = &MCU_B.DataTypeConversion1;
          sfcnUPtrs[2] = &MCU_B.CustomRelay1.IndexVector;
          sfcnUPtrs[3] = &MCU_B.DataTypeConversion3;
          sfcnUPtrs[4] = &MCU_B.CustomRelay2.IndexVector;
          sfcnUPtrs[5] = &MCU_B.DataTypeConversion5;
          ssSetInputPortSignalPtrs(rts, 1, (InputPtrsType)&sfcnUPtrs[0]);
          _ssSetInputPortNumDimensions(rts, 1, 1);
          ssSetInputPortWidthAsInt(rts, 1, 6);
        }

        /* port 2 */
        {
          real_T const **sfcnUPtrs = (real_T const **)
            &MCU_M->NonInlinedSFcns.Sfcn1.UPtrs2;
          sfcnUPtrs[0] = MCU_B.Sum;
          sfcnUPtrs[1] = &MCU_B.Sum[1];
          sfcnUPtrs[2] = &MCU_B.Sum[2];
          sfcnUPtrs[3] = &MCU_B.Sum[3];
          ssSetInputPortSignalPtrs(rts, 2, (InputPtrsType)&sfcnUPtrs[0]);
          _ssSetInputPortNumDimensions(rts, 2, 1);
          ssSetInputPortWidthAsInt(rts, 2, 4);
        }
      }

      /* outputs */
      {
        ssSetPortInfoForOutputs(rts,
          &MCU_M->NonInlinedSFcns.Sfcn1.outputPortInfo[0]);
        ssSetPortInfoForOutputs(rts,
          &MCU_M->NonInlinedSFcns.Sfcn1.outputPortInfo[0]);
        _ssSetNumOutputPorts(rts, 3);
        _ssSetPortInfo2ForOutputUnits(rts,
          &MCU_M->NonInlinedSFcns.Sfcn1.outputPortUnits[0]);
        ssSetOutputPortUnit(rts, 0, 0);
        ssSetOutputPortUnit(rts, 1, 0);
        ssSetOutputPortUnit(rts, 2, 0);
        _ssSetPortInfo2ForOutputCoSimAttribute(rts,
          &MCU_M->NonInlinedSFcns.Sfcn1.outputPortCoSimAttribute[0]);
        ssSetOutputPortIsContinuousQuantity(rts, 0, 0);
        ssSetOutputPortIsContinuousQuantity(rts, 1, 0);
        ssSetOutputPortIsContinuousQuantity(rts, 2, 0);

        /* port 0 */
        {
          _ssSetOutputPortNumDimensions(rts, 0, 1);
          ssSetOutputPortWidthAsInt(rts, 0, 9);
          ssSetOutputPortSignal(rts, 0, ((real_T *) MCU_B.StateSpace_o1));
        }

        /* port 1 */
        {
          _ssSetOutputPortNumDimensions(rts, 1, 1);
          ssSetOutputPortWidthAsInt(rts, 1, 6);
          ssSetOutputPortSignal(rts, 1, ((real_T *) MCU_B.StateSpace_o2));
        }

        /* port 2 */
        {
          _ssSetOutputPortNumDimensions(rts, 2, 1);
          ssSetOutputPortWidthAsInt(rts, 2, 1);
          ssSetOutputPortSignal(rts, 2, ((real_T *) &MCU_B.StateSpace_o3));
        }
      }

      /* states */
      ssSetDiscStates(rts, (real_T *) &MCU_DW.StateSpace_DSTATE);

      /* path info */
      ssSetModelName(rts, "State-Space");
      ssSetPath(rts, "MCU/powergui/EquivalentModel1/State-Space");
      ssSetRTModel(rts,MCU_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **) &MCU_M->NonInlinedSFcns.Sfcn1.params;
        ssSetSFcnParamsCount(rts, 24);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)MCU_P.StateSpace_P1_Size);
        ssSetSFcnParam(rts, 1, (mxArray*)MCU_P.StateSpace_P2_Size);
        ssSetSFcnParam(rts, 2, (mxArray*)MCU_P.StateSpace_P3_Size);
        ssSetSFcnParam(rts, 3, (mxArray*)MCU_P.StateSpace_P4_Size);
        ssSetSFcnParam(rts, 4, (mxArray*)MCU_P.StateSpace_P5_Size);
        ssSetSFcnParam(rts, 5, (mxArray*)MCU_P.StateSpace_P6_Size);
        ssSetSFcnParam(rts, 6, (mxArray*)MCU_P.StateSpace_P7_Size);
        ssSetSFcnParam(rts, 7, (mxArray*)MCU_P.StateSpace_P8_Size);
        ssSetSFcnParam(rts, 8, (mxArray*)MCU_P.StateSpace_P9_Size);
        ssSetSFcnParam(rts, 9, (mxArray*)MCU_P.StateSpace_P10_Size);
        ssSetSFcnParam(rts, 10, (mxArray*)MCU_P.StateSpace_P11_Size);
        ssSetSFcnParam(rts, 11, (mxArray*)MCU_P.StateSpace_P12_Size);
        ssSetSFcnParam(rts, 12, (mxArray*)MCU_P.StateSpace_P13_Size);
        ssSetSFcnParam(rts, 13, (mxArray*)MCU_P.StateSpace_P14_Size);
        ssSetSFcnParam(rts, 14, (mxArray*)MCU_P.StateSpace_P15_Size);
        ssSetSFcnParam(rts, 15, (mxArray*)MCU_P.StateSpace_P16_Size);
        ssSetSFcnParam(rts, 16, (mxArray*)MCU_P.StateSpace_P17_Size);
        ssSetSFcnParam(rts, 17, (mxArray*)MCU_P.StateSpace_P18_Size);
        ssSetSFcnParam(rts, 18, (mxArray*)MCU_P.StateSpace_P19_Size);
        ssSetSFcnParam(rts, 19, (mxArray*)MCU_P.StateSpace_P20_Size);
        ssSetSFcnParam(rts, 20, (mxArray*)MCU_P.StateSpace_P21_Size);
        ssSetSFcnParam(rts, 21, (mxArray*)MCU_P.StateSpace_P22_Size);
        ssSetSFcnParam(rts, 22, (mxArray*)MCU_P.StateSpace_P23_Size);
        ssSetSFcnParam(rts, 23, (mxArray*)MCU_P.StateSpace_P24_Size);
      }

      /* work vectors */
      ssSetRWork(rts, (real_T *) &MCU_DW.StateSpace_RWORK);
      ssSetIWork(rts, (int_T *) &MCU_DW.StateSpace_IWORK[0]);
      ssSetPWork(rts, (void **) &MCU_DW.StateSpace_PWORK[0]);

      {
        struct _ssDWorkRecord *dWorkRecord = (struct _ssDWorkRecord *)
          &MCU_M->NonInlinedSFcns.Sfcn1.dWork;
        struct _ssDWorkAuxRecord *dWorkAuxRecord = (struct _ssDWorkAuxRecord *)
          &MCU_M->NonInlinedSFcns.Sfcn1.dWorkAux;
        ssSetSFcnDWork(rts, dWorkRecord);
        ssSetSFcnDWorkAux(rts, dWorkAuxRecord);
        ssSetNumDWorkAsInt(rts, 4);

        /* RWORK */
        ssSetDWorkWidthAsInt(rts, 0, 1);
        ssSetDWorkDataType(rts, 0,SS_DOUBLE);
        ssSetDWorkComplexSignal(rts, 0, 0);
        ssSetDWork(rts, 0, &MCU_DW.StateSpace_RWORK);

        /* IWORK */
        ssSetDWorkWidthAsInt(rts, 1, 12);
        ssSetDWorkDataType(rts, 1,SS_INTEGER);
        ssSetDWorkComplexSignal(rts, 1, 0);
        ssSetDWork(rts, 1, &MCU_DW.StateSpace_IWORK[0]);

        /* PWORK */
        ssSetDWorkWidthAsInt(rts, 2, 65);
        ssSetDWorkDataType(rts, 2,SS_POINTER);
        ssSetDWorkComplexSignal(rts, 2, 0);
        ssSetDWork(rts, 2, &MCU_DW.StateSpace_PWORK[0]);

        /* DSTATE */
        ssSetDWorkWidthAsInt(rts, 3, 1);
        ssSetDWorkDataType(rts, 3,SS_DOUBLE);
        ssSetDWorkComplexSignal(rts, 3, 0);
        ssSetDWorkUsedAsDState(rts, 3, 1);
        ssSetDWork(rts, 3, &MCU_DW.StateSpace_DSTATE);
      }

      /* registration */
      sfun_spssw_discc_DSS(rts);
      sfcnInitializeSizes(rts);
      sfcnInitializeSampleTimes(rts);

      /* adjust sample time */
      ssSetSampleTime(rts, 0, 5.0E-6);
      ssSetOffsetTime(rts, 0, 0.0);
      sfcnTsMap[0] = 0;

      /* set compiled values of dynamic vector attributes */
      ssSetNumNonsampledZCsAsInt(rts, 0);

      /* Update connectivity flags for each port */
      _ssSetInputPortConnected(rts, 0, 1);
      _ssSetInputPortConnected(rts, 1, 1);
      _ssSetInputPortConnected(rts, 2, 1);
      _ssSetOutputPortConnected(rts, 0, 1);
      _ssSetOutputPortConnected(rts, 1, 1);
      _ssSetOutputPortConnected(rts, 2, 1);
      _ssSetOutputPortBeingMerged(rts, 0, 0);
      _ssSetOutputPortBeingMerged(rts, 1, 0);
      _ssSetOutputPortBeingMerged(rts, 2, 0);

      /* Update the BufferDstPort flags for each input port */
      ssSetInputPortBufferDstPort(rts, 0, -1);
      ssSetInputPortBufferDstPort(rts, 1, -1);
      ssSetInputPortBufferDstPort(rts, 2, -1);
    }
  }

  /* user code (registration function exit) */
  {
    if (NULL != MCU_DW.SFunction_SysVar) {
      cnReleaseSystemVariable(MCU_DW.SFunction_SysVar);
    }

    MCU_DW.SFunction_SysVar = (void*)cnCreateOutputSystemVariable(
      "MCU",
      "Motor_Torque",
      1,
      (int_T)-1.0,
      false,
      false,
      1.0);
  }

  {
    if (NULL != MCU_DW.SFunction_SysVar_k) {
      cnReleaseSystemVariable(MCU_DW.SFunction_SysVar_k);
    }

    MCU_DW.SFunction_SysVar_k = (void*)cnCreateInputSystemVariable(
      "MCU",
      "Target_Torque",
      1,
      (int_T)-1.0,
      false,
      false,
      1.0);
  }

  {
    if (NULL != MCU_DW.SFunction_SysVar_a) {
      cnReleaseSystemVariable(MCU_DW.SFunction_SysVar_a);
    }

    MCU_DW.SFunction_SysVar_a = (void*)cnCreateInputSystemVariable(
      "MCU",
      "Regen_Mode",
      1,
      (int_T)-1.0,
      false,
      false,
      1.0);
  }

  {
    if (NULL != MCU_DW.SFunction_SysVar_f) {
      cnReleaseSystemVariable(MCU_DW.SFunction_SysVar_f);
    }

    MCU_DW.SFunction_SysVar_f = (void*)cnCreateOutputSystemVariable(
      "BMS",
      "SoC",
      1,
      (int_T)-1.0,
      false,
      false,
      1.0);
  }

  {
    if (NULL != MCU_DW.SFunction_SysVar_m) {
      cnReleaseSystemVariable(MCU_DW.SFunction_SysVar_m);
    }

    MCU_DW.SFunction_SysVar_m = (void*)cnCreateOutputSystemVariable(
      "BMS",
      "Current",
      1,
      (int_T)-1.0,
      false,
      false,
      1.0);
  }

  {
    if (NULL != MCU_DW.SFunction_SysVar_kc) {
      cnReleaseSystemVariable(MCU_DW.SFunction_SysVar_kc);
    }

    MCU_DW.SFunction_SysVar_kc = (void*)cnCreateOutputSystemVariable(
      "BMS",
      "Voltage",
      1,
      (int_T)-1.0,
      false,
      false,
      1.0);
  }

  /* Initialize Sizes */
  MCU_M->Sizes.numContStates = (0);    /* Number of continuous states */
  MCU_M->Sizes.numY = (0);             /* Number of model outputs */
  MCU_M->Sizes.numU = (0);             /* Number of model inputs */
  MCU_M->Sizes.sysDirFeedThru = (0);   /* The model is not direct feedthrough */
  MCU_M->Sizes.numSampTimes = (2);     /* Number of sample times */
  MCU_M->Sizes.numBlocks = (227);      /* Number of blocks */
  MCU_M->Sizes.numBlockIO = (35);      /* Number of block outputs */
  MCU_M->Sizes.numBlockPrms = (310);   /* Sum of parameter "widths" */
  return MCU_M;
}

/*========================================================================*
 * End of Classic call interface                                          *
 *========================================================================*/
#if defined(MULTITASKING)

void CallRateMonotonicScheduler(void)
{
//rate_monotonic_scheduler();
}

#endif
