/*

  ******************************************************************************
  * @file     : CANTest.cpp
  * @Copyright: ViewTool
  * @Revision : ver 1.0
  * @Date     : 2014/12/29 9:27
  * @brief    : CANTest demo
  ******************************************************************************
  * @attention
  *
  * Copyright 2009-2014, ViewTool
  * http://www.viewtool.com/
  * All Rights Reserved
  *
  ******************************************************************************
  */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ControlCAN.h"
#include <stdio.h>

#define CAN_CALLBACK_READ_DATA 0
#define can_index 1

#ifdef CAN_CALLBACK_READ_DATA
void WINAPI GetDataCallback(uint32_t DevIndex,uint32_t CANIndex,uint32_t Len)
{
    printf("GetDataCallback\n");
    int ReadDataNum;
	int DataNum = VCI_GetReceiveNum(VCI_USBCAN2, 0, can_index);
	VCI_CAN_OBJ	*pCAN_ReceiveData = (VCI_CAN_OBJ *)malloc(DataNum*sizeof(VCI_CAN_OBJ));
    if((DataNum > 0)&&(pCAN_ReceiveData != NULL)){
        ReadDataNum = VCI_Receive(VCI_USBCAN2, 0, can_index, pCAN_ReceiveData, DataNum);
        for (int i = 0; i < ReadDataNum; i++)
        {
			printf("\n");
            printf("--CAN_ReceiveData.RemoteFlag = %d\n",pCAN_ReceiveData[i].RemoteFlag);
            printf("--CAN_ReceiveData.ExternFlag = %d\n",pCAN_ReceiveData[i].ExternFlag);
            printf("--CAN_ReceiveData.ID = 0x%X\n",pCAN_ReceiveData[i].ID);
            printf("--CAN_ReceiveData.DataLen = %d\n",pCAN_ReceiveData[i].DataLen);
            printf("--CAN_ReceiveData.Data:");
            for(int j=0;j<pCAN_ReceiveData[i].DataLen;j++){
                printf("%02X ",pCAN_ReceiveData[i].Data[j]);
            }
            printf("\n");
            printf("--CAN_ReceiveData.TimeStamp = %d\n\n",pCAN_ReceiveData[i].TimeStamp);
        }
    }
	free(pCAN_ReceiveData);
}
#endif

// CAN READ DATA
void ReadData()
{
  int ReadDataNum;
  int DataNum = VCI_GetReceiveNum(VCI_USBCAN2, 0, can_index);
  VCI_CAN_OBJ	*pCAN_ReceiveData = (VCI_CAN_OBJ *)malloc(DataNum*sizeof(VCI_CAN_OBJ));

      // printf("DataNum: %i\n", DataNum);
    if((DataNum > 0)&&(pCAN_ReceiveData != NULL)){
    ReadDataNum = VCI_Receive(VCI_USBCAN2, 0, can_index, pCAN_ReceiveData, DataNum);
    for (int i = 0; i < ReadDataNum; i++)
    {
      printf("\n");
      printf("--CAN_ReceiveData.RemoteFlag = %d\n",pCAN_ReceiveData[i].RemoteFlag);
      printf("--CAN_ReceiveData.ExternFlag = %d\n",pCAN_ReceiveData[i].ExternFlag);
      printf("--CAN_ReceiveData.ID = 0x%X\n",pCAN_ReceiveData[i].ID);
      printf("--CAN_ReceiveData.DataLen = %d\n",pCAN_ReceiveData[i].DataLen);
      printf("--CAN_ReceiveData.Data:");
      for(int j=0;j<pCAN_ReceiveData[i].DataLen;j++){
        printf("%02X ",pCAN_ReceiveData[i].Data[j]);
      }
      printf("\n");
      printf("--CAN_ReceiveData.TimeStamp = %d\n\n",pCAN_ReceiveData[i].TimeStamp);
    }
  }
  free(pCAN_ReceiveData);
}


void ParseData(VCI_CAN_OBJ pCAN_ReceiveData){


}

// CAN SEND DATA
//Send data
void WINAPI SendData()
{
  int Status;

  VCI_CAN_OBJ	CAN_SendData[2];
  for(int j=0;j<2;j++){
  CAN_SendData[j].DataLen = 8;
  for(int i=0;i<CAN_SendData[j].DataLen;i++){
    CAN_SendData[j].Data[i] = 0x55;
  }
  CAN_SendData[j].ExternFlag = 0;
  CAN_SendData[j].RemoteFlag = 0;
  CAN_SendData[j].ID = 0x155+j;
  CAN_SendData[j].SendType = 2;
  CAN_SendData[j].SendType = 0;

  }
  Status = VCI_Transmit(VCI_USBCAN2,0,can_index,CAN_SendData,2);
  if(Status==STATUS_ERR){
      printf("Send CAN data failed!\n");
      VCI_ResetCAN(VCI_USBCAN2,0,can_index);
  }else{
      printf("Send CAN data success!\n");
  }
}


