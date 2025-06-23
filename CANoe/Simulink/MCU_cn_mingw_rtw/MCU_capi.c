/*
 * MCU_capi.c
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

#include "rtw_capi.h"
#ifdef HOST_CAPI_BUILD
#include "MCU_capi_host.h"
#define sizeof(...)                    ((size_t)(0xFFFF))
#undef rt_offsetof
#define rt_offsetof(s,el)              ((uint16_T)(0xFFFF))
#define TARGET_CONST
#define TARGET_STRING(s)               (s)
#else                                  /* HOST_CAPI_BUILD */
#include "builtin_typeid_types.h"
#include "MCU.h"
#include "MCU_capi.h"
#include "MCU_private.h"
#ifdef LIGHT_WEIGHT_CAPI
#define TARGET_CONST
#define TARGET_STRING(s)               ((NULL))
#else
#define TARGET_CONST                   const
#define TARGET_STRING(s)               (s)
#endif
#endif                                 /* HOST_CAPI_BUILD */

/* Block output signal information */
static const rtwCAPI_Signals rtBlockSignals[] = {
  /* addrMapIndex, sysNum, blockPath,
   * signalName, portNumber, dataTypeIndex, dimIndex, fxpIndex, sTimeIndex
   */
  { 0, 4, TARGET_STRING("MCU/Subsystem"),
    TARGET_STRING(""), 0, 0, 0, 0, 0 },

  { 1, 5, TARGET_STRING("MCU/Subsystem1"),
    TARGET_STRING(""), 0, 1, 0, 0, 0 },

  { 2, 6, TARGET_STRING("MCU/Subsystem2"),
    TARGET_STRING(""), 0, 0, 0, 0, 0 },

  { 3, 7, TARGET_STRING("MCU/Subsystem3"),
    TARGET_STRING(""), 0, 0, 0, 0, 0 },

  { 4, 14, TARGET_STRING("MCU/current controller"),
    TARGET_STRING(""), 0, 0, 0, 0, 1 },

  { 5, 21, TARGET_STRING("MCU/current controller"),
    TARGET_STRING(""), 0, 0, 0, 0, 1 },

  { 6, 17, TARGET_STRING("MCU/current controller"),
    TARGET_STRING(""), 0, 0, 0, 0, 1 },

  { 7, 21, TARGET_STRING("MCU/current controller"),
    TARGET_STRING(""), 0, 0, 0, 0, 1 },

  { 8, 20, TARGET_STRING("MCU/current controller"),
    TARGET_STRING(""), 0, 0, 0, 0, 1 },

  { 9, 21, TARGET_STRING("MCU/current controller"),
    TARGET_STRING(""), 0, 0, 0, 0, 1 },

  { 10, 0, TARGET_STRING("MCU/Data Type Conversion"),
    TARGET_STRING(""), 0, 1, 0, 0, 1 },

  { 11, 0, TARGET_STRING("MCU/Gain2"),
    TARGET_STRING(""), 0, 0, 0, 0, 1 },

  { 12, 0, TARGET_STRING("MCU/Simulation Step/S-Function"),
    TARGET_STRING(""), 0, 0, 0, 0, 1 },

  { 13, 0, TARGET_STRING("MCU/Simulation Step/S-Function1"),
    TARGET_STRING(""), 0, 0, 0, 0, 1 },

  { 14, 0, TARGET_STRING("MCU/Simulation Step/S-Function1"),
    TARGET_STRING(""), 1, 0, 0, 0, 2 },

  { 15, 4, TARGET_STRING("MCU/Subsystem/In1"),
    TARGET_STRING(""), 0, 0, 0, 0, 0 },

  { 16, 5, TARGET_STRING("MCU/Subsystem1/In1"),
    TARGET_STRING(""), 0, 1, 0, 0, 0 },

  { 17, 6, TARGET_STRING("MCU/Subsystem2/In1"),
    TARGET_STRING(""), 0, 0, 0, 0, 0 },

  { 18, 7, TARGET_STRING("MCU/Subsystem3/In1"),
    TARGET_STRING(""), 0, 0, 0, 0, 0 },

  { 19, 0, TARGET_STRING("MCU/System Variable Input/S-Function"),
    TARGET_STRING(""), 0, 0, 0, 0, 1 },

  { 20, 0, TARGET_STRING("MCU/System Variable Input1/S-Function"),
    TARGET_STRING(""), 0, 0, 0, 0, 1 },

  { 21, 14, TARGET_STRING("MCU/current controller/Custom Relay"),
    TARGET_STRING(""), 0, 0, 0, 0, 1 },

  { 22, 17, TARGET_STRING("MCU/current controller/Custom Relay1"),
    TARGET_STRING(""), 0, 0, 0, 0, 1 },

  { 23, 20, TARGET_STRING("MCU/current controller/Custom Relay2"),
    TARGET_STRING(""), 0, 0, 0, 0, 1 },

  { 24, 21, TARGET_STRING("MCU/current controller/Data Type Conversion1"),
    TARGET_STRING(""), 0, 0, 0, 0, 1 },

  { 25, 21, TARGET_STRING("MCU/current controller/Data Type Conversion3"),
    TARGET_STRING(""), 0, 0, 0, 0, 1 },

  { 26, 21, TARGET_STRING("MCU/current controller/Data Type Conversion5"),
    TARGET_STRING(""), 0, 0, 0, 0, 1 },

  { 27, 0, TARGET_STRING(
    "MCU/Battery/Current Measurement/do not delete this gain"),
    TARGET_STRING(""), 0, 0, 0, 0, 1 },

  { 28, 0, TARGET_STRING(
    "MCU/Permanent Magnet Synchronous Machine/Electrical model/first 2 columns of D"),
    TARGET_STRING(""), 0, 0, 1, 0, 1 },

  { 29, 0, TARGET_STRING(
    "MCU/Permanent Magnet Synchronous Machine/Mechanical model/Gain"),
    TARGET_STRING(""), 0, 0, 0, 0, 1 },

  { 30, 0, TARGET_STRING(
    "MCU/Permanent Magnet Synchronous Machine/Mechanical model/Gain2"),
    TARGET_STRING(""), 0, 0, 0, 0, 1 },

  { 31, 0, TARGET_STRING("MCU/Universal Bridge1/Model/Constant"),
    TARGET_STRING(""), 0, 0, 2, 0, 1 },

  { 32, 14, TARGET_STRING("MCU/current controller/Custom Relay/Index Vector"),
    TARGET_STRING(""), 0, 0, 0, 0, 1 },

  { 33, 17, TARGET_STRING("MCU/current controller/Custom Relay1/Index Vector"),
    TARGET_STRING(""), 0, 0, 0, 0, 1 },

  { 34, 20, TARGET_STRING("MCU/current controller/Custom Relay2/Index Vector"),
    TARGET_STRING(""), 0, 0, 0, 0, 1 },

  { 35, 0, TARGET_STRING("MCU/powergui/EquivalentModel1/State-Space"),
    TARGET_STRING(""), 0, 0, 3, 0, 1 },

  { 36, 0, TARGET_STRING("MCU/powergui/EquivalentModel1/State-Space"),
    TARGET_STRING(""), 1, 0, 2, 0, 1 },

  { 37, 0, TARGET_STRING("MCU/powergui/EquivalentModel1/State-Space"),
    TARGET_STRING(""), 2, 0, 0, 0, 1 },

  { 38, 0, TARGET_STRING("MCU/powergui/EquivalentModel1/Sum"),
    TARGET_STRING(""), 0, 0, 4, 0, 1 },

  { 39, 0, TARGET_STRING("MCU/Battery/Model/Discrete/Data Type Conversion2"),
    TARGET_STRING(""), 0, 0, 0, 0, 1 },

  { 40, 0, TARGET_STRING("MCU/Battery/Model/Discrete/Gain2"),
    TARGET_STRING(""), 0, 0, 0, 0, 1 },

  { 41, 0, TARGET_STRING("MCU/Battery/Model/Discrete/Saturation"),
    TARGET_STRING("Voltage (V)"), 0, 0, 0, 0, 1 },

  { 42, 0, TARGET_STRING("MCU/Battery/Model/Discrete/Add"),
    TARGET_STRING(""), 0, 0, 0, 0, 1 },

  { 43, 0, TARGET_STRING("MCU/Battery/Model/Discrete/Current Filter"),
    TARGET_STRING(""), 0, 0, 0, 0, 1 },

  { 44, 0, TARGET_STRING(
    "MCU/Permanent Magnet Synchronous Machine/Electrical model/Ihistory_States/Add"),
    TARGET_STRING(""), 0, 0, 5, 0, 1 },

  { 45, 0, TARGET_STRING(
    "MCU/Permanent Magnet Synchronous Machine/Electrical model/Ihistory_States/Add2"),
    TARGET_STRING(""), 0, 0, 5, 0, 1 },

  { 46, 0, TARGET_STRING(
    "MCU/Battery/Model/Discrete/Exponential Zone Voltage/Add1"),
    TARGET_STRING(""), 0, 0, 0, 0, 1 },

  { 47, 22, TARGET_STRING(
    "MCU/speed controller/Discrete PID Controller/I Gain/Internal Parameters/Integral Gain"),
    TARGET_STRING(""), 0, 0, 0, 0, 1 },

  {
    0, 0, (NULL), (NULL), 0, 0, 0, 0, 0
  }
};

