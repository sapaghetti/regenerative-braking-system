/*
 * MCU.h
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

#ifndef MCU_h_
#define MCU_h_
#ifndef MCU_COMMON_INCLUDES_
#define MCU_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "simstruc.h"
#include "fixedpoint.h"
#include "rt_logging.h"
#include "rt_nonfinite.h"
#include "math.h"
#include "cncomm.h"
#endif                                 /* MCU_COMMON_INCLUDES_ */

#include "MCU_types.h"
#include <stddef.h>
#include "rtGetNaN.h"
#include <string.h>
#include "rtw_modelmap.h"
#include "rt_defines.h"
#include "zero_crossing_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetBlockIO
#define rtmGetBlockIO(rtm)             ((rtm)->blockIO)
#endif

#ifndef rtmSetBlockIO
#define rtmSetBlockIO(rtm, val)        ((rtm)->blockIO = (val))
#endif

#ifndef rtmGetChecksums
#define rtmGetChecksums(rtm)           ((rtm)->Sizes.checksums)
#endif

#ifndef rtmSetChecksums
#define rtmSetChecksums(rtm, val)      ((rtm)->Sizes.checksums = (val))
#endif

#ifndef rtmGetConstBlockIO
#define rtmGetConstBlockIO(rtm)        ((rtm)->constBlockIO)
#endif

#ifndef rtmSetConstBlockIO
#define rtmSetConstBlockIO(rtm, val)   ((rtm)->constBlockIO = (val))
#endif

#ifndef rtmGetContStateDisabled
#define rtmGetContStateDisabled(rtm)   ((rtm)->contStateDisabled)
#endif

#ifndef rtmSetContStateDisabled
#define rtmSetContStateDisabled(rtm, val) ((rtm)->contStateDisabled = (val))
#endif

#ifndef rtmGetContStates
#define rtmGetContStates(rtm)          ((rtm)->contStates)
#endif

#ifndef rtmSetContStates
#define rtmSetContStates(rtm, val)     ((rtm)->contStates = (val))
#endif

#ifndef rtmGetContTimeOutputInconsistentWithStateAtMajorStepFlag
#define rtmGetContTimeOutputInconsistentWithStateAtMajorStepFlag(rtm) ((rtm)->CTOutputIncnstWithState)
#endif

#ifndef rtmSetContTimeOutputInconsistentWithStateAtMajorStepFlag
#define rtmSetContTimeOutputInconsistentWithStateAtMajorStepFlag(rtm, val) ((rtm)->CTOutputIncnstWithState = (val))
#endif

#ifndef rtmGetCtrlRateMdlRefTiming
#define rtmGetCtrlRateMdlRefTiming(rtm) ()
#endif

#ifndef rtmSetCtrlRateMdlRefTiming
#define rtmSetCtrlRateMdlRefTiming(rtm, val) ()
#endif

#ifndef rtmGetCtrlRateMdlRefTimingPtr
#define rtmGetCtrlRateMdlRefTimingPtr(rtm) ()
#endif

#ifndef rtmSetCtrlRateMdlRefTimingPtr
#define rtmSetCtrlRateMdlRefTimingPtr(rtm, val) ()
#endif

#ifndef rtmGetCtrlRateNumTicksToNextHit
#define rtmGetCtrlRateNumTicksToNextHit(rtm) ()
#endif

#ifndef rtmSetCtrlRateNumTicksToNextHit
#define rtmSetCtrlRateNumTicksToNextHit(rtm, val) ()
#endif

#ifndef rtmGetDataMapInfo
#define rtmGetDataMapInfo(rtm)         ((rtm)->DataMapInfo)
#endif

#ifndef rtmSetDataMapInfo
#define rtmSetDataMapInfo(rtm, val)    ((rtm)->DataMapInfo = (val))
#endif

#ifndef rtmGetDefaultParam
#define rtmGetDefaultParam(rtm)        ((rtm)->defaultParam)
#endif

#ifndef rtmSetDefaultParam
#define rtmSetDefaultParam(rtm, val)   ((rtm)->defaultParam = (val))
#endif

#ifndef rtmGetDerivCacheNeedsReset
#define rtmGetDerivCacheNeedsReset(rtm) ((rtm)->derivCacheNeedsReset)
#endif

#ifndef rtmSetDerivCacheNeedsReset
#define rtmSetDerivCacheNeedsReset(rtm, val) ((rtm)->derivCacheNeedsReset = (val))
#endif

#ifndef rtmGetDirectFeedThrough
#define rtmGetDirectFeedThrough(rtm)   ((rtm)->Sizes.sysDirFeedThru)
#endif

#ifndef rtmSetDirectFeedThrough
#define rtmSetDirectFeedThrough(rtm, val) ((rtm)->Sizes.sysDirFeedThru = (val))
#endif

#ifndef rtmGetErrorStatusFlag
#define rtmGetErrorStatusFlag(rtm)     ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatusFlag
#define rtmSetErrorStatusFlag(rtm, val) ((rtm)->errorStatus = (val))
#endif

#ifndef rtmGetFinalTime
#define rtmGetFinalTime(rtm)           ((rtm)->Timing.tFinal)
#endif

#ifndef rtmSetFinalTime
#define rtmSetFinalTime(rtm, val)      ((rtm)->Timing.tFinal = (val))
#endif

#ifndef rtmGetFirstInitCondFlag
#define rtmGetFirstInitCondFlag(rtm)   ()
#endif

#ifndef rtmSetFirstInitCondFlag
#define rtmSetFirstInitCondFlag(rtm, val) ()
#endif

#ifndef rtmGetIntgData
#define rtmGetIntgData(rtm)            ()
#endif

#ifndef rtmSetIntgData
#define rtmSetIntgData(rtm, val)       ()
#endif

#ifndef rtmGetMdlRefGlobalRuntimeEventIndices
#define rtmGetMdlRefGlobalRuntimeEventIndices(rtm) ()
#endif

#ifndef rtmSetMdlRefGlobalRuntimeEventIndices
#define rtmSetMdlRefGlobalRuntimeEventIndices(rtm, val) ()
#endif

#ifndef rtmGetMdlRefGlobalTID
#define rtmGetMdlRefGlobalTID(rtm)     ()
#endif

#ifndef rtmSetMdlRefGlobalTID
#define rtmSetMdlRefGlobalTID(rtm, val) ()
#endif

#ifndef rtmGetMdlRefGlobalTimerIndices
#define rtmGetMdlRefGlobalTimerIndices(rtm) ()
#endif

#ifndef rtmSetMdlRefGlobalTimerIndices
#define rtmSetMdlRefGlobalTimerIndices(rtm, val) ()
#endif

#ifndef rtmGetMdlRefTriggerTID
#define rtmGetMdlRefTriggerTID(rtm)    ()
#endif

#ifndef rtmSetMdlRefTriggerTID
#define rtmSetMdlRefTriggerTID(rtm, val) ()
#endif

#ifndef rtmGetModelMappingInfo
#define rtmGetModelMappingInfo(rtm)    ((rtm)->SpecialInfo.mappingInfo)
#endif

#ifndef rtmSetModelMappingInfo
#define rtmSetModelMappingInfo(rtm, val) ((rtm)->SpecialInfo.mappingInfo = (val))
#endif

#ifndef rtmGetModelName
#define rtmGetModelName(rtm)           ((rtm)->modelName)
#endif

#ifndef rtmSetModelName
#define rtmSetModelName(rtm, val)      ((rtm)->modelName = (val))
#endif

#ifndef rtmGetNonInlinedSFcns
#define rtmGetNonInlinedSFcns(rtm)     ((rtm)->NonInlinedSFcns)
#endif

#ifndef rtmSetNonInlinedSFcns
#define rtmSetNonInlinedSFcns(rtm, val) ((rtm)->NonInlinedSFcns = (val))
#endif

#ifndef rtmGetNumBlockIO
#define rtmGetNumBlockIO(rtm)          ((rtm)->Sizes.numBlockIO)
#endif

#ifndef rtmSetNumBlockIO
#define rtmSetNumBlockIO(rtm, val)     ((rtm)->Sizes.numBlockIO = (val))
#endif

#ifndef rtmGetNumBlockParams
#define rtmGetNumBlockParams(rtm)      ((rtm)->Sizes.numBlockPrms)
#endif

#ifndef rtmSetNumBlockParams
#define rtmSetNumBlockParams(rtm, val) ((rtm)->Sizes.numBlockPrms = (val))
#endif

#ifndef rtmGetNumBlocks
#define rtmGetNumBlocks(rtm)           ((rtm)->Sizes.numBlocks)
#endif

#ifndef rtmSetNumBlocks
#define rtmSetNumBlocks(rtm, val)      ((rtm)->Sizes.numBlocks = (val))
#endif

#ifndef rtmGetNumContStates
#define rtmGetNumContStates(rtm)       ((rtm)->Sizes.numContStates)
#endif

#ifndef rtmSetNumContStates
#define rtmSetNumContStates(rtm, val)  ((rtm)->Sizes.numContStates = (val))
#endif

#ifndef rtmGetNumDWork
#define rtmGetNumDWork(rtm)            ((rtm)->Sizes.numDwork)
#endif

#ifndef rtmSetNumDWork
#define rtmSetNumDWork(rtm, val)       ((rtm)->Sizes.numDwork = (val))
#endif

