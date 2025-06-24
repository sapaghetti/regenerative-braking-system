#ifndef CAN_H_
#define CAN_H_

/***********************************************************************/
/*Include*/ 
/***********************************************************************/
#include "Ifx_Types.h"
#include "IfxCpu.h"
#include "IfxMultican_Can.h"
#include "Can_Message.h"
#include "My_Itoa.h"

/***********************************************************************/
/*Define*/ 
/***********************************************************************/
#define CAN0_SRCID IfxMultican_SrcId_0

/***********************************************************************/
/*Typedef*/ 
/***********************************************************************/


typedef struct
{
    struct
    {
        IfxMultican_Can        can;          /**< \brief CAN driver handle */
        IfxMultican_Can_Node   canSrcNode;   /**< \brief CAN Source Node */
        IfxMultican_Can_MsgObj canSrcMsgObj; /**< \brief CAN Source Message object */
    }drivers;
} App_MulticanBasic;


/***********************************************************************/
/*External Variable*/
/***********************************************************************/
IFX_EXTERN App_MulticanBasic g_MulticanBasic;


/***********************************************************************/
/*Global Function Prototype*/
/***********************************************************************/
extern void Driver_Can_Init(void);



typedef struct
{
    IfxMultican_Can        can;          /**< \brief CAN driver handle */
    IfxMultican_Can_Node   canNode;   /**< \brief CAN Source Node */
    IfxMultican_Can_MsgObj canMsgTxObj;
    IfxMultican_Can_MsgObj canMsgRxObj;
} Can_Info;

typedef struct{
    VehicleStatus vehicle_status;
    VehicleControl vehicle_control;
    volatile uint8 vehicle_control_flag;
}Message_Info;

/***********************************************************************/
/*External Variable*/ 
/***********************************************************************/
extern Message_Info g_MessageInfo;
/***********************************************************************/
/*Global Function Prototype*/ 
/***********************************************************************/
void init_message(void);
void init_can(void);

void transmit_message(Message_Info* msgptr, uint32 messageID);
void can_TxTest(void);
uint64 getTargetRpm(void);

#endif /* DRIVER_STM */
