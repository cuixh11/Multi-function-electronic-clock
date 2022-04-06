/*********************************************************************************** 
;����˵��:�๦�ܵ�����
;�ļ�����:Lcd1602.h     
;�汾�ţ�v1.0.0  
;΢������:STC89C52
;���뻷��:Keil uVision V4.10 
;��    ��:Cui Xinghai 
;��������:2022.4.3   
;***********************************************************************************/
#ifndef _LCD1602_H
#define _LCD1602_H


#ifndef _LCD1602_C

#endif

void InitLcd1602();
void LcdClearScreen();
void LcdOpenCursor();
void LcdCloseCursor();
void LcdSetCursor(uint8 x, uint8 y);
void LcdShowStr(uint8 x, uint8 y, uint8 *str);
void LcdShowChar(uint8 x, uint8 y, uint8 chr);

#endif