static const rtwCAPI_BlockParameters rtBlockParameters[] = {
  /* addrMapIndex, blockPath,
   * paramName, dataTypeIndex, dimIndex, fixPtIdx
   */
  { 48, TARGET_STRING("MCU/Battery"),
    TARGET_STRING("SOC"), 0, 0, 0 },

  { 49, TARGET_STRING("MCU/AUTO"),
    TARGET_STRING("Value"), 0, 0, 0 },

  { 50, TARGET_STRING("MCU/Constant"),
    TARGET_STRING("Value"), 0, 0, 0 },

  { 51, TARGET_STRING("MCU/Regen_Mode1"),
    TARGET_STRING("Value"), 0, 0, 0 },

  { 52, TARGET_STRING("MCU/Regen_Mode2"),
    TARGET_STRING("Value"), 0, 0, 0 },

  { 53, TARGET_STRING("MCU/Regen_Mode3"),
    TARGET_STRING("Value"), 0, 0, 0 },

  { 54, TARGET_STRING("MCU/Gain2"),
    TARGET_STRING("Gain"), 0, 0, 0 },

  { 55, TARGET_STRING("MCU/Simulation Step/S-Function"),
    TARGET_STRING("P1"), 0, 0, 0 },

  { 56, TARGET_STRING("MCU/Simulation Step/S-Function1"),
    TARGET_STRING("P1"), 0, 0, 0 },

  { 57, TARGET_STRING("MCU/speed controller/Discrete PID Controller"),
    TARGET_STRING("P"), 0, 0, 0 },

  { 58, TARGET_STRING("MCU/speed controller/Discrete PID Controller"),
    TARGET_STRING("I"), 0, 0, 0 },

  { 59, TARGET_STRING("MCU/speed controller/Discrete PID Controller"),
    TARGET_STRING("InitialConditionForIntegrator"), 0, 0, 0 },

  { 60, TARGET_STRING("MCU/Battery/Current Measurement/do not delete this gain"),
    TARGET_STRING("Gain"), 0, 0, 0 },

  { 61, TARGET_STRING("MCU/Permanent Magnet Synchronous Machine/Electrical model/I"),
    TARGET_STRING("Value"), 0, 1, 0 },

  { 62, TARGET_STRING("MCU/Permanent Magnet Synchronous Machine/Electrical model/M(2,5)"),
    TARGET_STRING("Value"), 0, 6, 0 },

  { 63, TARGET_STRING("MCU/Permanent Magnet Synchronous Machine/Electrical model/-R//L"),
    TARGET_STRING("Gain"), 0, 0, 0 },

  { 64, TARGET_STRING("MCU/Permanent Magnet Synchronous Machine/Electrical model/1//(3*L)"),
    TARGET_STRING("Gain"), 0, 0, 0 },

  { 65, TARGET_STRING("MCU/Permanent Magnet Synchronous Machine/Electrical model/Gain3"),
    TARGET_STRING("Gain"), 0, 0, 0 },

  { 66, TARGET_STRING("MCU/Permanent Magnet Synchronous Machine/Mechanical model/Coulomb & Viscous Friction"),
    TARGET_STRING("offset"), 0, 0, 0 },

  { 67, TARGET_STRING("MCU/Permanent Magnet Synchronous Machine/Mechanical model/Coulomb & Viscous Friction"),
    TARGET_STRING("gain"), 0, 0, 0 },

  { 68, TARGET_STRING("MCU/Permanent Magnet Synchronous Machine/Mechanical model/Discrete-Time Integrator"),
    TARGET_STRING("gainval"), 0, 0, 0 },

  { 69, TARGET_STRING("MCU/Permanent Magnet Synchronous Machine/Mechanical model/Discrete-Time Integrator"),
    TARGET_STRING("InitialCondition"), 0, 0, 0 },

  { 70, TARGET_STRING("MCU/Permanent Magnet Synchronous Machine/Mechanical model/Discrete-Time Integrator1"),
    TARGET_STRING("gainval"), 0, 0, 0 },

  { 71, TARGET_STRING("MCU/Permanent Magnet Synchronous Machine/Mechanical model/Discrete-Time Integrator1"),
    TARGET_STRING("InitialCondition"), 0, 0, 0 },

  { 72, TARGET_STRING("MCU/Permanent Magnet Synchronous Machine/Mechanical model/Gain"),
    TARGET_STRING("Gain"), 0, 0, 0 },

  { 73, TARGET_STRING("MCU/Permanent Magnet Synchronous Machine/Mechanical model/Gain2"),
    TARGET_STRING("Gain"), 0, 0, 0 },

  { 74, TARGET_STRING("MCU/Universal Bridge1/Model/Constant"),
    TARGET_STRING("Value"), 0, 7, 0 },

  { 75, TARGET_STRING("MCU/current controller/Custom Relay/OFF"),
    TARGET_STRING("Value"), 0, 0, 0 },

  { 76, TARGET_STRING("MCU/current controller/Custom Relay/ON"),
    TARGET_STRING("Value"), 0, 0, 0 },

  { 77, TARGET_STRING("MCU/current controller/Custom Relay/offset"),
    TARGET_STRING("Value"), 0, 0, 0 },

  { 78, TARGET_STRING("MCU/current controller/Custom Relay/rev"),
    TARGET_STRING("Value"), 0, 0, 0 },

  { 79, TARGET_STRING("MCU/current controller/Custom Relay/Delay"),
    TARGET_STRING("InitialCondition"), 0, 0, 0 },

  { 80, TARGET_STRING("MCU/current controller/Custom Relay1/OFF"),
    TARGET_STRING("Value"), 0, 0, 0 },

  { 81, TARGET_STRING("MCU/current controller/Custom Relay1/ON"),
    TARGET_STRING("Value"), 0, 0, 0 },

  { 82, TARGET_STRING("MCU/current controller/Custom Relay1/offset"),
    TARGET_STRING("Value"), 0, 0, 0 },

  { 83, TARGET_STRING("MCU/current controller/Custom Relay1/rev"),
    TARGET_STRING("Value"), 0, 0, 0 },

  { 84, TARGET_STRING("MCU/current controller/Custom Relay1/Delay"),
    TARGET_STRING("InitialCondition"), 0, 0, 0 },

  { 85, TARGET_STRING("MCU/current controller/Custom Relay2/OFF"),
    TARGET_STRING("Value"), 0, 0, 0 },

  { 86, TARGET_STRING("MCU/current controller/Custom Relay2/ON"),
    TARGET_STRING("Value"), 0, 0, 0 },

  { 87, TARGET_STRING("MCU/current controller/Custom Relay2/offset"),
    TARGET_STRING("Value"), 0, 0, 0 },

  { 88, TARGET_STRING("MCU/current controller/Custom Relay2/rev"),
    TARGET_STRING("Value"), 0, 0, 0 },

  { 89, TARGET_STRING("MCU/current controller/Custom Relay2/Delay"),
    TARGET_STRING("InitialCondition"), 0, 0, 0 },

  { 90, TARGET_STRING("MCU/powergui/EquivalentModel1/State-Space"),
    TARGET_STRING("P1"), 0, 0, 0 },

  { 91, TARGET_STRING("MCU/powergui/EquivalentModel1/State-Space"),
    TARGET_STRING("P2"), 0, 0, 0 },

  { 92, TARGET_STRING("MCU/powergui/EquivalentModel1/State-Space"),
    TARGET_STRING("P3"), 0, 8, 0 },

  { 93, TARGET_STRING("MCU/powergui/EquivalentModel1/State-Space"),
    TARGET_STRING("P4"), 0, 3, 0 },

  { 94, TARGET_STRING("MCU/powergui/EquivalentModel1/State-Space"),
    TARGET_STRING("P5"), 0, 9, 0 },

  { 95, TARGET_STRING("MCU/powergui/EquivalentModel1/State-Space"),
    TARGET_STRING("P6"), 0, 0, 0 },

  { 96, TARGET_STRING("MCU/powergui/EquivalentModel1/State-Space"),
    TARGET_STRING("P7"), 0, 0, 0 },

  { 97, TARGET_STRING("MCU/powergui/EquivalentModel1/State-Space"),
    TARGET_STRING("P8"), 0, 10, 0 },

  { 98, TARGET_STRING("MCU/powergui/EquivalentModel1/State-Space"),
    TARGET_STRING("P10"), 0, 11, 0 },

  { 99, TARGET_STRING("MCU/powergui/EquivalentModel1/State-Space"),
    TARGET_STRING("P11"), 0, 11, 0 },

  { 100, TARGET_STRING("MCU/powergui/EquivalentModel1/State-Space"),
    TARGET_STRING("P12"), 0, 10, 0 },

  { 101, TARGET_STRING("MCU/powergui/EquivalentModel1/State-Space"),
    TARGET_STRING("P13"), 0, 0, 0 },

  { 102, TARGET_STRING("MCU/powergui/EquivalentModel1/State-Space"),
    TARGET_STRING("P14"), 0, 0, 0 },

  { 103, TARGET_STRING("MCU/powergui/EquivalentModel1/State-Space"),
    TARGET_STRING("P16"), 0, 0, 0 },

  { 104, TARGET_STRING("MCU/powergui/EquivalentModel1/State-Space"),
    TARGET_STRING("P17"), 0, 0, 0 },

  { 105, TARGET_STRING("MCU/powergui/EquivalentModel1/State-Space"),
    TARGET_STRING("P18"), 0, 0, 0 },

  { 106, TARGET_STRING("MCU/powergui/EquivalentModel1/State-Space"),
    TARGET_STRING("P19"), 0, 0, 0 },

  { 107, TARGET_STRING("MCU/powergui/EquivalentModel1/State-Space"),
    TARGET_STRING("P20"), 0, 7, 0 },

  { 108, TARGET_STRING("MCU/powergui/EquivalentModel1/State-Space"),
    TARGET_STRING("P21"), 0, 7, 0 },

  { 109, TARGET_STRING("MCU/powergui/EquivalentModel1/State-Space"),
    TARGET_STRING("P22"), 0, 2, 0 },

  { 110, TARGET_STRING("MCU/powergui/EquivalentModel1/State-Space"),
    TARGET_STRING("P23"), 0, 7, 0 },

  { 111, TARGET_STRING("MCU/powergui/EquivalentModel1/State-Space"),
    TARGET_STRING("P24"), 0, 0, 0 },

  { 112, TARGET_STRING("MCU/powergui/EquivalentModel1/Unit Delay"),
    TARGET_STRING("InitialCondition"), 0, 0, 0 },

  { 113, TARGET_STRING("MCU/Battery/Model/Discrete/Constant"),
    TARGET_STRING("Value"), 0, 0, 0 },

  { 114, TARGET_STRING("MCU/Battery/Model/Discrete/Constant1"),
    TARGET_STRING("Value"), 0, 0, 0 },

  { 115, TARGET_STRING("MCU/Battery/Model/Discrete/capacity"),
    TARGET_STRING("Value"), 0, 0, 0 },

  { 116, TARGET_STRING("MCU/Battery/Model/Discrete/constantA"),
    TARGET_STRING("Value"), 0, 0, 0 },

  { 117, TARGET_STRING("MCU/Battery/Model/Discrete/constantB"),
    TARGET_STRING("Value"), 0, 0, 0 },

  { 118, TARGET_STRING("MCU/Battery/Model/Discrete/constantK"),
    TARGET_STRING("Value"), 0, 0, 0 },

  { 119, TARGET_STRING("MCU/Battery/Model/Discrete/Coulomb Counter"),
    TARGET_STRING("gainval"), 0, 0, 0 },

  { 120, TARGET_STRING("MCU/Battery/Model/Discrete/Coulomb Counter"),
    TARGET_STRING("UpperSaturationLimit"), 0, 0, 0 },

  { 121, TARGET_STRING("MCU/Battery/Model/Discrete/Coulomb Counter"),
    TARGET_STRING("LowerSaturationLimit"), 0, 0, 0 },

  { 122, TARGET_STRING("MCU/Battery/Model/Discrete/Gain"),
    TARGET_STRING("Gain"), 0, 0, 0 },

  { 123, TARGET_STRING("MCU/Battery/Model/Discrete/Gain1"),
    TARGET_STRING("Gain"), 0, 0, 0 },

  { 124, TARGET_STRING("MCU/Battery/Model/Discrete/Gain2"),
    TARGET_STRING("Gain"), 0, 0, 0 },

  { 125, TARGET_STRING("MCU/Battery/Model/Discrete/Saturation"),
    TARGET_STRING("UpperLimit"), 0, 0, 0 },

  { 126, TARGET_STRING("MCU/Battery/Model/Discrete/Saturation"),
    TARGET_STRING("LowerLimit"), 0, 0, 0 },

  { 127, TARGET_STRING("MCU/Battery/Model/Discrete/Current Filter"),
    TARGET_STRING("Numerator"), 0, 0, 0 },

  { 128, TARGET_STRING("MCU/Battery/Model/Discrete/Current Filter"),
    TARGET_STRING("Denominator"), 0, 11, 0 },

  { 129, TARGET_STRING("MCU/Battery/Model/Discrete/Current Filter"),
    TARGET_STRING("InitialStates"), 0, 0, 0 },

  { 130, TARGET_STRING("MCU/Battery/Model/Discrete/Unit Delay"),
    TARGET_STRING("InitialCondition"), 0, 0, 0 },

  { 131, TARGET_STRING("MCU/Permanent Magnet Synchronous Machine/Electrical model/Discretize  TBE or Tustin/u1"),
    TARGET_STRING("Value"), 2, 0, 0 },

  { 132, TARGET_STRING("MCU/Permanent Magnet Synchronous Machine/Electrical model/Discretize  TBE or Tustin/u5"),
    TARGET_STRING("Value"), 0, 1, 0 },

  { 133, TARGET_STRING("MCU/Permanent Magnet Synchronous Machine/Electrical model/Discretize  TBE or Tustin/Tustin: 1//2 BE : 1.0"),
    TARGET_STRING("Gain"), 0, 0, 0 },

  { 134, TARGET_STRING("MCU/Permanent Magnet Synchronous Machine/Electrical model/Discretize  TBE or Tustin/Tustin: Ts//2 BE : Ts"),
    TARGET_STRING("Gain"), 0, 0, 0 },

  { 135, TARGET_STRING("MCU/Permanent Magnet Synchronous Machine/Electrical model/Discretize  TBE or Tustin/wbase3"),
    TARGET_STRING("Gain"), 0, 0, 0 },

  { 136, TARGET_STRING("MCU/Permanent Magnet Synchronous Machine/Electrical model/Hall effect sensor/Constant"),
    TARGET_STRING("Value"), 0, 0, 0 },

  { 137, TARGET_STRING("MCU/Permanent Magnet Synchronous Machine/Electrical model/Hall effect sensor/Constant1"),
    TARGET_STRING("Value"), 0, 0, 0 },

  { 138, TARGET_STRING("MCU/Permanent Magnet Synchronous Machine/Electrical model/Hall effect sensor/Constant2"),
    TARGET_STRING("Value"), 0, 0, 0 },

  { 139, TARGET_STRING("MCU/Permanent Magnet Synchronous Machine/Electrical model/Hall effect sensor/Constant3"),
    TARGET_STRING("Value"), 0, 0, 0 },

  { 140, TARGET_STRING("MCU/Permanent Magnet Synchronous Machine/Electrical model/Hall effect sensor/Constant4"),
    TARGET_STRING("Value"), 0, 0, 0 },

  { 141, TARGET_STRING("MCU/Permanent Magnet Synchronous Machine/Electrical model/Hall effect sensor/Constant5"),
    TARGET_STRING("Value"), 0, 0, 0 },

  { 142, TARGET_STRING("MCU/Permanent Magnet Synchronous Machine/Electrical model/Ihistory_States/BE=1 Tustin=1//2"),
    TARGET_STRING("Gain"), 0, 0, 0 },

  { 143, TARGET_STRING("MCU/Permanent Magnet Synchronous Machine/Electrical model/Ihistory_States/Ts"),
    TARGET_STRING("Gain"), 0, 0, 0 },

  { 144, TARGET_STRING("MCU/Permanent Magnet Synchronous Machine/Electrical model/Ihistory_States/Unit Delay"),
    TARGET_STRING("InitialCondition"), 0, 5, 0 },

  { 145, TARGET_STRING("MCU/Battery/Model/Discrete/Compare To Zero/Constant"),
    TARGET_STRING("Value"), 0, 0, 0 },

  { 146, TARGET_STRING("MCU/Battery/Model/Discrete/Compare To Zero1/Constant"),
    TARGET_STRING("Value"), 0, 0, 0 },

  { 147, TARGET_STRING("MCU/Battery/Model/Discrete/Exponential Zone Voltage/Discrete-Time Integrator"),
    TARGET_STRING("gainval"), 0, 0, 0 },

  { 148, TARGET_STRING("MCU/Battery/Model/Discrete/Exponential Zone Voltage/Gain"),
    TARGET_STRING("Gain"), 0, 0, 0 },

  { 149, TARGET_STRING("MCU/Battery/Model/Discrete/Exponential Zone Voltage/Gain1"),
    TARGET_STRING("Gain"), 0, 0, 0 },

  { 150, TARGET_STRING("MCU/Battery/Model/Discrete/SoC/Gain1"),
    TARGET_STRING("Gain"), 0, 0, 0 },

  { 151, TARGET_STRING("MCU/Battery/Model/Discrete/SoC/Saturation"),
    TARGET_STRING("UpperLimit"), 0, 0, 0 },

  { 152, TARGET_STRING("MCU/Battery/Model/Discrete/SoC/Saturation"),
    TARGET_STRING("LowerLimit"), 0, 0, 0 },

  { 153, TARGET_STRING("MCU/Permanent Magnet Synchronous Machine/Electrical model/BEMF, Flux/Phase a/Gain4"),
    TARGET_STRING("Gain"), 0, 0, 0 },

  { 154, TARGET_STRING("MCU/Permanent Magnet Synchronous Machine/Electrical model/BEMF, Flux/Phase a/Saturation"),
    TARGET_STRING("UpperLimit"), 0, 0, 0 },

  { 155, TARGET_STRING("MCU/Permanent Magnet Synchronous Machine/Electrical model/BEMF, Flux/Phase a/Saturation"),
    TARGET_STRING("LowerLimit"), 0, 0, 0 },

  { 156, TARGET_STRING("MCU/Permanent Magnet Synchronous Machine/Electrical model/BEMF, Flux/Phase b/Gain4"),
    TARGET_STRING("Gain"), 0, 0, 0 },

  { 157, TARGET_STRING("MCU/Permanent Magnet Synchronous Machine/Electrical model/BEMF, Flux/Phase b/Saturation"),
    TARGET_STRING("UpperLimit"), 0, 0, 0 },

  { 158, TARGET_STRING("MCU/Permanent Magnet Synchronous Machine/Electrical model/BEMF, Flux/Phase b/Saturation"),
    TARGET_STRING("LowerLimit"), 0, 0, 0 },

  { 159, TARGET_STRING("MCU/Permanent Magnet Synchronous Machine/Electrical model/BEMF, Flux/Phase c/Gain4"),
    TARGET_STRING("Gain"), 0, 0, 0 },

  { 160, TARGET_STRING("MCU/Permanent Magnet Synchronous Machine/Electrical model/BEMF, Flux/Phase c/Saturation"),
    TARGET_STRING("UpperLimit"), 0, 0, 0 },

  { 161, TARGET_STRING("MCU/Permanent Magnet Synchronous Machine/Electrical model/BEMF, Flux/Phase c/Saturation"),
    TARGET_STRING("LowerLimit"), 0, 0, 0 },

  { 162, TARGET_STRING("MCU/Permanent Magnet Synchronous Machine/Electrical model/Hall effect sensor/Angle converter/rad2deg"),
    TARGET_STRING("Gain"), 0, 0, 0 },

  { 163, TARGET_STRING("MCU/speed controller/Discrete PID Controller/Integrator/Discrete/Integrator"),
    TARGET_STRING("gainval"), 0, 0, 0 },

  {
    0, (NULL), (NULL), 0, 0, 0
  }
};