#ifndef rtmGetNumInputPorts
#define rtmGetNumInputPorts(rtm)       ((rtm)->Sizes.numIports)
#endif

#ifndef rtmSetNumInputPorts
#define rtmSetNumInputPorts(rtm, val)  ((rtm)->Sizes.numIports = (val))
#endif

#ifndef rtmGetNumNonSampledZCs
#define rtmGetNumNonSampledZCs(rtm)    ((rtm)->Sizes.numNonSampZCs)
#endif

#ifndef rtmSetNumNonSampledZCs
#define rtmSetNumNonSampledZCs(rtm, val) ((rtm)->Sizes.numNonSampZCs = (val))
#endif

#ifndef rtmGetNumOutputPorts
#define rtmGetNumOutputPorts(rtm)      ((rtm)->Sizes.numOports)
#endif

#ifndef rtmSetNumOutputPorts
#define rtmSetNumOutputPorts(rtm, val) ((rtm)->Sizes.numOports = (val))
#endif

#ifndef rtmGetNumPeriodicContStates
#define rtmGetNumPeriodicContStates(rtm) ((rtm)->Sizes.numPeriodicContStates)
#endif

#ifndef rtmSetNumPeriodicContStates
#define rtmSetNumPeriodicContStates(rtm, val) ((rtm)->Sizes.numPeriodicContStates = (val))
#endif

#ifndef rtmGetNumSFcnParams
#define rtmGetNumSFcnParams(rtm)       ((rtm)->Sizes.numSFcnPrms)
#endif

#ifndef rtmSetNumSFcnParams
#define rtmSetNumSFcnParams(rtm, val)  ((rtm)->Sizes.numSFcnPrms = (val))
#endif

#ifndef rtmGetNumSFunctions
#define rtmGetNumSFunctions(rtm)       ((rtm)->Sizes.numSFcns)
#endif

#ifndef rtmSetNumSFunctions
#define rtmSetNumSFunctions(rtm, val)  ((rtm)->Sizes.numSFcns = (val))
#endif

#ifndef rtmGetNumSampleTimes
#define rtmGetNumSampleTimes(rtm)      ((rtm)->Sizes.numSampTimes)
#endif

#ifndef rtmSetNumSampleTimes
#define rtmSetNumSampleTimes(rtm, val) ((rtm)->Sizes.numSampTimes = (val))
#endif

#ifndef rtmGetNumU
#define rtmGetNumU(rtm)                ((rtm)->Sizes.numU)
#endif

#ifndef rtmSetNumU
#define rtmSetNumU(rtm, val)           ((rtm)->Sizes.numU = (val))
#endif

#ifndef rtmGetNumY
#define rtmGetNumY(rtm)                ((rtm)->Sizes.numY)
#endif

#ifndef rtmSetNumY
#define rtmSetNumY(rtm, val)           ((rtm)->Sizes.numY = (val))
#endif

#ifndef rtmGetOdeF
#define rtmGetOdeF(rtm)                ()
#endif

#ifndef rtmSetOdeF
#define rtmSetOdeF(rtm, val)           ()
#endif

#ifndef rtmGetOdeY
#define rtmGetOdeY(rtm)                ()
#endif

#ifndef rtmSetOdeY
#define rtmSetOdeY(rtm, val)           ()
#endif

#ifndef rtmGetOffsetTimeArray
#define rtmGetOffsetTimeArray(rtm)     ((rtm)->Timing.offsetTimesArray)
#endif

#ifndef rtmSetOffsetTimeArray
#define rtmSetOffsetTimeArray(rtm, val) ((rtm)->Timing.offsetTimesArray = (val))
#endif

#ifndef rtmGetOffsetTimePtr
#define rtmGetOffsetTimePtr(rtm)       ((rtm)->Timing.offsetTimes)
#endif

#ifndef rtmSetOffsetTimePtr
#define rtmSetOffsetTimePtr(rtm, val)  ((rtm)->Timing.offsetTimes = (val))
#endif

#ifndef rtmGetOptions
#define rtmGetOptions(rtm)             ((rtm)->Sizes.options)
#endif

#ifndef rtmSetOptions
#define rtmSetOptions(rtm, val)        ((rtm)->Sizes.options = (val))
#endif

#ifndef rtmGetParamIsMalloced
#define rtmGetParamIsMalloced(rtm)     ()
#endif

#ifndef rtmSetParamIsMalloced
#define rtmSetParamIsMalloced(rtm, val) ()
#endif

#ifndef rtmGetPath
#define rtmGetPath(rtm)                ((rtm)->path)
#endif

#ifndef rtmSetPath
#define rtmSetPath(rtm, val)           ((rtm)->path = (val))
#endif

#ifndef rtmGetPerTaskSampleHits
#define rtmGetPerTaskSampleHits(rtm)   ()
#endif

#ifndef rtmSetPerTaskSampleHits
#define rtmSetPerTaskSampleHits(rtm, val) ()
#endif

#ifndef rtmGetPerTaskSampleHitsArray
#define rtmGetPerTaskSampleHitsArray(rtm) ((rtm)->Timing.perTaskSampleHitsArray)
#endif

#ifndef rtmSetPerTaskSampleHitsArray
#define rtmSetPerTaskSampleHitsArray(rtm, val) ((rtm)->Timing.perTaskSampleHitsArray = (val))
#endif

#ifndef rtmGetPerTaskSampleHitsPtr
#define rtmGetPerTaskSampleHitsPtr(rtm) ((rtm)->Timing.perTaskSampleHits)
#endif

#ifndef rtmSetPerTaskSampleHitsPtr
#define rtmSetPerTaskSampleHitsPtr(rtm, val) ((rtm)->Timing.perTaskSampleHits = (val))
#endif

#ifndef rtmGetPeriodicContStateIndices
#define rtmGetPeriodicContStateIndices(rtm) ((rtm)->periodicContStateIndices)
#endif

#ifndef rtmSetPeriodicContStateIndices
#define rtmSetPeriodicContStateIndices(rtm, val) ((rtm)->periodicContStateIndices = (val))
#endif

#ifndef rtmGetPeriodicContStateRanges
#define rtmGetPeriodicContStateRanges(rtm) ((rtm)->periodicContStateRanges)
#endif

#ifndef rtmSetPeriodicContStateRanges
#define rtmSetPeriodicContStateRanges(rtm, val) ((rtm)->periodicContStateRanges = (val))
#endif

#ifndef rtmGetPrevZCSigState
#define rtmGetPrevZCSigState(rtm)      ((rtm)->prevZCSigState)
#endif

#ifndef rtmSetPrevZCSigState
#define rtmSetPrevZCSigState(rtm, val) ((rtm)->prevZCSigState = (val))
#endif

#ifndef rtmGetProxyFunctions
#define rtmGetProxyFunctions(rtm)      ()
#endif

#ifndef rtmSetProxyFunctions
#define rtmSetProxyFunctions(rtm, val) ()
#endif

#ifndef rtmGetRTWExtModeInfo
#define rtmGetRTWExtModeInfo(rtm)      ((rtm)->extModeInfo)
#endif

#ifndef rtmSetRTWExtModeInfo
#define rtmSetRTWExtModeInfo(rtm, val) ((rtm)->extModeInfo = (val))
#endif

#ifndef rtmGetRTWGeneratedSFcn
#define rtmGetRTWGeneratedSFcn(rtm)    ((rtm)->Sizes.rtwGenSfcn)
#endif

#ifndef rtmSetRTWGeneratedSFcn
#define rtmSetRTWGeneratedSFcn(rtm, val) ((rtm)->Sizes.rtwGenSfcn = (val))
#endif

#ifndef rtmGetRTWLogInfo
#define rtmGetRTWLogInfo(rtm)          ((rtm)->rtwLogInfo)
#endif

#ifndef rtmSetRTWLogInfo
#define rtmSetRTWLogInfo(rtm, val)     ((rtm)->rtwLogInfo = (val))
#endif

#ifndef rtmGetRTWRTModelMethodsInfo
#define rtmGetRTWRTModelMethodsInfo(rtm) ()
#endif

#ifndef rtmSetRTWRTModelMethodsInfo
#define rtmSetRTWRTModelMethodsInfo(rtm, val) ()
#endif

#ifndef rtmGetRTWSfcnInfo
#define rtmGetRTWSfcnInfo(rtm)         ((rtm)->sfcnInfo)
#endif

#ifndef rtmSetRTWSfcnInfo
#define rtmSetRTWSfcnInfo(rtm, val)    ((rtm)->sfcnInfo = (val))
#endif

#ifndef rtmGetRTWSolverInfo
#define rtmGetRTWSolverInfo(rtm)       ((rtm)->solverInfo)
#endif

#ifndef rtmSetRTWSolverInfo
#define rtmSetRTWSolverInfo(rtm, val)  ((rtm)->solverInfo = (val))
#endif

#ifndef rtmGetRTWSolverInfoPtr
#define rtmGetRTWSolverInfoPtr(rtm)    ((rtm)->solverInfoPtr)
#endif

#ifndef rtmSetRTWSolverInfoPtr
#define rtmSetRTWSolverInfoPtr(rtm, val) ((rtm)->solverInfoPtr = (val))
#endif

#ifndef rtmGetReservedForXPC
#define rtmGetReservedForXPC(rtm)      ((rtm)->SpecialInfo.xpcData)
#endif

