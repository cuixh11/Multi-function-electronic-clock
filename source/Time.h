/*********************************************************************************** 
;����˵��:�๦�ܵ�����
;�ļ�����:Time.h
;�汾�ţ�v1.0.0  
;΢������:STC89C52
;���뻷��:Keil uVision V4.10 
;��    ��:Cui Xinghai 
;��������:2022.4.3   
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