// CAN INIT Code in case INIT_EX too much
// VCI_INIT_CONFIG	CAN_Init;
//   //Config device
// CAN_Init.AccCode = 0x00000000;
// CAN_Init.AccMask = 0xFFFFFFFF;
// CAN_Init.Filter = 1;
// CAN_Init.Mode = 1;
// CAN_Init.Timing0 = 0x00;
// CAN_Init.Timing1 = 0x1C;
//   Status = VCI_InitCAN(VCI_USBCAN2,0,can_index,&CAN_Init);
//   if(Status==STATUS_ERR){
//       printf("Init device failed!\n");
//       return 0;
//   }else{
//       printf("Init device success!\n");
//   }


int main(void)
{
    int DevNum,Status;

    //Scan device
    DevNum = VCI_ScanDevice(1);
    if(DevNum > 0){
        printf("Have %d device connected!\n",DevNum);
    } else{
        printf("No device connected!\n");
        return 0;
    }

    //Open device
    Status = VCI_OpenDevice(VCI_USBCAN2,0,0);
    if(Status==STATUS_ERR){
        printf("Open device failed!\n");
        printf("Status Message: %i\n", Status);
        return 0;
    } else{
        printf("Open device success!\n");
    }

    // CAN INIT Ex
	  VCI_INIT_CONFIG_EX	CAN_InitEx;
    //Config device
    CAN_InitEx.CAN_ABOM = 0;
    CAN_InitEx.CAN_Mode = 0;
    //CAN baud rate = 36MHz/(CAN_BRP)/(CAN_SJW+CAN_BS1+CAN_BS2)
    //1Mbps
    CAN_InitEx.CAN_BRP = 12;//6;
    CAN_InitEx.CAN_BS1 = 3;//3;
    CAN_InitEx.CAN_BS2 = 2;//2;
    CAN_InitEx.CAN_SJW = 1;

    CAN_InitEx.CAN_NART = 0;
    CAN_InitEx.CAN_RFLM = 0;
    CAN_InitEx.CAN_TXFP = 1;
	  CAN_InitEx.CAN_RELAY = 0;
    Status = VCI_InitCANEx(VCI_USBCAN2,0,can_index,&CAN_InitEx);
    if(Status==STATUS_ERR){
        printf("Init device failed!\n");
        printf("Status Err: %i\n", Status);
        return 0;
    }else{
        printf("Init device success!\n");
    }
    //Set filter
	  VCI_FILTER_CONFIG CAN_FilterConfig;
    CAN_FilterConfig.FilterIndex = 0;
    CAN_FilterConfig.Enable = 1;		//Enable
    CAN_FilterConfig.ExtFrame = 1;
    CAN_FilterConfig.FilterMode = 0;
    CAN_FilterConfig.ID_IDE = 0;
    CAN_FilterConfig.ID_RTR = 0;
    CAN_FilterConfig.ID_Std_Ext = 0;
    CAN_FilterConfig.MASK_IDE = 0;
    CAN_FilterConfig.MASK_RTR = 0;
    CAN_FilterConfig.MASK_Std_Ext = 0;
    Status = VCI_SetFilter(VCI_USBCAN2,0,can_index,&CAN_FilterConfig);
    if(Status==STATUS_ERR){
        printf("Set filter failed!\n");
        return 0;
    }else{
        printf("Set filter success!\n");
    }

	   //Register callback function
    #if CAN_CALLBACK_READ_DATA
      VCI_RegisterReceiveCallback(0,GetDataCallback);
    #endif

    //Start CAN
    Status = VCI_StartCAN(VCI_USBCAN2,0,can_index);
    if(Status==STATUS_ERR){
        printf("Start CAN failed!\n");
        return 0;
    }else{
        printf("Start CAN success!\n");
    }

    // Read Data Block
    bool read_data = 1;
    if(read_data){
      while(1){
        ReadData();
      }
    }

    sleep(0.01);
    while(getchar()!='\n');

    #if CAN_CALLBACK_READ_DATA
	  VCI_LogoutReceiveCallback(0);
	  printf("VCI_LogoutReceiveCallback\n");
    #endif

	  usleep(10*1000);
	  //Stop receive can data
	  Status = VCI_ResetCAN(VCI_USBCAN2,0,can_index);
    printf("VCI_ResetCAN %d\n",Status);
	  VCI_CloseDevice(VCI_USBCAN2,0);
	  printf("VCI_CloseDevice\n");
    return 0;
}