#ifndef rtmSetReservedForXPC
#define rtmSetReservedForXPC(rtm, val) ((rtm)->SpecialInfo.xpcData = (val))
#endif

#ifndef rtmGetRootDWork
#define rtmGetRootDWork(rtm)           ((rtm)->dwork)
#endif

#ifndef rtmSetRootDWork
#define rtmSetRootDWork(rtm, val)      ((rtm)->dwork = (val))
#endif

#ifndef rtmGetSFunctions
#define rtmGetSFunctions(rtm)          ((rtm)->childSfunctions)
#endif

#ifndef rtmSetSFunctions
#define rtmSetSFunctions(rtm, val)     ((rtm)->childSfunctions = (val))
#endif

#ifndef rtmGetSampleHitArray
#define rtmGetSampleHitArray(rtm)      ((rtm)->Timing.sampleHitArray)
#endif

#ifndef rtmSetSampleHitArray
#define rtmSetSampleHitArray(rtm, val) ((rtm)->Timing.sampleHitArray = (val))
#endif

#ifndef rtmGetSampleHitPtr
#define rtmGetSampleHitPtr(rtm)        ((rtm)->Timing.sampleHits)
#endif

#ifndef rtmSetSampleHitPtr
#define rtmSetSampleHitPtr(rtm, val)   ((rtm)->Timing.sampleHits = (val))
#endif

#ifndef rtmGetSampleTimeArray
#define rtmGetSampleTimeArray(rtm)     ((rtm)->Timing.sampleTimesArray)
#endif

#ifndef rtmSetSampleTimeArray
#define rtmSetSampleTimeArray(rtm, val) ((rtm)->Timing.sampleTimesArray = (val))
#endif

#ifndef rtmGetSampleTimePtr
#define rtmGetSampleTimePtr(rtm)       ((rtm)->Timing.sampleTimes)
#endif

#ifndef rtmSetSampleTimePtr
#define rtmSetSampleTimePtr(rtm, val)  ((rtm)->Timing.sampleTimes = (val))
#endif

#ifndef rtmGetSampleTimeTaskIDArray
#define rtmGetSampleTimeTaskIDArray(rtm) ((rtm)->Timing.sampleTimeTaskIDArray)
#endif

#ifndef rtmSetSampleTimeTaskIDArray
#define rtmSetSampleTimeTaskIDArray(rtm, val) ((rtm)->Timing.sampleTimeTaskIDArray = (val))
#endif

#ifndef rtmGetSampleTimeTaskIDPtr
#define rtmGetSampleTimeTaskIDPtr(rtm) ((rtm)->Timing.sampleTimeTaskIDPtr)
#endif

#ifndef rtmSetSampleTimeTaskIDPtr
#define rtmSetSampleTimeTaskIDPtr(rtm, val) ((rtm)->Timing.sampleTimeTaskIDPtr = (val))
#endif

#ifndef rtmGetSelf
#define rtmGetSelf(rtm)                ()
#endif

#ifndef rtmSetSelf
#define rtmSetSelf(rtm, val)           ()
#endif

#ifndef rtmGetSimMode
#define rtmGetSimMode(rtm)             ((rtm)->simMode)
#endif

#ifndef rtmSetSimMode
#define rtmSetSimMode(rtm, val)        ((rtm)->simMode = (val))
#endif

#ifndef rtmGetSimTimeStep
#define rtmGetSimTimeStep(rtm)         ((rtm)->Timing.simTimeStep)
#endif

#ifndef rtmSetSimTimeStep
#define rtmSetSimTimeStep(rtm, val)    ((rtm)->Timing.simTimeStep = (val))
#endif

#ifndef rtmGetStartTime
#define rtmGetStartTime(rtm)           ((rtm)->Timing.tStart)
#endif

#ifndef rtmSetStartTime
#define rtmSetStartTime(rtm, val)      ((rtm)->Timing.tStart = (val))
#endif

#ifndef rtmGetStepSize
#define rtmGetStepSize(rtm)            ((rtm)->Timing.stepSize)
#endif

#ifndef rtmSetStepSize
#define rtmSetStepSize(rtm, val)       ((rtm)->Timing.stepSize = (val))
#endif

#ifndef rtmGetStopRequestedFlag
#define rtmGetStopRequestedFlag(rtm)   ((rtm)->Timing.stopRequestedFlag)
#endif

#ifndef rtmSetStopRequestedFlag
#define rtmSetStopRequestedFlag(rtm, val) ((rtm)->Timing.stopRequestedFlag = (val))
#endif

#ifndef rtmGetStubFunctions
#define rtmGetStubFunctions(rtm)       ()
#endif

#ifndef rtmSetStubFunctions
#define rtmSetStubFunctions(rtm, val)  ()
#endif

#ifndef rtmGetTaskCounters
#define rtmGetTaskCounters(rtm)        ((rtm)->Timing.TaskCounters)
#endif

#ifndef rtmSetTaskCounters
#define rtmSetTaskCounters(rtm, val)   ((rtm)->Timing.TaskCounters = (val))
#endif

#ifndef rtmGetTaskTimeArray
#define rtmGetTaskTimeArray(rtm)       ((rtm)->Timing.tArray)
#endif

#ifndef rtmSetTaskTimeArray
#define rtmSetTaskTimeArray(rtm, val)  ((rtm)->Timing.tArray = (val))
#endif

#ifndef rtmGetTimePtr
#define rtmGetTimePtr(rtm)             ((rtm)->Timing.t)
#endif

#ifndef rtmSetTimePtr
#define rtmSetTimePtr(rtm, val)        ((rtm)->Timing.t = (val))
#endif

#ifndef rtmGetTimingData
#define rtmGetTimingData(rtm)          ((rtm)->Timing.timingData)
#endif

#ifndef rtmSetTimingData
#define rtmSetTimingData(rtm, val)     ((rtm)->Timing.timingData = (val))
#endif

#ifndef rtmGetU
#define rtmGetU(rtm)                   ((rtm)->inputs)
#endif

#ifndef rtmSetU
#define rtmSetU(rtm, val)              ((rtm)->inputs = (val))
#endif

#ifndef rtmGetVarNextHitTimesListPtr
#define rtmGetVarNextHitTimesListPtr(rtm) ((rtm)->Timing.varNextHitTimesList)
#endif

#ifndef rtmSetVarNextHitTimesListPtr
#define rtmSetVarNextHitTimesListPtr(rtm, val) ((rtm)->Timing.varNextHitTimesList = (val))
#endif

#ifndef rtmGetY
#define rtmGetY(rtm)                   ((rtm)->outputs)
#endif

#ifndef rtmSetY
#define rtmSetY(rtm, val)              ((rtm)->outputs = (val))
#endif

#ifndef rtmGetZCCacheNeedsReset
#define rtmGetZCCacheNeedsReset(rtm)   ((rtm)->zCCacheNeedsReset)
#endif

#ifndef rtmSetZCCacheNeedsReset
#define rtmSetZCCacheNeedsReset(rtm, val) ((rtm)->zCCacheNeedsReset = (val))
#endif

#ifndef rtmGetZCSignalValues
#define rtmGetZCSignalValues(rtm)      ((rtm)->zcSignalValues)
#endif

#ifndef rtmSetZCSignalValues
#define rtmSetZCSignalValues(rtm, val) ((rtm)->zcSignalValues = (val))
#endif

#ifndef rtmGet_TimeOfLastOutput
#define rtmGet_TimeOfLastOutput(rtm)   ((rtm)->Timing.timeOfLastOutput)
#endif

#ifndef rtmSet_TimeOfLastOutput
#define rtmSet_TimeOfLastOutput(rtm, val) ((rtm)->Timing.timeOfLastOutput = (val))
#endif

#ifndef rtmGetdX
#define rtmGetdX(rtm)                  ((rtm)->derivs)
#endif

#ifndef rtmSetdX
#define rtmSetdX(rtm, val)             ((rtm)->derivs = (val))
#endif

#ifndef rtmGettimingBridge
#define rtmGettimingBridge(rtm)        ()
#endif

#ifndef rtmSettimingBridge
#define rtmSettimingBridge(rtm, val)   ()
#endif

#ifndef rtmGetChecksumVal
#define rtmGetChecksumVal(rtm, idx)    ((rtm)->Sizes.checksums[idx])
#endif

#ifndef rtmSetChecksumVal
#define rtmSetChecksumVal(rtm, idx, val) ((rtm)->Sizes.checksums[idx] = (val))
#endif

#ifndef rtmGetDWork
#define rtmGetDWork(rtm, idx)          ((rtm)->dwork[idx])
#endif

#ifndef rtmSetDWork
#define rtmSetDWork(rtm, idx, val)     ((rtm)->dwork[idx] = (val))
#endif

#ifndef rtmGetOffsetTime
#define rtmGetOffsetTime(rtm, idx)     ((rtm)->Timing.offsetTimes[idx])
#endif

#ifndef rtmSetOffsetTime
#define rtmSetOffsetTime(rtm, idx, val) ((rtm)->Timing.offsetTimes[idx] = (val))
#endif

#ifndef rtmGetSFunction
#define rtmGetSFunction(rtm, idx)      ((rtm)->childSfunctions[idx])
#endif

#ifndef rtmSetSFunction
#define rtmSetSFunction(rtm, idx, val) ((rtm)->childSfunctions[idx] = (val))
#endif

