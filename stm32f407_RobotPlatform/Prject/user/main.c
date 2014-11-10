/**
  ******************************************************************************
  * @file    Audio_playback_and_record/src/main.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    28-October-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include"includes.h"
#include"bsp.h"

/** @addtogroup STM32F4-Discovery_Audio_Player_Recorder
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static OS_TCB AppTaskStartTCB;    // ����ʱ��� 
static CPU_STK AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE];   // �����ջ 
// static OS_TCB AppTaskUpdatTCB;  // ����ʱ��� 
// static CPU_STK AppTaskUpdatStk[APP_CFG_TASK_UPDATW_STK_SIZE];  // �����ջ

static  OS_TCB   AppTaskGUITCB;
static  CPU_STK  AppTaskGUIStk[APP_CFG_TASK_GUI_STK_SIZE];

static  OS_TCB   AppTaskGUIRefreshTCB;
static  CPU_STK  AppTaskGUIRefreshStk[APP_CFG_TASK_GUIRefresh_STK_SIZE];
uint8_t datesBuff[8];
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
static void AppTaskCreate (void);
static void AppTaskStart(void *p_arg);
void Config(void)
{
	
}
void AppTaskCreate(void)
{
	OS_ERR      err;	 // 
/* ����GUI ����*/
	OSTaskCreate((OS_TCB       *)&AppTaskGUITCB,              
                 (CPU_CHAR     *)"App Task GUI",
                 (OS_TASK_PTR   )AppTaskGUI, 
                 (void         *)0,
                 (OS_PRIO       )APP_CFG_TASK_GUI_PRIO,
                 (CPU_STK      *)&AppTaskGUIStk[0],
                 (CPU_STK_SIZE  )APP_CFG_TASK_GUI_STK_SIZE / 10,
                 (CPU_STK_SIZE  )APP_CFG_TASK_GUI_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )8,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);
	/**************����GUIˢ������*********************/			 
	OSTaskCreate((OS_TCB       *)&AppTaskGUIRefreshTCB,              
                 (CPU_CHAR     *)"App Task GUIRefresh",
                 (OS_TASK_PTR   )AppTaskGUIRefresh, 
                 (void         *)0,
                 (OS_PRIO       )APP_CFG_TASK_GUIRefresh_PRIO,
                 (CPU_STK      *)&AppTaskGUIRefreshStk[0],
                 (CPU_STK_SIZE  )APP_CFG_TASK_GUIRefresh_STK_SIZE / 10,
                 (CPU_STK_SIZE  )APP_CFG_TASK_GUIRefresh_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )8,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);
}
/**
  * @brief  Main program.
  * @param  None
  * @retval None
*/
int main(void)
{ 
	OS_ERR err;
	
	RCC_ClocksTypeDef Rcc_get;   // ��·ʱ�� 
	RCC_GetClocksFreq(&Rcc_get);
  OSInit(&err);
	OSTaskCreate((OS_TCB *) &AppTaskStartTCB,
	(CPU_CHAR *)"App Task Start",
	(OS_TASK_PTR) AppTaskStart,
	(void *) 0,
	(OS_PRIO ) APP_CFG_TASK_START_PRIO,
	(CPU_STK * )&AppTaskStartStk[0],
	(CPU_STK_SIZE ) APP_CFG_TASK_START_STK_SIZE/10,
	(CPU_STK_SIZE ) APP_CFG_TASK_START_STK_SIZE,
	(OS_MSG_QTY)0,
	(OS_TICK)  0,
	(void *) 0,
	(OS_OPT )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
	(OS_ERR *)&err);
	OSStart(&err);
  (void)&err;
	return (0);
	
}
uint16_t tempDatas;
static void AppTaskStart(void *p_arg)
{
  OS_ERR      err;
	uint8_t i,dats,j;
	I2CxInformation __IO * I2CConfigPoint;
	CPU_SR_ALLOC();
	
	(void)p_arg;
	// BSP_Init();	
	CPU_Init();
	BSP_Tick_Init();

	Mem_Init();                             
	Math_Init();  
#if OS_CFG_STAT_TASK_EN > 0u
     OSStatTaskCPUUsageInit(&err);   
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();
#endif
    AppTaskCreate();
	  Init_I2CAnd_DMA();
//	  PortConfig(LED,PORT1,"Led1");
//	  DatasUsedToInput[PORT1][0]=0x68;
//		tempDatas=getValue("Led1");
		
	  AccessToInformation();
		while(1)
		{
			 
			GPIO_ToggleBits(LEDLeft_PORT,LEDDown);
			
			for(i=0;i<16;i++)
			{
				if(FlagForOK&(0x01<<i))                   //��־λ���� 
				{
					I2CConfigPoint=GetConfig();	
					I2CConfigPoint->comState=COMM_PRE;      // ����ǰ��
					I2CConfigPoint->comDir=MSTER_RECERVE;		// ����ģʽ
					I2CConfigPoint->configLength=0;         // �ɱ䳤��0 
					I2CConfigPoint->slveAddress=0x30;
				  // GPIO_ResetBits(GPIOB,GPIO_Pin_7);       //���� IO ��as  HIgh
				  slectPortUesd(i,1);                     // ѡ��˿� ʹ�� 
					OSTimeDlyHMSM((CPU_INT16U) 0u,
												(CPU_INT16U) 0u,
												(CPU_INT16U) 0,
												(CPU_INT32U) 5,
												(OS_OPT    ) OS_OPT_TIME_HMSM_STRICT,
												(OS_ERR   *)&err);
				  masterRecerveData(0x30,datesBuff,6);   
				  // GPIO_SetBits(GPIOB,GPIO_Pin_7);	
          slectPortUesd(i,0);                    // �˿�ֹͣͨ��
				  dats=0;
				  for(j=0;j<5;j++)
				  {
					  dats+=datesBuff[j];
				  }
				  if(dats==datesBuff[5])
					{
					  LED_Change(0);
						comData[i].DeviveID=datesBuff[0];   // ��ȡ�豸ID
						// if(comData[i].DeviveID)    //�豸ID�� ��ʪ�����ݴ���Ļ���Ҫ���⴦��
						comData[i].DataPoint[0]=	datesBuff[1];  // ���ݴ洢
						comData[i].DataPoint[1]=	datesBuff[2];				
					}
				}
			}
			OSTimeDlyHMSM((CPU_INT16U) 0u,
										(CPU_INT16U) 0u,
										(CPU_INT16U) 0,
										(CPU_INT32U) 30,
										(OS_OPT    ) OS_OPT_TIME_HMSM_STRICT,
										(OS_ERR   *)&err);
		}
}
  
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
