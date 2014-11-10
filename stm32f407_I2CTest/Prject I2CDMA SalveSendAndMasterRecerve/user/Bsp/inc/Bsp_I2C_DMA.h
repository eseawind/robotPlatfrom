
#ifndef _BSP_I2C_DMA_H_
#define _BSP_I2C_DMA_H_
#include"stm32f4xx.h"
typedef enum i2cState
{
	COMM_DOWN=0,    //success 
	COMM_PRE=1,     //׼��ͨ��
	COMM_IN_PRECESS=2,  // ����ͨ����
	COMM_EXIT=3					// ͨ��ʧ���˳�
}I2Cx_STATE;
typedef enum i2cDir
{
	MSTER_SEND=0,				// ������ģʽ
	MSTER_RECERVE=1			// ������ģʽ
}I2Cx_DIRSTATE;
typedef struct I2CxInformation{
	I2Cx_STATE comState;				//ͨ��״̬
	I2Cx_DIRSTATE comDir;       //ͨ�ŷ�ʽ �����ջ��Ƿ���
	uint8_t slveAddress;				//��Ҫ���͵Ĵӻ���ַ
	uint8_t configLength;       //�����Ƿ�ɱ䳤�ȷ���
	
}I2CxInformation;

extern uint8_t I2C1_MasterSendBuff[10];   // I2C1 ���ͱ�����
extern uint8_t I2C1_MasterReceBuff[10];	  // I2C1 ���ձ�����
/*
*   I2C1 �����жϴ�����
**/
void I2C1_EE_IRQ_Delay(void);

/*
*   I2C1 �¼��жϴ�����
**/
void I2C1_EV_IRQ_Delay(void);

/* ���I2C�����ļ�*/
void SetConfig(I2CxInformation * data);
	
/* ��ȡI2C�����ļ�*/
I2CxInformation * GetConfig(void);

void Init_I2CAnd_DMA(void);
/*
*  �������� ����
*  salveAddress : �ӻ���ַ
*  sendData[] ��������ָ��
*  length     �������ݵ�����
**/
void masterSendData(uint8_t salveAddress,uint8_t sendData[],uint8_t lenght);
/*
*  �������� ����
*  salveAddress : �ӻ���ַ
*  RecerveData[] ��������ָ��
*  length     �������ݵ�����
**/
void masterRecerveData(uint8_t salveAddress,uint8_t RecerveData[],uint8_t lenght);
#endif 
/**************************************END LINE ****************************************/