#ifndef rtmGetSampleTime
#define rtmGetSampleTime(rtm, idx)     ((rtm)->Timing.sampleTimes[idx])
#endif

#ifndef rtmSetSampleTime
#define rtmSetSampleTime(rtm, idx, val) ((rtm)->Timing.sampleTimes[idx] = (val))
#endif

#ifndef rtmGetSampleTimeTaskID
#define rtmGetSampleTimeTaskID(rtm, idx) ((rtm)->Timing.sampleTimeTaskIDPtr[idx])
#endif

#ifndef rtmSetSampleTimeTaskID
#define rtmSetSampleTimeTaskID(rtm, idx, val) ((rtm)->Timing.sampleTimeTaskIDPtr[idx] = (val))
#endif

#ifndef rtmGetVarNextHitTimeList
#define rtmGetVarNextHitTimeList(rtm, idx) ((rtm)->Timing.varNextHitTimesList[idx])
#endif

#ifndef rtmSetVarNextHitTimeList
#define rtmSetVarNextHitTimeList(rtm, idx, val) ((rtm)->Timing.varNextHitTimesList[idx] = (val))
#endif

#ifndef rtmIsContinuousTask
#define rtmIsContinuousTask(rtm, tid)  0
#endif

#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

#ifndef rtmIsSampleHit
#define rtmIsSampleHit(rtm, sti, tid)  ((rtm)->Timing.sampleHits[(rtm)->Timing.sampleTimeTaskIDPtr[sti]])
#endif

#ifndef rtmGetStopRequested
#define rtmGetStopRequested(rtm)       ((rtm)->Timing.stopRequestedFlag)
#endif

#ifndef rtmSetStopRequested
#define rtmSetStopRequested(rtm, val)  ((rtm)->Timing.stopRequestedFlag = (val))
#endif

#ifndef rtmGetStopRequestedPtr
#define rtmGetStopRequestedPtr(rtm)    (&((rtm)->Timing.stopRequestedFlag))
#endif

#ifndef rtmGetT
#define rtmGetT(rtm)                   (rtmGetTPtr((rtm))[0])
#endif

#ifndef rtmSetT
#define rtmSetT(rtm, val)                                        /* Do Nothing */
#endif

#ifndef rtmGetTFinal
#define rtmGetTFinal(rtm)              ((rtm)->Timing.tFinal)
#endif

#ifndef rtmSetTFinal
#define rtmSetTFinal(rtm, val)         ((rtm)->Timing.tFinal = (val))
#endif

#ifndef rtmGetTPtr
#define rtmGetTPtr(rtm)                ((rtm)->Timing.t)
#endif

#ifndef rtmSetTPtr
#define rtmSetTPtr(rtm, val)           ((rtm)->Timing.t = (val))
#endif

#ifndef rtmGetTStart
#define rtmGetTStart(rtm)              ((rtm)->Timing.tStart)
#endif

#ifndef rtmSetTStart
#define rtmSetTStart(rtm, val)         ((rtm)->Timing.tStart = (val))
#endif

#ifndef rtmGetTaskTime
#define rtmGetTaskTime(rtm, sti)       (rtmGetTPtr((rtm))[(rtm)->Timing.sampleTimeTaskIDPtr[sti]])
#endif

#ifndef rtmSetTaskTime
#define rtmSetTaskTime(rtm, sti, val)  (rtmGetTPtr((rtm))[sti] = (val))
#endif

#ifndef rtmGetTimeOfLastOutput
#define rtmGetTimeOfLastOutput(rtm)    ((rtm)->Timing.timeOfLastOutput)
#endif

#ifdef rtmGetRTWSolverInfo
#undef rtmGetRTWSolverInfo
#endif

#define rtmGetRTWSolverInfo(rtm)       &((rtm)->solverInfo)

/* Definition for use in the target main file */
#define MCU_rtModel                    RT_MODEL_MCU_T

/* Block signals for system '<Root>/Subsystem2' */
typedef struct {
  real_T In1;                          /* '<S7>/In1' */
} B_Subsystem2_MCU_T;

/* Block signals for system '<S16>/Custom Relay' */
typedef struct {
  real_T IndexVector;                  /* '<S45>/Index Vector' */
} B_CustomRelay_MCU_T;

/* Block states (default storage) for system '<S16>/Custom Relay' */
typedef struct {
  real_T Delay_DSTATE;                 /* '<S45>/Delay' */
} DW_CustomRelay_MCU_T;

/* Block signals (default storage) */
typedef struct {
  real_T SFunction;                    /* '<S4>/S-Function' */
  real_T SFunction1_o1;                /* '<S4>/S-Function1' */
  real_T Constant[6];                  /* '<S44>/Constant' */
  real_T VoltageV;                     /* '<S23>/Saturation' */
  real_T Gain;                         /* '<S34>/Gain' */
  real_T Add2[2];                      /* '<S38>/Add2' */
  real_T first2columnsofD[4];          /* '<S32>/first 2 columns of D' */
  real_T Sum[4];                       /* '<S48>/Sum' */
  real_T StateSpace_o1[9];             /* '<S48>/State-Space' */
  real_T StateSpace_o2[6];             /* '<S48>/State-Space' */
  real_T StateSpace_o3;                /* '<S48>/State-Space' */
  real_T SFunction_j;                  /* '<S9>/S-Function' */
  real_T SFunction_m;                  /* '<S10>/S-Function' */
  real_T Add[2];                       /* '<S38>/Add' */
  real_T Gain2;                        /* '<S34>/Gain2' */
  real_T donotdeletethisgain;          /* '<S20>/do not delete this gain' */
  real_T DataTypeConversion2;          /* '<S23>/Data Type Conversion2' */
  real_T CurrentFilter;                /* '<S23>/Current Filter' */
  real_T Add_c;                        /* '<S23>/Add' */
  real_T Add1;                         /* '<S26>/Add1' */
  real_T Gain2_a;                      /* '<S23>/Gain2' */
  real_T IntegralGain;                 /* '<S87>/Integral Gain' */
  real_T DataTypeConversion1;          /* '<S16>/Data Type Conversion1' */
  real_T DataTypeConversion3;          /* '<S16>/Data Type Conversion3' */
  real_T DataTypeConversion5;          /* '<S16>/Data Type Conversion5' */
  int16_T DataTypeConversion14;        /* '<Root>/Data Type Conversion14' */
  int16_T In1;                         /* '<S5>/In1' */
  uint8_T DataTypeConversion;          /* '<Root>/Data Type Conversion' */
  uint8_T In1_g;                       /* '<S6>/In1' */
  B_CustomRelay_MCU_T CustomRelay2;    /* '<S16>/Custom Relay2' */
  B_CustomRelay_MCU_T CustomRelay1;    /* '<S16>/Custom Relay1' */
  B_CustomRelay_MCU_T CustomRelay;     /* '<S16>/Custom Relay' */
  B_Subsystem2_MCU_T Subsystem3;       /* '<Root>/Subsystem3' */
  B_Subsystem2_MCU_T Subsystem2;       /* '<Root>/Subsystem2' */
} B_MCU_T;

/* Block states (default storage) for system '<Root>' */
typedef struct {
  real_T UnitDelay_DSTATE;             /* '<S23>/Unit Delay' */
  real_T UnitDelay_DSTATE_n[2];        /* '<S38>/Unit Delay' */
  real_T DiscreteTimeIntegrator1_DSTATE;/* '<S34>/Discrete-Time Integrator1' */
  real_T DiscreteTimeIntegrator_DSTATE;/* '<S34>/Discrete-Time Integrator' */
  real_T UnitDelay_DSTATE_g[4];        /* '<S48>/Unit Delay' */
  real_T StateSpace_DSTATE;            /* '<S48>/State-Space' */
  real_T CoulombCounter_DSTATE;        /* '<S23>/Coulomb Counter' */
  real_T DiscreteTimeIntegrator_DSTATE_h;/* '<S26>/Discrete-Time Integrator' */
  real_T CurrentFilter_states;         /* '<S23>/Current Filter' */
  real_T Integrator_DSTATE;            /* '<S90>/Integrator' */
  real_T inversion_DWORK4[4];          /* '<S36>/inversion' */
  real_T Initialit_PreviousInput;      /* '<S23>/Initial it' */
  real_T StateSpace_RWORK;             /* '<S48>/State-Space' */
  void *StateSpace_PWORK[65];          /* '<S48>/State-Space' */
  void* SFunction_SysVar;              /* '<S12>/S-Function' */
  void* SFunction_SysVar_k;            /* '<S9>/S-Function' */
  void* SFunction_SysVar_a;            /* '<S10>/S-Function' */
  void* SFunction_SysVar_f;            /* '<S11>/S-Function' */
  void* SFunction_SysVar_m;            /* '<S13>/S-Function' */
  void* SFunction_SysVar_kc;           /* '<S14>/S-Function' */
  int_T StateSpace_IWORK[12];          /* '<S48>/State-Space' */
  int8_T CoulombCounter_PrevResetState;/* '<S23>/Coulomb Counter' */
  uint8_T CoulombCounter_IC_LOADING;   /* '<S23>/Coulomb Counter' */
  DW_CustomRelay_MCU_T CustomRelay2;   /* '<S16>/Custom Relay2' */
  DW_CustomRelay_MCU_T CustomRelay1;   /* '<S16>/Custom Relay1' */
  DW_CustomRelay_MCU_T CustomRelay;    /* '<S16>/Custom Relay' */
} DW_MCU_T;

