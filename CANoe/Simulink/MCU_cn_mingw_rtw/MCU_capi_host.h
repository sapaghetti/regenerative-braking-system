#ifndef MCU_cap_host_h__
#define MCU_cap_host_h__
#ifdef HOST_CAPI_BUILD
#include "rtw_capi.h"
#include "rtw_modelmap.h"

typedef struct {
  rtwCAPI_ModelMappingInfo mmi;
} MCU_host_DataMapInfo_T;

#ifdef __cplusplus

extern "C"
{

#endif

  void MCU_host_InitializeDataMapInfo(MCU_host_DataMapInfo_T *dataMap, const
    char *path);

#ifdef __cplusplus

}

#endif
#endif                                 /* HOST_CAPI_BUILD */
#endif                                 /* MCU_cap_host_h__ */

/* EOF: MCU_capi_host.h */