/* Tunable variable parameters */
static const rtwCAPI_ModelParameters rtModelParameters[] = {
  /* addrMapIndex, varName, dataTypeIndex, dimIndex, fixPtIndex */
  { 0, (NULL), 0, 0, 0 }
};

#ifndef HOST_CAPI_BUILD

/* Declare Data Addresses statically */
static void* rtDataAddrMap[] = {
  &MCU_B.Subsystem.In1,                /* 0: Signal */
  &MCU_B.In1,                          /* 1: Signal */
  &MCU_B.Subsystem2.In1,               /* 2: Signal */
  &MCU_B.Subsystem3.In1,               /* 3: Signal */
  &MCU_B.CustomRelay.IndexVector,      /* 4: Signal */
  &MCU_B.DataTypeConversion1,          /* 5: Signal */
  &MCU_B.CustomRelay1.IndexVector,     /* 6: Signal */
  &MCU_B.DataTypeConversion3,          /* 7: Signal */
  &MCU_B.CustomRelay2.IndexVector,     /* 8: Signal */
  &MCU_B.DataTypeConversion5,          /* 9: Signal */
  &MCU_B.DataTypeConversion,           /* 10: Signal */
  &MCU_B.Gain2,                        /* 11: Signal */
  &MCU_B.SFunction,                    /* 12: Signal */
  &MCU_B.SFunction1_o1,                /* 13: Signal */
  (void *) &MCU_ConstB.SFunction1_o2,  /* 14: Signal */
  &MCU_B.Subsystem.In1,                /* 15: Signal */
  &MCU_B.In1,                          /* 16: Signal */
  &MCU_B.Subsystem2.In1,               /* 17: Signal */
  &MCU_B.Subsystem3.In1,               /* 18: Signal */
  &MCU_B.SFunction_j,                  /* 19: Signal */
  &MCU_B.SFunction_m,                  /* 20: Signal */
  &MCU_B.CustomRelay.IndexVector,      /* 21: Signal */
  &MCU_B.CustomRelay1.IndexVector,     /* 22: Signal */
  &MCU_B.CustomRelay2.IndexVector,     /* 23: Signal */
  &MCU_B.DataTypeConversion1,          /* 24: Signal */
  &MCU_B.DataTypeConversion3,          /* 25: Signal */
  &MCU_B.DataTypeConversion5,          /* 26: Signal */
  &MCU_B.donotdeletethisgain,          /* 27: Signal */
  &MCU_B.first2columnsofD[0],          /* 28: Signal */
  &MCU_B.Gain,                         /* 29: Signal */
  &MCU_B.Gain2_i,                      /* 30: Signal */
  &MCU_B.Constant[0],                  /* 31: Signal */
  &MCU_B.CustomRelay.IndexVector,      /* 32: Signal */
  &MCU_B.CustomRelay1.IndexVector,     /* 33: Signal */
  &MCU_B.CustomRelay2.IndexVector,     /* 34: Signal */
  &MCU_B.StateSpace_o1[0],             /* 35: Signal */
  &MCU_B.StateSpace_o2[0],             /* 36: Signal */
  &MCU_B.StateSpace_o3,                /* 37: Signal */
  &MCU_B.Sum[0],                       /* 38: Signal */
  &MCU_B.DataTypeConversion2,          /* 39: Signal */
  &MCU_B.Gain2_a,                      /* 40: Signal */
  &MCU_B.VoltageV,                     /* 41: Signal */
  &MCU_B.Add_c,                        /* 42: Signal */
  &MCU_B.CurrentFilter,                /* 43: Signal */
  &MCU_B.Add[0],                       /* 44: Signal */
  &MCU_B.Add2[0],                      /* 45: Signal */
  &MCU_B.Add1,                         /* 46: Signal */
  &MCU_B.IntegralGain,                 /* 47: Signal */
  &MCU_P.Battery_SOC,                  /* 48: Mask Parameter */
  &MCU_P.AUTO_Value,                   /* 49: Block Parameter */
  &MCU_P.Constant_Value_h,             /* 50: Block Parameter */
  &MCU_P.Regen_Mode1_Value,            /* 51: Block Parameter */
  &MCU_P.Regen_Mode2_Value,            /* 52: Block Parameter */
  &MCU_P.Regen_Mode3_Value,            /* 53: Block Parameter */
  &MCU_P.Gain2_Gain,                   /* 54: Block Parameter */
  &MCU_P.SFunction_P1,                 /* 55: Block Parameter */
  &MCU_P.SFunction1_P1,                /* 56: Block Parameter */
  &MCU_P.DiscretePIDController_P,      /* 57: Mask Parameter */
  &MCU_P.DiscretePIDController_I,      /* 58: Mask Parameter */
  &MCU_P.DiscretePIDController_InitialCo,/* 59: Mask Parameter */
  &MCU_P.donotdeletethisgain_Gain,     /* 60: Block Parameter */
  &MCU_P.I_Value[0],                   /* 61: Block Parameter */
  &MCU_P.M25_Value[0],                 /* 62: Block Parameter */
  &MCU_P.RL_Gain,                      /* 63: Block Parameter */
  &MCU_P.u3L_Gain,                     /* 64: Block Parameter */
  &MCU_P.Gain3_Gain,                   /* 65: Block Parameter */
  &MCU_P.CoulombViscousFriction_offset,/* 66: Mask Parameter */
  &MCU_P.CoulombViscousFriction_gain,  /* 67: Mask Parameter */
  &MCU_P.DiscreteTimeIntegrator_gainval,/* 68: Block Parameter */
  &MCU_P.DiscreteTimeIntegrator_IC,    /* 69: Block Parameter */
  &MCU_P.DiscreteTimeIntegrator1_gainval,/* 70: Block Parameter */
  &MCU_P.DiscreteTimeIntegrator1_IC,   /* 71: Block Parameter */
  &MCU_P.Gain_Gain,                    /* 72: Block Parameter */
  &MCU_P.Gain2_Gain_l,                 /* 73: Block Parameter */
  &MCU_P.Constant_Value_o[0],          /* 74: Block Parameter */
  &MCU_P.CustomRelay.OFF_Value,        /* 75: Block Parameter */
  &MCU_P.CustomRelay.ON_Value,         /* 76: Block Parameter */
  &MCU_P.CustomRelay.offset_Value,     /* 77: Block Parameter */
  &MCU_P.CustomRelay.rev_Value,        /* 78: Block Parameter */
  &MCU_P.CustomRelay.Delay_InitialCondition,/* 79: Block Parameter */
  &MCU_P.CustomRelay1.OFF_Value,       /* 80: Block Parameter */
  &MCU_P.CustomRelay1.ON_Value,        /* 81: Block Parameter */
  &MCU_P.CustomRelay1.offset_Value,    /* 82: Block Parameter */
  &MCU_P.CustomRelay1.rev_Value,       /* 83: Block Parameter */
  &MCU_P.CustomRelay1.Delay_InitialCondition,/* 84: Block Parameter */
  &MCU_P.CustomRelay2.OFF_Value,       /* 85: Block Parameter */
  &MCU_P.CustomRelay2.ON_Value,        /* 86: Block Parameter */
  &MCU_P.CustomRelay2.offset_Value,    /* 87: Block Parameter */
  &MCU_P.CustomRelay2.rev_Value,       /* 88: Block Parameter */
  &MCU_P.CustomRelay2.Delay_InitialCondition,/* 89: Block Parameter */
  &MCU_P.StateSpace_P1,                /* 90: Block Parameter */
  &MCU_P.StateSpace_P2,                /* 91: Block Parameter */
  &MCU_P.StateSpace_P3[0],             /* 92: Block Parameter */
  &MCU_P.StateSpace_P4[0],             /* 93: Block Parameter */
  &MCU_P.StateSpace_P5[0],             /* 94: Block Parameter */
  &MCU_P.StateSpace_P6,                /* 95: Block Parameter */
  &MCU_P.StateSpace_P7,                /* 96: Block Parameter */
  &MCU_P.StateSpace_P8[0],             /* 97: Block Parameter */
  &MCU_P.StateSpace_P10[0],            /* 98: Block Parameter */
  &MCU_P.StateSpace_P11[0],            /* 99: Block Parameter */
  &MCU_P.StateSpace_P12[0],            /* 100: Block Parameter */
  &MCU_P.StateSpace_P13,               /* 101: Block Parameter */
  &MCU_P.StateSpace_P14,               /* 102: Block Parameter */
  &MCU_P.StateSpace_P16,               /* 103: Block Parameter */
  &MCU_P.StateSpace_P17,               /* 104: Block Parameter */
  &MCU_P.StateSpace_P18,               /* 105: Block Parameter */
  &MCU_P.StateSpace_P19,               /* 106: Block Parameter */
  &MCU_P.StateSpace_P20[0],            /* 107: Block Parameter */
  &MCU_P.StateSpace_P21[0],            /* 108: Block Parameter */
  &MCU_P.StateSpace_P22[0],            /* 109: Block Parameter */
  &MCU_P.StateSpace_P23[0],            /* 110: Block Parameter */
  &MCU_P.StateSpace_P24,               /* 111: Block Parameter */
  &MCU_P.UnitDelay_InitialCondition_o, /* 112: Block Parameter */
  &MCU_P.Constant_Value_f,             /* 113: Block Parameter */
  &MCU_P.Constant1_Value_f,            /* 114: Block Parameter */
  &MCU_P.capacity_Value,               /* 115: Block Parameter */
  &MCU_P.constantA_Value,              /* 116: Block Parameter */
  &MCU_P.constantB_Value,              /* 117: Block Parameter */
  &MCU_P.constantK_Value,              /* 118: Block Parameter */
  &MCU_P.CoulombCounter_gainval,       /* 119: Block Parameter */
  &MCU_P.CoulombCounter_UpperSat,      /* 120: Block Parameter */
  &MCU_P.CoulombCounter_LowerSat,      /* 121: Block Parameter */
  &MCU_P.Gain_Gain_e,                  /* 122: Block Parameter */
  &MCU_P.Gain1_Gain,                   /* 123: Block Parameter */
  &MCU_P.Gain2_Gain_i,                 /* 124: Block Parameter */
  &MCU_P.Saturation_UpperSat,          /* 125: Block Parameter */
  &MCU_P.Saturation_LowerSat,          /* 126: Block Parameter */
  &MCU_P.CurrentFilter_NumCoef,        /* 127: Block Parameter */
  &MCU_P.CurrentFilter_DenCoef[0],     /* 128: Block Parameter */
  &MCU_P.CurrentFilter_InitialStates,  /* 129: Block Parameter */
  &MCU_P.UnitDelay_InitialCondition,   /* 130: Block Parameter */
  &MCU_P.u1_Value,                     /* 131: Block Parameter */
  &MCU_P.u5_Value[0],                  /* 132: Block Parameter */
  &MCU_P.Tustin12BE10_Gain,            /* 133: Block Parameter */
  &MCU_P.TustinTs2BETs_Gain,           /* 134: Block Parameter */
  &MCU_P.wbase3_Gain,                  /* 135: Block Parameter */
  &MCU_P.Constant_Value_l,             /* 136: Block Parameter */
  &MCU_P.Constant1_Value,              /* 137: Block Parameter */
  &MCU_P.Constant2_Value,              /* 138: Block Parameter */
  &MCU_P.Constant3_Value,              /* 139: Block Parameter */
  &MCU_P.Constant4_Value,              /* 140: Block Parameter */
  &MCU_P.Constant5_Value,              /* 141: Block Parameter */
  &MCU_P.BE1Tustin12_Gain,             /* 142: Block Parameter */
  &MCU_P.Ts_Gain,                      /* 143: Block Parameter */
  &MCU_P.UnitDelay_InitialCondition_k[0],/* 144: Block Parameter */
  &MCU_P.Constant_Value,               /* 145: Block Parameter */
  &MCU_P.Constant_Value_g,             /* 146: Block Parameter */
  &MCU_P.DiscreteTimeIntegrator_gainva_b,/* 147: Block Parameter */
  &MCU_P.Gain_Gain_k,                  /* 148: Block Parameter */
  &MCU_P.Gain1_Gain_g,                 /* 149: Block Parameter */
  &MCU_P.Gain1_Gain_c,                 /* 150: Block Parameter */
  &MCU_P.Saturation_UpperSat_d,        /* 151: Block Parameter */
  &MCU_P.Saturation_LowerSat_e,        /* 152: Block Parameter */
  &MCU_P.Gain4_Gain,                   /* 153: Block Parameter */
  &MCU_P.Saturation_UpperSat_n,        /* 154: Block Parameter */
  &MCU_P.Saturation_LowerSat_g,        /* 155: Block Parameter */
  &MCU_P.Gain4_Gain_l,                 /* 156: Block Parameter */
  &MCU_P.Saturation_UpperSat_i,        /* 157: Block Parameter */
  &MCU_P.Saturation_LowerSat_d,        /* 158: Block Parameter */
  &MCU_P.Gain4_Gain_j,                 /* 159: Block Parameter */
  &MCU_P.Saturation_UpperSat_f,        /* 160: Block Parameter */
  &MCU_P.Saturation_LowerSat_f,        /* 161: Block Parameter */
  &MCU_P.rad2deg_Gain,                 /* 162: Block Parameter */
  &MCU_P.Integrator_gainval,           /* 163: Block Parameter */
};