/* Invariant block signals (default storage) */
typedef struct {
  const real_T SFunction1_o2;          /* '<S4>/S-Function1' */
} ConstB_MCU_T;

/* Backward compatible GRT Identifiers */
#define rtB                            MCU_B
#define BlockIO                        B_MCU_T
#define rtP                            MCU_P
#define Parameters                     P_MCU_T
#define rtDWork                        MCU_DW
#define D_Work                         DW_MCU_T
#define tConstBlockIOType              ConstB_MCU_T
#define rtC                            MCU_ConstB

/* Parameters for system: '<S16>/Custom Relay' */
struct P_CustomRelay_MCU_T_ {
  real_T OFF_Value;                    /* Expression: 0
                                        * Referenced by: '<S45>/OFF'
                                        */
  real_T ON_Value;                     /* Expression: 1
                                        * Referenced by: '<S45>/ON'
                                        */
  real_T offset_Value;                 /* Expression: 1
                                        * Referenced by: '<S45>/offset'
                                        */
  real_T rev_Value;                    /* Expression: -1
                                        * Referenced by: '<S45>/rev'
                                        */
  real_T Delay_InitialCondition;       /* Expression: 0
                                        * Referenced by: '<S45>/Delay'
                                        */
};

/* Parameters (default storage) */
struct P_MCU_T_ {
  real_T DiscretePIDController_I;     /* Mask Parameter: DiscretePIDController_I
                                       * Referenced by: '<S87>/Integral Gain'
                                       */
  real_T DiscretePIDController_InitialCo;
                              /* Mask Parameter: DiscretePIDController_InitialCo
                               * Referenced by: '<S90>/Integrator'
                               */
  real_T DiscretePIDController_P;     /* Mask Parameter: DiscretePIDController_P
                                       * Referenced by: '<S95>/Proportional Gain'
                                       */
  real_T Battery_SOC;                  /* Mask Parameter: Battery_SOC
                                        * Referenced by:
                                        *   '<S23>/Initial it'
                                        *   '<S26>/Discrete-Time Integrator'
                                        */
  real_T CoulombViscousFriction_gain;
                                  /* Mask Parameter: CoulombViscousFriction_gain
                                   * Referenced by: '<S43>/Gain'
                                   */
  real_T CoulombViscousFriction_offset;
                                /* Mask Parameter: CoulombViscousFriction_offset
                                 * Referenced by: '<S43>/Gain1'
                                 */
  real_T AUTO_Value;                   /* Expression: 4
                                        * Referenced by: '<Root>/AUTO'
                                        */
  real_T Regen_Mode1_Value;            /* Expression: 7
                                        * Referenced by: '<Root>/Regen_Mode1'
                                        */
  real_T Regen_Mode2_Value;            /* Expression: 6
                                        * Referenced by: '<Root>/Regen_Mode2'
                                        */
  real_T Regen_Mode3_Value;            /* Expression: 5
                                        * Referenced by: '<Root>/Regen_Mode3'
                                        */
  real_T Integrator_gainval;           /* Computed Parameter: Integrator_gainval
                                        * Referenced by: '<S90>/Integrator'
                                        */
  real_T Constant_Value;               /* Expression: 0
                                        * Referenced by: '<S24>/Constant'
                                        */
  real_T Constant_Value_g;             /* Expression: 0
                                        * Referenced by: '<S25>/Constant'
                                        */
  real_T SFunction_P1_Size[2];         /* Computed Parameter: SFunction_P1_Size
                                        * Referenced by: '<S4>/S-Function'
                                        */
  real_T SFunction_P1;                 /* Expression: 0
                                        * Referenced by: '<S4>/S-Function'
                                        */
  real_T SFunction1_P1;                /* Expression: mode
                                        * Referenced by: '<S4>/S-Function1'
                                        */
  real_T Constant_Value_o[6];          /* Expression: zeros(1,Switches)
                                        * Referenced by: '<S44>/Constant'
                                        */
  real_T UnitDelay_InitialCondition;   /* Expression: 0
                                        * Referenced by: '<S23>/Unit Delay'
                                        */
  real_T Saturation_UpperSat;          /* Expression: 2*Batt.E0
                                        * Referenced by: '<S23>/Saturation'
                                        */
  real_T Saturation_LowerSat;          /* Expression: 0
                                        * Referenced by: '<S23>/Saturation'
                                        */
  real_T I_Value[4];                   /* Expression: eye(2,2)
                                        * Referenced by: '<S32>/I'
                                        */
  real_T u5_Value[4];                  /* Expression: eye(2,2)
                                        * Referenced by: '<S36>/u5'
                                        */
  real_T RL_Gain;                      /* Expression: -PM.R/PM.L
                                        * Referenced by: '<S32>/-R//L'
                                        */
  real_T TustinTs2BETs_Gain;           /* Expression: Ts/PM.DSSmethod
                                        * Referenced by: '<S36>/Tustin: Ts//2 BE : Ts'
                                        */
  real_T wbase3_Gain;                  /* Expression: Ts
                                        * Referenced by: '<S36>/wbase3'
                                        */
  real_T UnitDelay_InitialCondition_k[2];/* Expression: PM.x0_d
                                          * Referenced by: '<S38>/Unit Delay'
                                          */
  real_T M25_Value[10];    /* Expression: [ 2  1   -2  1  1;   -1  1  1  -2  1 ]
                            * Referenced by: '<S32>/M(2,5)'
                            */
  real_T u3L_Gain;                     /* Expression: 1/(3*PM.L)
                                        * Referenced by: '<S32>/1//(3*L)'
                                        */
  real_T Tustin12BE10_Gain;            /* Expression: 1/PM.DSSmethod
                                        * Referenced by: '<S36>/Tustin: 1//2 BE : 1.0'
                                        */
  real_T DiscreteTimeIntegrator1_gainval;
                          /* Computed Parameter: DiscreteTimeIntegrator1_gainval
                           * Referenced by: '<S34>/Discrete-Time Integrator1'
                           */
  real_T DiscreteTimeIntegrator1_IC;   /* Expression: PM.tho+PM.thOffest
                                        * Referenced by: '<S34>/Discrete-Time Integrator1'
                                        */
  real_T Saturation_UpperSat_n;        /* Expression: PM.trap
                                        * Referenced by: '<S39>/Saturation'
                                        */
  real_T Saturation_LowerSat_g;        /* Expression: -PM.trap
                                        * Referenced by: '<S39>/Saturation'
                                        */
  real_T Gain4_Gain;                   /* Expression: PM.Flux/PM.trap
                                        * Referenced by: '<S39>/Gain4'
                                        */
  real_T DiscreteTimeIntegrator_gainval;
                           /* Computed Parameter: DiscreteTimeIntegrator_gainval
                            * Referenced by: '<S34>/Discrete-Time Integrator'
                            */
  real_T DiscreteTimeIntegrator_IC;    /* Expression: PM.wmo
                                        * Referenced by: '<S34>/Discrete-Time Integrator'
                                        */
  real_T Gain_Gain;                    /* Expression: PM.p
                                        * Referenced by: '<S34>/Gain'
                                        */
  real_T Saturation_UpperSat_i;        /* Expression: PM.trap
                                        * Referenced by: '<S40>/Saturation'
                                        */
  real_T Saturation_LowerSat_d;        /* Expression: -PM.trap
                                        * Referenced by: '<S40>/Saturation'
                                        */
  real_T Gain4_Gain_l;                 /* Expression: PM.Flux/PM.trap
                                        * Referenced by: '<S40>/Gain4'
                                        */
  real_T Saturation_UpperSat_f;        /* Expression: PM.trap
                                        * Referenced by: '<S41>/Saturation'
                                        */
  real_T Saturation_LowerSat_f;        /* Expression: -PM.trap
                                        * Referenced by: '<S41>/Saturation'
                                        */
  real_T Gain4_Gain_j;                 /* Expression: PM.Flux/PM.trap
                                        * Referenced by: '<S41>/Gain4'
                                        */
  real_T UnitDelay_InitialCondition_o; /* Expression: 0
                                        * Referenced by: '<S48>/Unit Delay'
                                        */
  real_T StateSpace_P1_Size[2];        /* Computed Parameter: StateSpace_P1_Size
                                        * Referenced by: '<S48>/State-Space'
                                        */
  real_T StateSpace_P1;                /* Expression: S.Ts
                                        * Referenced by: '<S48>/State-Space'
                                        */
  real_T StateSpace_P2_Size[2];        /* Computed Parameter: StateSpace_P2_Size
                                        * Referenced by: '<S48>/State-Space'
                                        */
  real_T StateSpace_P2;                /* Expression: S.A
                                        * Referenced by: '<S48>/State-Space'
                                        */
  real_T StateSpace_P3_Size[2];        /* Computed Parameter: StateSpace_P3_Size
                                        * Referenced by: '<S48>/State-Space'
                                        */
  real_T StateSpace_P3[9];             /* Expression: S.B
                                        * Referenced by: '<S48>/State-Space'
                                        */
  real_T StateSpace_P4_Size[2];        /* Computed Parameter: StateSpace_P4_Size
                                        * Referenced by: '<S48>/State-Space'
                                        */
  real_T StateSpace_P4[9];             /* Expression: S.C
                                        * Referenced by: '<S48>/State-Space'
                                        */
  real_T StateSpace_P5_Size[2];        /* Computed Parameter: StateSpace_P5_Size
                                        * Referenced by: '<S48>/State-Space'
                                        */
  real_T StateSpace_P5[81];            /* Expression: S.D
                                        * Referenced by: '<S48>/State-Space'
                                        */
  real_T StateSpace_P6_Size[2];        /* Computed Parameter: StateSpace_P6_Size
                                        * Referenced by: '<S48>/State-Space'
                                        */
  real_T StateSpace_P6;                /* Expression: S.x0
                                        * Referenced by: '<S48>/State-Space'
                                        */
  real_T StateSpace_P7_Size[2];        /* Computed Parameter: StateSpace_P7_Size
                                        * Referenced by: '<S48>/State-Space'
                                        */
  real_T StateSpace_P7;                /* Expression: S.EnableUseOfTLC
                                        * Referenced by: '<S48>/State-Space'
                                        */
  real_T StateSpace_P8_Size[2];        /* Computed Parameter: StateSpace_P8_Size
                                        * Referenced by: '<S48>/State-Space'
                                        */
  real_T StateSpace_P8[3];             /* Expression: S.NonLinearDim
                                        * Referenced by: '<S48>/State-Space'
                                        */
  real_T StateSpace_P9_Size[2];        /* Computed Parameter: StateSpace_P9_Size
                                        * Referenced by: '<S48>/State-Space'
                                        */
  real_T StateSpace_P10_Size[2];      /* Computed Parameter: StateSpace_P10_Size
                                       * Referenced by: '<S48>/State-Space'
                                       */
  real_T StateSpace_P10[2];            /* Expression: S.NonLinear_Inputs
                                        * Referenced by: '<S48>/State-Space'
                                        */
  real_T StateSpace_P11_Size[2];      /* Computed Parameter: StateSpace_P11_Size
                                       * Referenced by: '<S48>/State-Space'
                                       */
  real_T StateSpace_P11[2];            /* Expression: S.NonLinear_Outputs
                                        * Referenced by: '<S48>/State-Space'
                                        */
  real_T StateSpace_P12_Size[2];      /* Computed Parameter: StateSpace_P12_Size
                                       * Referenced by: '<S48>/State-Space'
                                       */
  real_T StateSpace_P12[3];            /* Expression: S.NonLinear_InitialOutputs
                                        * Referenced by: '<S48>/State-Space'
                                        */
  real_T StateSpace_P13_Size[2];      /* Computed Parameter: StateSpace_P13_Size
                                       * Referenced by: '<S48>/State-Space'
                                       */
  real_T StateSpace_P13;               /* Expression: S.NonLinearIterative
                                        * Referenced by: '<S48>/State-Space'
                                        */
  real_T StateSpace_P14_Size[2];      /* Computed Parameter: StateSpace_P14_Size
                                       * Referenced by: '<S48>/State-Space'
                                       */
  real_T StateSpace_P14;               /* Expression: S.NonLinear_SizeVI
                                        * Referenced by: '<S48>/State-Space'
                                        */
  real_T StateSpace_P15_Size[2];      /* Computed Parameter: StateSpace_P15_Size
                                       * Referenced by: '<S48>/State-Space'
                                       */
  real_T StateSpace_P16_Size[2];      /* Computed Parameter: StateSpace_P16_Size
                                       * Referenced by: '<S48>/State-Space'
                                       */
  real_T StateSpace_P16;               /* Expression: S.NonLinear_Method
                                        * Referenced by: '<S48>/State-Space'
                                        */
  real_T StateSpace_P17_Size[2];      /* Computed Parameter: StateSpace_P17_Size
                                       * Referenced by: '<S48>/State-Space'
                                       */
  real_T StateSpace_P17;               /* Expression: S.Nonlinear_Tolerance
                                        * Referenced by: '<S48>/State-Space'
                                        */
  real_T StateSpace_P18_Size[2];      /* Computed Parameter: StateSpace_P18_Size
                                       * Referenced by: '<S48>/State-Space'
                                       */
  real_T StateSpace_P18;               /* Expression: double(nMaxIteration)
                                        * Referenced by: '<S48>/State-Space'
                                        */
  real_T StateSpace_P19_Size[2];      /* Computed Parameter: StateSpace_P19_Size
                                       * Referenced by: '<S48>/State-Space'
                                       */
  real_T StateSpace_P19;           /* Expression: double(ContinueOnMaxIteration)
                                    * Referenced by: '<S48>/State-Space'
                                    */
  real_T StateSpace_P20_Size[2];      /* Computed Parameter: StateSpace_P20_Size
                                       * Referenced by: '<S48>/State-Space'
                                       */
  real_T StateSpace_P20[6];            /* Expression: S.SwitchResistance
                                        * Referenced by: '<S48>/State-Space'
                                        */
  real_T StateSpace_P21_Size[2];      /* Computed Parameter: StateSpace_P21_Size
                                       * Referenced by: '<S48>/State-Space'
                                       */
  real_T StateSpace_P21[6];            /* Expression: S.SwitchType
                                        * Referenced by: '<S48>/State-Space'
                                        */
  real_T StateSpace_P22_Size[2];      /* Computed Parameter: StateSpace_P22_Size
                                       * Referenced by: '<S48>/State-Space'
                                       */
  real_T StateSpace_P22[6];            /* Expression: S.SwitchGateInitialValue
                                        * Referenced by: '<S48>/State-Space'
                                        */
  real_T StateSpace_P23_Size[2];      /* Computed Parameter: StateSpace_P23_Size
                                       * Referenced by: '<S48>/State-Space'
                                       */
  real_T StateSpace_P23[6];            /* Expression: S.OutputsToResetToZero
                                        * Referenced by: '<S48>/State-Space'
                                        */
  real_T StateSpace_P24_Size[2];      /* Computed Parameter: StateSpace_P24_Size
                                       * Referenced by: '<S48>/State-Space'
                                       */
  real_T StateSpace_P24;               /* Expression: double(S.TBEON)
                                        * Referenced by: '<S48>/State-Space'
                                        */
  real_T Gain2_Gain;                   /* Expression: 30/pi
                                        * Referenced by: '<Root>/Gain2'
                                        */
  real_T Constant_Value_h;             /* Expression: 5
                                        * Referenced by: '<Root>/Constant'
                                        */
  real_T rad2deg_Gain;                 /* Expression: 180/pi
                                        * Referenced by: '<S42>/rad2deg'
                                        */
  real_T Constant4_Value;              /* Expression: -180
                                        * Referenced by: '<S37>/Constant4'
                                        */
  real_T Constant5_Value;              /* Expression: 0
                                        * Referenced by: '<S37>/Constant5'
                                        */
  real_T BE1Tustin12_Gain;             /* Expression: 1/PM.DSSmethod
                                        * Referenced by: '<S38>/BE=1 Tustin=1//2'
                                        */
  real_T Ts_Gain;                      /* Expression: Ts
                                        * Referenced by: '<S38>/Ts'
                                        */
  real_T Constant_Value_l;             /* Expression: -60
                                        * Referenced by: '<S37>/Constant'
                                        */
  real_T Constant1_Value;              /* Expression: 120
                                        * Referenced by: '<S37>/Constant1'
                                        */
  real_T Constant2_Value;              /* Expression: 60
                                        * Referenced by: '<S37>/Constant2'
                                        */
  real_T Constant3_Value;              /* Expression: -120
                                        * Referenced by: '<S37>/Constant3'
                                        */
  real_T Gain3_Gain;                   /* Expression: PM.p
                                        * Referenced by: '<S32>/Gain3'
                                        */
  real_T Gain2_Gain_l;                 /* Expression: 1/PM.J
                                        * Referenced by: '<S34>/Gain2'
                                        */
  real_T capacity_Value;               /* Expression: Batt.Q
                                        * Referenced by: '<S23>/capacity'
                                        */
  real_T donotdeletethisgain_Gain;     /* Expression: 1
                                        * Referenced by: '<S20>/do not delete this gain'
                                        */
  real_T CoulombCounter_gainval;   /* Computed Parameter: CoulombCounter_gainval
                                    * Referenced by: '<S23>/Coulomb Counter'
                                    */
  real_T CoulombCounter_UpperSat;      /* Expression: Batt.Q*3600
                                        * Referenced by: '<S23>/Coulomb Counter'
                                        */
  real_T CoulombCounter_LowerSat;      /* Expression: -inf
                                        * Referenced by: '<S23>/Coulomb Counter'
                                        */
  real_T Gain1_Gain;                   /* Expression: 1/3600
                                        * Referenced by: '<S23>/Gain1'
                                        */
  real_T Constant1_Value_f;            /* Expression: Batt.kcsat
                                        * Referenced by: '<S23>/Constant1'
                                        */
  real_T Gain1_Gain_c;                 /* Expression: 100
                                        * Referenced by: '<S28>/Gain1'
                                        */
  real_T Saturation_UpperSat_d;        /* Expression: 100
                                        * Referenced by: '<S28>/Saturation'
                                        */
  real_T Saturation_LowerSat_e;        /* Expression: 0
                                        * Referenced by: '<S28>/Saturation'
                                        */
  real_T DiscreteTimeIntegrator_gainva_b;
                          /* Computed Parameter: DiscreteTimeIntegrator_gainva_b
                           * Referenced by: '<S26>/Discrete-Time Integrator'
                           */
  real_T CurrentFilter_NumCoef;        /* Expression: [1-exp(-Ts/(Batt_Tr/3))]
                                        * Referenced by: '<S23>/Current Filter'
                                        */
  real_T CurrentFilter_DenCoef[2];     /* Expression: [1 -exp(-Ts/(Batt_Tr/3))]
                                        * Referenced by: '<S23>/Current Filter'
                                        */
  real_T CurrentFilter_InitialStates;  /* Expression: 0
                                        * Referenced by: '<S23>/Current Filter'
                                        */
  real_T constantK_Value;              /* Expression: Batt.K
                                        * Referenced by: '<S23>/constantK'
                                        */
  real_T constantA_Value;              /* Expression: Batt.A
                                        * Referenced by: '<S23>/constantA'
                                        */
  real_T constantB_Value;              /* Expression: Batt.B
                                        * Referenced by: '<S23>/constantB'
                                        */
  real_T Gain_Gain_e;                  /* Expression: Batt.R
                                        * Referenced by: '<S23>/Gain'
                                        */
  real_T Constant_Value_f;             /* Expression: Batt.E0
                                        * Referenced by: '<S23>/Constant'
                                        */
  real_T Gain_Gain_k;                  /* Expression: Batt.B/3600
                                        * Referenced by: '<S26>/Gain'
                                        */
  real_T Gain1_Gain_g;                 /* Expression: Batt.A
                                        * Referenced by: '<S26>/Gain1'
                                        */
  real_T Gain2_Gain_i;                 /* Expression: 3600
                                        * Referenced by: '<S23>/Gain2'
                                        */
  boolean_T u1_Value;                  /* Expression: PM.DSSmethod==2
                                        * Referenced by: '<S36>/u1'
                                        */
  P_CustomRelay_MCU_T CustomRelay2;    /* '<S16>/Custom Relay2' */
  P_CustomRelay_MCU_T CustomRelay1;    /* '<S16>/Custom Relay1' */
  P_CustomRelay_MCU_T CustomRelay;     /* '<S16>/Custom Relay' */
};

