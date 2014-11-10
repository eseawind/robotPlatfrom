
#include"I2CConfigDeclaration.h"
#include"String.h"
uint16_t DatasUsedToInput[MAXINPUT][2];
uint16_t DatasUsedToOutput[16][2];
COMDATA comData[MAXINPUT];
uint8_t comUsedNumber=0;
extern void Config(void);
/* ͨ�� ���Ʒ���ȡ�õ����� */
uint16_t getValue(char buff[])
{
	uint8_t i=0;
	for(i=0;i<16;i++)
	{
		if(strcmp(buff,comData[i].name)==0)  // �������
		{
			return (*comData[i].DataPoint);
		}			
	}
	return 0xffff;
}
/*  �������������豸  */
void PortConfig(Mode modes,uint8_t comPort,char buff[])
{
	uint8_t i;
	if(comUsedNumber >=16) return ;        // ������������޶�
	comData[comUsedNumber].modeType=modes; // ����ģʽ  
	comData[comUsedNumber].dir=1;          // ������� 
	i=strlen(buff);
	if(i>0 && i<10){
	  strcpy(comData[comUsedNumber].name,buff);
	}
	comData[comUsedNumber].Online=0;
	comData[comUsedNumber].DataPoint= DatasUsedToInput[comUsedNumber];
	comUsedNumber++;	
}
/*  ������������豸 */
void DriverComfig(Mode modes,uint8_t comPort,char buff[])
{
	uint8_t i;
	if(comUsedNumber >=16) return ;  // ������������޶�
	comData[comUsedNumber].dir=0;   // ������� 
	i=strlen(buff);
	if(i>0 && i<10){
	  strcpy(comData[comUsedNumber].name,buff);
	}
	comData[comUsedNumber].Online=0;
	comData[comUsedNumber].DataPoint= DatasUsedToOutput[comUsedNumber];
	comUsedNumber++;
}
void ModeConfig(void)
{
	// 
	Config();
	
}