/* Declare Data Run-Time Dimension Buffer Addresses statically */
static int32_T* rtVarDimsAddrMap[] = {
  (NULL)
};

#endif

/* Data Type Map - use dataTypeMapIndex to access this structure */
static TARGET_CONST rtwCAPI_DataTypeMap rtDataTypeMap[] = {
  /* cName, mwName, numElements, elemMapIndex, dataSize, slDataId, *
   * isComplex, isPointer, enumStorageType */
  { "double", "real_T", 0, 0, sizeof(real_T), (uint8_T)SS_DOUBLE, 0, 0, 0 },

  { "unsigned char", "uint8_T", 0, 0, sizeof(uint8_T), (uint8_T)SS_UINT8, 0, 0,
    0 },

  { "unsigned char", "boolean_T", 0, 0, sizeof(boolean_T), (uint8_T)SS_BOOLEAN,
    0, 0, 0 }
};

#ifdef HOST_CAPI_BUILD
#undef sizeof
#endif

/* Structure Element Map - use elemMapIndex to access this structure */
static TARGET_CONST rtwCAPI_ElementMap rtElementMap[] = {
  /* elementName, elementOffset, dataTypeIndex, dimIndex, fxpIndex */
  { (NULL), 0, 0, 0, 0 },
};

/* Dimension Map - use dimensionMapIndex to access elements of ths structure*/
static const rtwCAPI_DimensionMap rtDimensionMap[] = {
  /* dataOrientation, dimArrayIndex, numDims, vardimsIndex */
  { rtwCAPI_SCALAR, 0, 2, 0 },

  { rtwCAPI_MATRIX_COL_MAJOR, 2, 2, 0 },

  { rtwCAPI_VECTOR, 4, 2, 0 },

  { rtwCAPI_VECTOR, 6, 2, 0 },

  { rtwCAPI_MATRIX_COL_MAJOR, 8, 2, 0 },

  { rtwCAPI_VECTOR, 10, 2, 0 },

  { rtwCAPI_MATRIX_COL_MAJOR, 12, 2, 0 },

  { rtwCAPI_VECTOR, 14, 2, 0 },

  { rtwCAPI_VECTOR, 16, 2, 0 },

  { rtwCAPI_MATRIX_COL_MAJOR, 18, 2, 0 },

  { rtwCAPI_VECTOR, 20, 2, 0 },

  { rtwCAPI_VECTOR, 22, 2, 0 }
};