/* Real-time Model Data Structure */
struct tag_RTM_MCU_T {
  const char_T *path;
  const char_T *modelName;
  struct SimStruct_tag * *childSfunctions;
  const char_T *errorStatus;
  SS_SimMode simMode;
  RTWLogInfo *rtwLogInfo;
  RTWExtModeInfo *extModeInfo;
  RTWSolverInfo solverInfo;
  RTWSolverInfo *solverInfoPtr;
  void *sfcnInfo;

  /*
   * NonInlinedSFcns:
   * The following substructure contains information regarding
   * non-inlined s-functions used in the model.
   */
  struct {
    RTWSfcnInfo sfcnInfo;
    time_T *taskTimePtrs[2];
    SimStruct childSFunctions[2];
    SimStruct *childSFunctionPtrs[2];
    struct _ssBlkInfo2 blkInfo2[2];
    struct _ssSFcnModelMethods2 methods2[2];
    struct _ssSFcnModelMethods3 methods3[2];
    struct _ssSFcnModelMethods4 methods4[2];
    struct _ssStatesInfo2 statesInfo2[2];
    ssPeriodicStatesInfo periodicStatesInfo[2];
    struct _ssPortInfo2 inputOutputPortInfo2[2];
    struct {
      time_T sfcnPeriod[1];
      time_T sfcnOffset[1];
      int_T sfcnTsMap[1];
      struct _ssPortOutputs outputPortInfo[1];
      struct _ssOutPortUnit outputPortUnits[1];
      struct _ssOutPortCoSimAttribute outputPortCoSimAttribute[1];
      uint_T attribs[1];
      mxArray *params[1];
    } Sfcn0;

