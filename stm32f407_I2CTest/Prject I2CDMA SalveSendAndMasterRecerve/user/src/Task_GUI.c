#include"Task_GUI.h"

void AppTaskGUI(void *p_arg)
{
		(void) p_arg;
	 while(1)
	 {
		  BSP_OS_TimeDly(200);	
	 }
}

void AppTaskGUIRefresh(void *p_arg)
{
	 (void)p_arg;
	while(1)
	{
		 GUI_Exec();             //    GUI �л�
		 GUI_TOUCH_Exec();	     //    �������л�
		 BSP_OS_TimeDly(100);
	}
}