/* Dimension Array- use dimArrayIndex to access elements of this array */
static const uint_T rtDimensionArray[] = {
  1,                                   /* 0 */
  1,                                   /* 1 */
  2,                                   /* 2 */
  2,                                   /* 3 */
  6,                                   /* 4 */
  1,                                   /* 5 */
  9,                                   /* 6 */
  1,                                   /* 7 */
  4,                                   /* 8 */
  1,                                   /* 9 */
  2,                                   /* 10 */
  1,                                   /* 11 */
  2,                                   /* 12 */
  5,                                   /* 13 */
  1,                                   /* 14 */
  6,                                   /* 15 */
  1,                                   /* 16 */
  9,                                   /* 17 */
  9,                                   /* 18 */
  9,                                   /* 19 */
  1,                                   /* 20 */
  3,                                   /* 21 */
  1,                                   /* 22 */
  2                                    /* 23 */
};

/* C-API stores floating point values in an array. The elements of this  *
 * are unique. This ensures that values which are shared across the model*
 * are stored in the most efficient way. These values are referenced by  *
 *           - rtwCAPI_FixPtMap.fracSlopePtr,                            *
 *           - rtwCAPI_FixPtMap.biasPtr,                                 *
 *           - rtwCAPI_SampleTimeMap.samplePeriodPtr,                    *
 *           - rtwCAPI_SampleTimeMap.sampleOffsetPtr                     */
