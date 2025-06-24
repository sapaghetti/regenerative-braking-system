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
uint64 rpm;
#define RPM_SCALE 3000

//uint64 getTargetRpm(void)
//{
//    return (rpm * 280u) / 3000u;
//}

IFX_INTERRUPT(CAN_RxInt0Handler, 0, 1);
void CAN_RxInt0Handler (void)
{
    IfxMultican_Status readStatus;

    IfxCpu_enableInterrupts();
    IfxMultican_Message readmsg;
    /*
     * readmsg.data[1]: 상위 4byte
     * readmsg.data[0]: 하위 4byte
     */

    readStatus = IfxMultican_Can_MsgObj_readMessage(&g_CanInfo.canMsgRxObj, &readmsg);

    if (readStatus == IfxMultican_Status_newData)
    {
        switch(readmsg.id){
            case VEHICLE_SPEED:
            {
                rpm = ((uint64)readmsg.data[1] << 8) | (uint64)readmsg.data[0];
                my_itoa_hex(rpm, buffer);
                Ifx_Console_print("Received: 0x");
                Ifx_Console_print(buffer);
                Ifx_Console_print("\r\n");

//                 RPM → duty 변환: 0~3000 → 65~100
               uint32 duty;
               if (rpm == 0) duty = 0;
               else duty = 65 + ((rpm * (100 - 65)) / RPM_SCALE);

               setMotorDuty(duty, duty);  // 좌우 모터 동일하게 설정
               break;
            }
            default:
                break;
        }
    }
}

void init_message(void){
//    g_MessageInfo.vehicle_control.LL = 0;
    memset(&g_MessageInfo, 0, sizeof(g_MessageInfo));
}

void init_can (void)
{
    /* create module config */
    IfxMultican_Can_Config canConfig;
    IfxMultican_Can_initModuleConfig(&canConfig, &MODULE_CAN);

    // CAN0_RX INT
    canConfig.nodePointer[CAN0_SRCID].priority = 1;
    canConfig.nodePointer[CAN0_SRCID].typeOfService = IfxSrc_Tos_cpu0;

    /* initialize module */
    IfxMultican_Can_initModule(&g_CanInfo.can, &canConfig);

    /* create CAN node config */
    IfxMultican_Can_NodeConfig canNodeConfig;
    IfxMultican_Can_Node_initConfig(&canNodeConfig, &g_CanInfo.can);

    canNodeConfig.baudrate = 500000UL; /*500kbps*/
    canNodeConfig.nodeId = (IfxMultican_NodeId) ((int) IfxMultican_NodeId_0);
    canNodeConfig.rxPin = &IfxMultican_RXD0B_P20_7_IN;
    canNodeConfig.rxPinMode = IfxPort_InputMode_pullUp;
    canNodeConfig.txPin = &IfxMultican_TXD0_P20_8_OUT;
    canNodeConfig.txPinMode = IfxPort_OutputMode_pushPull;

    IfxMultican_Can_Node_init(&g_CanInfo.canNode, &canNodeConfig);

    // TX
    IfxMultican_Can_MsgObjConfig canMsgObjConfig;
    IfxMultican_Can_MsgObj_initConfig(&canMsgObjConfig, &g_CanInfo.canNode);

    canMsgObjConfig.msgObjId = 0;
    canMsgObjConfig.messageId = 0x200;
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
//    SRC_CAN_CAN0_INT0.B.SRPN = 30U;
//    SRC_CAN_CAN0_INT0.B.TOS = 0U;
//    SRC_CAN_CAN0_INT0.B.SRE = 1U;
    IfxMultican_Can_MsgObj_init(&g_CanInfo.canMsgRxObj, &canMsgObjConfig);
}

// 메세지만 전달
void transmit_message (Message_Info *msgptr, uint32 messageID)
{
    IfxMultican_Message msg;

    /*
     * send_data[0]:하위 4 byte
     * send_data[1]:상위 4 byte
     * */

    uint32 send_data[2] = {0};

    switch (messageID)
    {
        case VEHICLE_STATUS_ID:
        {
            send_data[0] = 0x11112222;
            send_data[1] = 0x33334444;
//            Ifx_Console_print("Transmit data: %x\r\n", (send_data[1] << 32) | send_data[0]);
            my_itoa_hex(send_data[0], buffer);
            Ifx_Console_print("Transmit: 0x");
            Ifx_Console_print(buffer);
            my_itoa_hex(send_data[1], buffer);
            Ifx_Console_print(buffer);
            Ifx_Console_print("\r\n");
            IfxMultican_Message_init(&msg,
                                     VEHICLE_STATUS_ID,
                                     send_data[0],
                                     send_data[1],
                                     IfxMultican_DataLengthCode_8);
            break;
        }
        default :

            break;
    }

    while (IfxMultican_Status_notSentBusy ==
                IfxMultican_Can_MsgObj_sendMessage(&g_CanInfo.canMsgTxObj,
                                                   &msg))
    {
    }
}
