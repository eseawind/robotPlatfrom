
#ifndef _I2CConfigDeclaration_H_
#define _I2CConfigDeclaration_H_
#include"includes.h"
#define MAXINPUT   16
#define PORT1 0
#define PORT2 1
#define PORT3 2
typedef enum{
	LED=0,
	LCD=1
}Mode;

typedef struct      // ���ڴ�Ŷ˿�����
{
	char name[10];
	Mode modeType;
	uint8_t dir;     // ���ݴ��䷽��
	uint8_t Online;  // ģ��ȡ�����ݳɹ��� 
	uint16_t * DataPoint;  // ����ָ�� 
	uint8_t DeviveID;      // �豸ID
}COMDATA;
extern COMDATA comData[MAXINPUT];
extern uint16_t DatasUsedToInput[16][2];
/*
*  modes    ģ�����ͺ� 
*  comPort  �ܽź�
*  buff     ģ������֣������������ģ�����ֲ�ͬ
**/
void PortConfig(Mode modes,uint8_t comPort,char buff[]);
void DriverComfig(Mode modes,uint8_t comPort,char buff[]);
/* ͨ�� ���Ʒ���ȡ�õ����� */
uint16_t getValue(char buff[]);
void ModeConfig(void);
#endif 