static const real_T rtcapiStoredFloats[] = {
  0.01, 0.0, 5.0E-6
};

/* Fixed Point Map */
static const rtwCAPI_FixPtMap rtFixPtMap[] = {
  /* fracSlopePtr, biasPtr, scaleType, wordLength, exponent, isSigned */
  { (NULL), (NULL), rtwCAPI_FIX_RESERVED, 0, 0, (boolean_T)0 },
};

/* Sample Time Map - use sTimeIndex to access elements of ths structure */
static const rtwCAPI_SampleTimeMap rtSampleTimeMap[] = {
  /* samplePeriodPtr, sampleOffsetPtr, tid, samplingMode */
  { (const void *) &rtcapiStoredFloats[0], (const void *) &rtcapiStoredFloats[1],
    (int8_T)1, (uint8_T)0 },

  { (const void *) &rtcapiStoredFloats[2], (const void *) &rtcapiStoredFloats[1],
    (int8_T)0, (uint8_T)0 },

  { (NULL), (NULL), -2, 0 }
};

static rtwCAPI_ModelMappingStaticInfo mmiStatic = {
  /* Signals:{signals, numSignals,
   *           rootInputs, numRootInputs,
   *           rootOutputs, numRootOutputs},
   * Params: {blockParameters, numBlockParameters,
   *          modelParameters, numModelParameters},
   * States: {states, numStates},
   * Maps:   {dataTypeMap, dimensionMap, fixPtMap,
   *          elementMap, sampleTimeMap, dimensionArray},
   * TargetType: targetType
   */
  { rtBlockSignals, 48,
    (NULL), 0,
    (NULL), 0 },

  { rtBlockParameters, 116,
    rtModelParameters, 0 },

  { (NULL), 0 },

  { rtDataTypeMap, rtDimensionMap, rtFixPtMap,
    rtElementMap, rtSampleTimeMap, rtDimensionArray },
  "float",

  { 124838842U,
    1933162013U,
    1287255322U,
    4200287352U },
  (NULL), 0,
  (boolean_T)0
};

