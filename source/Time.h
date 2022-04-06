/*********************************************************************************** 
;功能说明:多功能电子钟
;文件名称:Time.h
;版本号：v1.0.0  
;微处理器:STC89C52
;编译环境:Keil uVision V4.10 
;作    者:Cui Xinghai 
;创建日期:2022.4.3   
*******************************************************************************/



#ifndef _TIME_H
#define _TIME_H


#ifndef _TIME_C

#endif

void RefreshTime();
void RefreshDate(uint8 ops);
void RefreshAlarm();
void AlarmMonitor();
void KeyAction(uint8 keycode);

#endif