    struct {
      time_T sfcnPeriod[1];
      time_T sfcnOffset[1];
      int_T sfcnTsMap[1];
      struct _ssPortInputs inputPortInfo[3];
      struct _ssInPortUnit inputPortUnits[3];
      struct _ssInPortCoSimAttribute inputPortCoSimAttribute[3];
      real_T const *UPtrs0[9];
      real_T const *UPtrs1[6];
      real_T const *UPtrs2[4];
      struct _ssPortOutputs outputPortInfo[3];
      struct _ssOutPortUnit outputPortUnits[3];
      struct _ssOutPortCoSimAttribute outputPortCoSimAttribute[3];
      uint_T attribs[24];
      mxArray *params[24];
      struct _ssDWorkRecord dWork[4];
      struct _ssDWorkAuxRecord dWorkAux[4];
    } Sfcn1;
  } NonInlinedSFcns;

  void *blockIO;
  const void *constBlockIO;
  void *defaultParam;
  ZCSigState *prevZCSigState;
  real_T *contStates;
  int_T *periodicContStateIndices;
  real_T *periodicContStateRanges;
  real_T *derivs;
  void *zcSignalValues;
  void *inputs;
  void *outputs;
  boolean_T *contStateDisabled;
  boolean_T zCCacheNeedsReset;
  boolean_T derivCacheNeedsReset;
  boolean_T CTOutputIncnstWithState;
  void *dwork;

  /*
   * DataMapInfo:
   * The following substructure contains information regarding
   * structures generated in the model's C API.
   */
  struct {
    rtwCAPI_ModelMappingInfo mmi;
  } DataMapInfo;

  /*
   * Sizes:
   * The following substructure contains sizes information
   * for many of the model attributes such as inputs, outputs,
   * dwork, sample times, etc.
   */
  struct {
    uint32_T checksums[4];
    uint32_T options;
    int_T numContStates;
    int_T numPeriodicContStates;
    int_T numU;
    int_T numY;
    int_T numSampTimes;
    int_T numBlocks;
    int_T numBlockIO;
    int_T numBlockPrms;
    int_T numDwork;
    int_T numSFcnPrms;
    int_T numSFcns;
    int_T numIports;
    int_T numOports;
    int_T numNonSampZCs;
    int_T sysDirFeedThru;
    int_T rtwGenSfcn;
  } Sizes;

  /*
   * SpecialInfo:
   * The following substructure contains special information
   * related to other components that are dependent on RTW.
   */
  struct {
    const void *mappingInfo;
    void *xpcData;
  } SpecialInfo;

  /*
   * Timing:
   * The following substructure contains information regarding
   * the timing information for the model.
   */
  struct {
    time_T stepSize;
    uint32_T clockTick0;
    uint32_T clockTickH0;
    time_T stepSize0;
    uint32_T clockTick1;
    uint32_T clockTickH1;
    time_T stepSize1;
    struct {
      uint16_T TID[2];
    } TaskCounters;

    time_T tStart;
    time_T tFinal;
    time_T timeOfLastOutput;
    void *timingData;
    real_T *varNextHitTimesList;
    SimTimeStep simTimeStep;
    boolean_T stopRequestedFlag;
    time_T *sampleTimes;
    time_T *offsetTimes;
    int_T *sampleTimeTaskIDPtr;
    int_T *sampleHits;
    int_T *perTaskSampleHits;
    time_T *t;
    time_T sampleTimesArray[2];
    time_T offsetTimesArray[2];
    int_T sampleTimeTaskIDArray[2];
    int_T sampleHitArray[2];
    int_T perTaskSampleHitsArray[4];
    time_T tArray[2];
  } Timing;
};

/* Block parameters (default storage) */
extern P_MCU_T MCU_P;

/* Block signals (default storage) */
extern B_MCU_T MCU_B;

/* Block states (default storage) */
extern DW_MCU_T MCU_DW;
extern const ConstB_MCU_T MCU_ConstB;  /* constant block i/o */

/*====================*
 * External functions *
 *====================*/
extern MCU_rtModel *MCU(void);
extern void MdlInitializeSizes(void);
extern void MdlInitializeSampleTimes(void);
extern void MdlInitialize(void);
extern void MdlStart(void);
extern void MdlOutputs(int_T tid);
extern void MdlUpdate(int_T tid);
extern void MdlTerminate(void);

/* Function to get C API Model Mapping Static Info */
extern const rtwCAPI_ModelMappingStaticInfo*
  MCU_GetCAPIStaticMap(void);