/* Function to get C API Model Mapping Static Info */
const rtwCAPI_ModelMappingStaticInfo*
  MCU_GetCAPIStaticMap(void)
{
  return &mmiStatic;
}

/* Cache pointers into DataMapInfo substructure of RTModel */
#ifndef HOST_CAPI_BUILD

void MCU_InitializeDataMapInfo(void)
{
  /* Set C-API version */
  rtwCAPI_SetVersion(MCU_M->DataMapInfo.mmi, 1);

  /* Cache static C-API data into the Real-time Model Data structure */
  rtwCAPI_SetStaticMap(MCU_M->DataMapInfo.mmi, &mmiStatic);

  /* Cache static C-API logging data into the Real-time Model Data structure */
  rtwCAPI_SetLoggingStaticMap(MCU_M->DataMapInfo.mmi, (NULL));

  /* Cache C-API Data Addresses into the Real-Time Model Data structure */
  rtwCAPI_SetDataAddressMap(MCU_M->DataMapInfo.mmi, rtDataAddrMap);

  /* Cache C-API Data Run-Time Dimension Buffer Addresses into the Real-Time Model Data structure */
  rtwCAPI_SetVarDimsAddressMap(MCU_M->DataMapInfo.mmi, rtVarDimsAddrMap);

  /* Cache the instance C-API logging pointer */
  rtwCAPI_SetInstanceLoggingInfo(MCU_M->DataMapInfo.mmi, (NULL));

  /* Set reference to submodels */
  rtwCAPI_SetChildMMIArray(MCU_M->DataMapInfo.mmi, (NULL));
  rtwCAPI_SetChildMMIArrayLen(MCU_M->DataMapInfo.mmi, 0);
}

#else                                  /* HOST_CAPI_BUILD */
#ifdef __cplusplus

extern "C"
{

#endif

  void MCU_host_InitializeDataMapInfo(MCU_host_DataMapInfo_T *dataMap, const
    char *path)
  {
    /* Set C-API version */
    rtwCAPI_SetVersion(dataMap->mmi, 1);

    /* Cache static C-API data into the Real-time Model Data structure */
    rtwCAPI_SetStaticMap(dataMap->mmi, &mmiStatic);

    /* host data address map is NULL */
    rtwCAPI_SetDataAddressMap(dataMap->mmi, (NULL));

    /* host vardims address map is NULL */
    rtwCAPI_SetVarDimsAddressMap(dataMap->mmi, (NULL));

    /* Set Instance specific path */
    rtwCAPI_SetPath(dataMap->mmi, path);
    rtwCAPI_SetFullPath(dataMap->mmi, (NULL));

    /* Set reference to submodels */
    rtwCAPI_SetChildMMIArray(dataMap->mmi, (NULL));
    rtwCAPI_SetChildMMIArrayLen(dataMap->mmi, 0);
  }

#ifdef __cplusplus

}

#endif
#endif                                 /* HOST_CAPI_BUILD */

/* EOF: MCU_capi.c */
