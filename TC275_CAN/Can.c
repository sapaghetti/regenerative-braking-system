/***********************************************************************/
/*Include*/
/***********************************************************************/
#include "Can.h"

/***********************************************************************/
/*Define*/
/***********************************************************************/

/***********************************************************************/
/*Typedef*/
/***********************************************************************/

/***********************************************************************/
/*Static Function Prototype*/
/***********************************************************************/

/***********************************************************************/
/*Variable*/
/***********************************************************************/
Can_Info g_CanInfo; /**< \brief Demo information */
Message_Info g_MessageInfo;
/***********************************************************************/
/*Function*/
/***********************************************************************/
char buffer[64];
int rpm;
uint8 rflag = 0;
IfxMultican_Message readmsg;
#define RPM_SCALE 400

IFX_INTERRUPT(CAN_RxInt0Handler, 0, 9);
void CAN_RxInt0Handler(void)
{
    IfxCpu_enableInterrupts();
    //setMotorDuty(100, 100);

    rflag = 1;
}

void Change_Duty()
{
    IfxMultican_Status readStatus;

    readStatus = IfxMultican_Can_MsgObj_readMessage(&g_CanInfo.canMsgRxObj, &readmsg);

    if (readStatus == IfxMultican_Status_newData)
    {
        switch (readmsg.id)
        {
        case VEHICLE_SPEED:
        {
            rpm = 0;
            rpm |= (uint8)(readmsg.data[0]);
            rpm = (int)(rpm / 10);

            uint32 duty;
            if (rpm == 0)
                duty = 0;
            else
                duty = 65 + ((rpm * (100 - 65)) / RPM_SCALE);

            setMotorDuty(duty, duty); // 좌우 모터 동일하게 설정
            break;
        }
        default:
            break;
        }
    }

    rflag = 0;
}

void init_message(void)
{
    //    g_MessageInfo.vehicle_control.LL = 0;
    memset(&g_MessageInfo, 0, sizeof(g_MessageInfo));
}

void init_can(void)
{
    /* create module config */
    IfxMultican_Can_Config canConfig;
    IfxMultican_Can_initModuleConfig(&canConfig, &MODULE_CAN);

    // CAN0_RX INT
    canConfig.nodePointer[CAN0_SRCID].priority = 9;
    canConfig.nodePointer[CAN0_SRCID].typeOfService = IfxSrc_Tos_cpu0;

    /* initialize module */
    IfxMultican_Can_initModule(&g_CanInfo.can, &canConfig);

    /* create CAN node config */
    IfxMultican_Can_NodeConfig canNodeConfig;
    IfxMultican_Can_Node_initConfig(&canNodeConfig, &g_CanInfo.can);

    canNodeConfig.baudrate = 500000UL; /*500kbps*/
    canNodeConfig.samplePoint = 7500;
    canNodeConfig.nodeId = (IfxMultican_NodeId)((int)IfxMultican_NodeId_0);
    canNodeConfig.rxPin = &IfxMultican_RXD0B_P20_7_IN;
    canNodeConfig.rxPinMode = IfxPort_InputMode_pullUp;
    canNodeConfig.txPin = &IfxMultican_TXD0_P20_8_OUT;
    canNodeConfig.txPinMode = IfxPort_OutputMode_pushPull;

    IfxMultican_Can_Node_init(&g_CanInfo.canNode, &canNodeConfig);

    // TX
    IfxMultican_Can_MsgObjConfig canMsgObjConfig;
    IfxMultican_Can_MsgObj_initConfig(&canMsgObjConfig, &g_CanInfo.canNode);

    canMsgObjConfig.msgObjId = 0;
    canMsgObjConfig.messageId = 0x099;
    canMsgObjConfig.acceptanceMask = 0x7FFFFFFFUL;
    canMsgObjConfig.frame = IfxMultican_Frame_transmit;
    canMsgObjConfig.control.messageLen = IfxMultican_DataLengthCode_8;
    canMsgObjConfig.control.extendedFrame = FALSE;
    canMsgObjConfig.control.matchingId = TRUE;
    IfxMultican_Can_MsgObj_init(&g_CanInfo.canMsgTxObj, &canMsgObjConfig);

    // RX
    canMsgObjConfig.msgObjId = 10U;
    canMsgObjConfig.messageId = 0x100;
    canMsgObjConfig.frame = IfxMultican_Frame_receive;
    canMsgObjConfig.control.extendedFrame = FALSE;

    // CAN0 INT 활성화
    canMsgObjConfig.rxInterrupt.enabled = TRUE;
    canMsgObjConfig.rxInterrupt.srcId = CAN0_SRCID;
    IfxMultican_Can_MsgObj_init(&g_CanInfo.canMsgRxObj, &canMsgObjConfig);
}
