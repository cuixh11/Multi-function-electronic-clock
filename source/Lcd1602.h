/*********************************************************************************** 
;功能说明:多功能电子钟
;文件名称:Lcd1602.h     
;版本号：v1.0.0  
;微处理器:STC89C52
;编译环境:Keil uVision V4.10 
;作    者:Cui Xinghai 
;创建日期:2022.4.3   
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