/* Real-time Model object */
extern RT_MODEL_MCU_T *const MCU_M;

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'MCU'
 * '<S1>'   : 'MCU/Battery'
 * '<S2>'   : 'MCU/Decoder'
 * '<S3>'   : 'MCU/Permanent Magnet Synchronous Machine'
 * '<S4>'   : 'MCU/Simulation Step'
 * '<S5>'   : 'MCU/Subsystem'
 * '<S6>'   : 'MCU/Subsystem1'
 * '<S7>'   : 'MCU/Subsystem2'
 * '<S8>'   : 'MCU/Subsystem3'
 * '<S9>'   : 'MCU/System Variable Input'
 * '<S10>'  : 'MCU/System Variable Input1'
 * '<S11>'  : 'MCU/System Variable Output'
 * '<S12>'  : 'MCU/System Variable Output1'
 * '<S13>'  : 'MCU/System Variable Output2'
 * '<S14>'  : 'MCU/System Variable Output3'
 * '<S15>'  : 'MCU/Universal Bridge1'
 * '<S16>'  : 'MCU/current controller'
 * '<S17>'  : 'MCU/powergui'
 * '<S18>'  : 'MCU/speed controller'
 * '<S19>'  : 'MCU/Battery/Controlled Voltage Source'
 * '<S20>'  : 'MCU/Battery/Current Measurement'
 * '<S21>'  : 'MCU/Battery/Model'
 * '<S22>'  : 'MCU/Battery/Current Measurement/Model'
 * '<S23>'  : 'MCU/Battery/Model/Discrete'
 * '<S24>'  : 'MCU/Battery/Model/Discrete/Compare To Zero'
 * '<S25>'  : 'MCU/Battery/Model/Discrete/Compare To Zero1'
 * '<S26>'  : 'MCU/Battery/Model/Discrete/Exponential Zone Voltage'
 * '<S27>'  : 'MCU/Battery/Model/Discrete/MATLAB Function'
 * '<S28>'  : 'MCU/Battery/Model/Discrete/SoC'
 * '<S29>'  : 'MCU/Battery/Model/Discrete/Voltage Loss Model'
 * '<S30>'  : 'MCU/Battery/Model/Discrete/Voltage Loss Model/Lithium Ion'
 * '<S31>'  : 'MCU/Battery/Model/Discrete/Voltage Loss Model/Lithium Ion/MATLAB Function'
 * '<S32>'  : 'MCU/Permanent Magnet Synchronous Machine/Electrical model'
 * '<S33>'  : 'MCU/Permanent Magnet Synchronous Machine/Measurements'
 * '<S34>'  : 'MCU/Permanent Magnet Synchronous Machine/Mechanical model'
 * '<S35>'  : 'MCU/Permanent Magnet Synchronous Machine/Electrical model/BEMF, Flux'
 * '<S36>'  : 'MCU/Permanent Magnet Synchronous Machine/Electrical model/Discretize  TBE or Tustin'
 * '<S37>'  : 'MCU/Permanent Magnet Synchronous Machine/Electrical model/Hall effect sensor'
 * '<S38>'  : 'MCU/Permanent Magnet Synchronous Machine/Electrical model/Ihistory_States'
 * '<S39>'  : 'MCU/Permanent Magnet Synchronous Machine/Electrical model/BEMF, Flux/Phase a'
 * '<S40>'  : 'MCU/Permanent Magnet Synchronous Machine/Electrical model/BEMF, Flux/Phase b'
 * '<S41>'  : 'MCU/Permanent Magnet Synchronous Machine/Electrical model/BEMF, Flux/Phase c'
 * '<S42>'  : 'MCU/Permanent Magnet Synchronous Machine/Electrical model/Hall effect sensor/Angle converter'
 * '<S43>'  : 'MCU/Permanent Magnet Synchronous Machine/Mechanical model/Coulomb & Viscous Friction'
 * '<S44>'  : 'MCU/Universal Bridge1/Model'
 * '<S45>'  : 'MCU/current controller/Custom Relay'
 * '<S46>'  : 'MCU/current controller/Custom Relay1'
 * '<S47>'  : 'MCU/current controller/Custom Relay2'
 * '<S48>'  : 'MCU/powergui/EquivalentModel1'
 * '<S49>'  : 'MCU/powergui/EquivalentModel1/DSS in'
 * '<S50>'  : 'MCU/powergui/EquivalentModel1/DSS out'
 * '<S51>'  : 'MCU/powergui/EquivalentModel1/Gates'
 * '<S52>'  : 'MCU/powergui/EquivalentModel1/Sources'
 * '<S53>'  : 'MCU/powergui/EquivalentModel1/Status'
 * '<S54>'  : 'MCU/powergui/EquivalentModel1/Yout'
 * '<S55>'  : 'MCU/speed controller/Discrete PID Controller'
 * '<S56>'  : 'MCU/speed controller/Discrete PID Controller/Anti-windup'
 * '<S57>'  : 'MCU/speed controller/Discrete PID Controller/D Gain'
 * '<S58>'  : 'MCU/speed controller/Discrete PID Controller/External Derivative'
 * '<S59>'  : 'MCU/speed controller/Discrete PID Controller/Filter'
 * '<S60>'  : 'MCU/speed controller/Discrete PID Controller/Filter ICs'
 * '<S61>'  : 'MCU/speed controller/Discrete PID Controller/I Gain'
 * '<S62>'  : 'MCU/speed controller/Discrete PID Controller/Ideal P Gain'
 * '<S63>'  : 'MCU/speed controller/Discrete PID Controller/Ideal P Gain Fdbk'
 * '<S64>'  : 'MCU/speed controller/Discrete PID Controller/Integrator'
 * '<S65>'  : 'MCU/speed controller/Discrete PID Controller/Integrator ICs'
 * '<S66>'  : 'MCU/speed controller/Discrete PID Controller/N Copy'
 * '<S67>'  : 'MCU/speed controller/Discrete PID Controller/N Gain'
 * '<S68>'  : 'MCU/speed controller/Discrete PID Controller/P Copy'
 * '<S69>'  : 'MCU/speed controller/Discrete PID Controller/Parallel P Gain'
 * '<S70>'  : 'MCU/speed controller/Discrete PID Controller/Reset Signal'
 * '<S71>'  : 'MCU/speed controller/Discrete PID Controller/Saturation'
 * '<S72>'  : 'MCU/speed controller/Discrete PID Controller/Saturation Fdbk'
 * '<S73>'  : 'MCU/speed controller/Discrete PID Controller/Sum'
 * '<S74>'  : 'MCU/speed controller/Discrete PID Controller/Sum Fdbk'
 * '<S75>'  : 'MCU/speed controller/Discrete PID Controller/Tracking Mode'
 * '<S76>'  : 'MCU/speed controller/Discrete PID Controller/Tracking Mode Sum'
 * '<S77>'  : 'MCU/speed controller/Discrete PID Controller/Tsamp - Integral'
 * '<S78>'  : 'MCU/speed controller/Discrete PID Controller/Tsamp - Ngain'
 * '<S79>'  : 'MCU/speed controller/Discrete PID Controller/postSat Signal'
 * '<S80>'  : 'MCU/speed controller/Discrete PID Controller/preInt Signal'
 * '<S81>'  : 'MCU/speed controller/Discrete PID Controller/preSat Signal'
 * '<S82>'  : 'MCU/speed controller/Discrete PID Controller/Anti-windup/Passthrough'
 * '<S83>'  : 'MCU/speed controller/Discrete PID Controller/D Gain/Disabled'
 * '<S84>'  : 'MCU/speed controller/Discrete PID Controller/External Derivative/Disabled'
 * '<S85>'  : 'MCU/speed controller/Discrete PID Controller/Filter/Disabled'
 * '<S86>'  : 'MCU/speed controller/Discrete PID Controller/Filter ICs/Disabled'
 * '<S87>'  : 'MCU/speed controller/Discrete PID Controller/I Gain/Internal Parameters'
 * '<S88>'  : 'MCU/speed controller/Discrete PID Controller/Ideal P Gain/Passthrough'
 * '<S89>'  : 'MCU/speed controller/Discrete PID Controller/Ideal P Gain Fdbk/Disabled'
 * '<S90>'  : 'MCU/speed controller/Discrete PID Controller/Integrator/Discrete'
 * '<S91>'  : 'MCU/speed controller/Discrete PID Controller/Integrator ICs/Internal IC'
 * '<S92>'  : 'MCU/speed controller/Discrete PID Controller/N Copy/Disabled wSignal Specification'
 * '<S93>'  : 'MCU/speed controller/Discrete PID Controller/N Gain/Disabled'
 * '<S94>'  : 'MCU/speed controller/Discrete PID Controller/P Copy/Disabled'
 * '<S95>'  : 'MCU/speed controller/Discrete PID Controller/Parallel P Gain/Internal Parameters'
 * '<S96>'  : 'MCU/speed controller/Discrete PID Controller/Reset Signal/Disabled'
 * '<S97>'  : 'MCU/speed controller/Discrete PID Controller/Saturation/Passthrough'
 * '<S98>'  : 'MCU/speed controller/Discrete PID Controller/Saturation Fdbk/Disabled'
 * '<S99>'  : 'MCU/speed controller/Discrete PID Controller/Sum/Sum_PI'
 * '<S100>' : 'MCU/speed controller/Discrete PID Controller/Sum Fdbk/Disabled'
 * '<S101>' : 'MCU/speed controller/Discrete PID Controller/Tracking Mode/Disabled'
 * '<S102>' : 'MCU/speed controller/Discrete PID Controller/Tracking Mode Sum/Passthrough'
 * '<S103>' : 'MCU/speed controller/Discrete PID Controller/Tsamp - Integral/TsSignalSpecification'
 * '<S104>' : 'MCU/speed controller/Discrete PID Controller/Tsamp - Ngain/Passthrough'
 * '<S105>' : 'MCU/speed controller/Discrete PID Controller/postSat Signal/Forward_Path'
 * '<S106>' : 'MCU/speed controller/Discrete PID Controller/preInt Signal/Internal PreInt'
 * '<S107>' : 'MCU/speed controller/Discrete PID Controller/preSat Signal/Forward_Path'
 */
#endif                                 /* MCU_h_ */
