/*********************************************************************************** 
;����˵��:�๦�ܵ�����
;�ļ�����:LedBuzzer.h 
;�汾�ţ�v1.0.0  
;΢������:STC89C52
;���뻷��:Keil uVision V4.10 
;��    ��:Cui Xinghai 
;��������:2022.4.3   
*******************************************************************************/


#ifndef _LED_BUZZER_H
#define _LED_BUZZER_H

struct sLedBuff {  //LED��ʾ�������ṹ
    uint8 array[8];   //���󻺳���
    uint8 number[6];  //����ܻ�����
    uint8 alone;      //����LED������
};

#ifndef _LED_BUZZER_C
extern bit staBuzzer;
extern struct sLedBuff ledBuff;
#endif

void InitLed();
void FlowingLight();
void ShowLedNumber(uint8 index, uint8 num, uint8 point);
void ShowLedArray(uint8 *ptr);

#endif
