/**
  ******************************************************************************
  * @file    Audio_playback_and_record/src/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    28-October-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
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
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include"stm32f4xx_it.h"
#include"Bsp_I2C.h"

/** @addtogroup STM32F4-Discovery_Audio_Player_Recorder
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern uint32_t LedTimeDelay;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/
void I2C1_EV_IRQHandler(void)
{
	uint32_t i2cEvent;
	I2C_InformationSendAndRecerve __IO * I2C_InterruptBuff;
  I2C_InterruptBuff=ARC_GetI2C_Information();  // get config address 
	i2cEvent=I2C_GetLastEvent(I2C1);  // h��ȡ״̬��־λ
	switch(i2cEvent)  
	{
		 /*���豸�����ڡ�I2C ���� �ɹ� */
		case  I2C_EVENT_MASTER_MODE_SELECT: // EV5
			I2C_AcknowledgeConfig(I2C1, ENABLE);  // ������ ��Ӧ�ź� 
			// send adds as Transmitter
		  // i2cEvent=ARC_I2C_DIRECTION_TX
		  // ���� �ӻ���ַ  �� �������� ���� 
		// if(I2C_InterruptBuff->)
			if(I2C_InterruptBuff->I2C_DIRECTION == ARC_I2C_DIRECTION_TX)
			{
				I2C_Send7bitAddress(I2C1, I2C_InterruptBuff->DeviceAddr, I2C_Direction_Transmitter); 
			}else{
				I2C_Send7bitAddress(I2C1, I2C_InterruptBuff->DeviceAddr, I2C_Direction_Receiver);   // ���ʹӻ���ַ��������������
			}				
			break;
		/* Master Transmitter -------------------------------------------------------*/
		case I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED:  // EV 6 ADDR=1  /* ��ַ���ͳɹ� */
			I2C_SendData(I2C1,I2C_InterruptBuff->TxData[I2C_InterruptBuff->Tx_I2C_Index++]);  // send data 
		  // end and disable TX buff 
			if(I2C_InterruptBuff->TxNumOfBytes==1)
			{
				I2C_ITConfig(I2C1, I2C_IT_BUF, DISABLE);  // ��ֹ�����жϣ������ͽ��� TxE=1 �����ж�
			}
			break;
		case I2C_EVENT_MASTER_BYTE_TRANSMITTING:  /* Without BTF, EV8 */   // ���� ���ݼĴ����� ���� Ϊ��� 
			if(I2C_InterruptBuff->RX_I2C_Index<I2C_InterruptBuff->TxNumOfBytes)
			{
				I2C_SendData(I2C1,I2C_InterruptBuff->TxData[I2C_InterruptBuff->Tx_I2C_Index++]);
			}else{
				I2C_ITConfig(I2C1, I2C_IT_BUF, DISABLE);  // end and disable TX buff 
			}		
		case I2C_EVENT_MASTER_BYTE_TRANSMITTED: /* With BTF EV8-2 */   // �������һ��������ɳ���ֹͣ����
		/*    ���������ź� */
			if(I2C_InterruptBuff->TX_Generate_stop==1)
			{
				I2C_GenerateSTOP(I2C1, ENABLE);                // ����ֹͣλ 
				I2C_InterruptBuff->ok=1;
				I2C_ITConfig(I2C1, I2C_IT_EVT, DISABLE);		
			}else{  
				/* ����������   ת���ɽ���ģʽ ��ʼ����*/
				I2C_InterruptBuff->I2C_DIRECTION=ARC_I2C_DIRECTION_RX; // ת���� ���� ģʽ
				I2C_ITConfig(I2C1, I2C_IT_BUF, ENABLE);                //  ���������ж�
				I2C_GenerateSTART(I2C1, ENABLE);										 // ������ʼ�ź� ת����������ģʽ
			}	
		break;
			
		/* Master Recerve ------------------------------------------------------------*/
		case I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED:   //  EV6  ���ݵ�ַ�ͽ���ģʽƥ��  ���ߴ���æ״̬   BUSY, MSL and ADDR flags 
			/* �ж� �������һ���ֽ�ǰ  ֹͣ ACKӦ�� ������ ֹͣ�ź�  */
		/*
		if(pI2C_param->RX_I2C_Index == (pI2C_param->RxNumOfBytes - 1))
		{
				I2C_AcknowledgeConfig(I2C1, DISABLE);
				I2C_GenerateSTOP(I2C1, ENABLE);
		}*/
		break;
		case I2C_EVENT_MASTER_BYTE_RECEIVED:  // EV7  ���ݽ��ճɹ� 
			/* ע�� �������һ������ǰ ֹͣ������Ӧ�źŲ��ҷ���ֹͣ�ź�*/
//			pI2C_param->RxData[pI2C_param->RX_I2C_Index++] = I2C_ReceiveData (I2C1);
//			if(pI2C_param->RX_I2C_Index == (pI2C_param->RxNumOfBytes - 1))
//			{
//				I2C_AcknowledgeConfig(I2C1, DISABLE);
//				I2C_GenerateSTOP(I2C1, ENABLE);
//			}			
		break;
	}
}
/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
    if(LedTimeDelay)
			LedTimeDelay--;
}


/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/
/**
  * @brief  This function handles External line 1 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI1_IRQHandler(void)
{

}

/**
  * @brief  This function handles TIM4 global interrupt request.
  * @param  None
  * @retval None
  */
void TIM4_IRQHandler(void)
{

}

#if defined MEDIA_USB_KEY
/**
  * @brief  EXTI0_IRQHandler
  *         This function handles External line 0 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler(void)
{

}


/**
  * @brief  This function handles TIM2 global interrupt request.
  * @param  None
  * @retval None
  */
void TIM2_IRQHandler(void)
{
  // USB_OTG_BSP_TimerIRQ();
}



/**
  * @brief  This function handles USB-On-The-Go FS global interrupt request.
  * @param  None
  * @retval None
  */
void OTG_FS_IRQHandler(void)
{
  // USBH_OTG_ISR_Handler(&USB_OTG_Core);
}
#endif /* MEDIA_USB_KEY */

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/


/**
  * @}
  */ 
  
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
