/*********************************************************************************** 
;����˵��:�๦�ܵ�����
;�ļ�����:Lcd1602.c     
;�汾�ţ�v1.0.0  
;΢������:STC89C52
;���뻷��:Keil uVision V4.10 
;��    ��:Cui Xinghai 
;��������:2022.4.3   
;��    ע����Һ���Ŀ�����LED��ʾ����P0�ں�P1�ڵĲ�����Դ�����Ա������ڷ���Һ��ʱ
*          ����ȡ��ʩ�����P0��P1���ߵķ��ʳ�ͻ��
*******************************************************************************/

#define  _LCD1602_C
#include "config.h"
#include "Lcd1602.h"

uint8 tmpP0;   //�ݴ�P0�ڵ�ֵ
bit tmpADDR0;  //�ݴ�LEDλѡ�����ַ0��ֵ
bit tmpADDR1;  //�ݴ�LEDλѡ�����ַ1��ֵ

/* ��ͣLED��̬ɨ�裬�ݴ�������ŵ�ֵ */
void LedScanPause()
{
    ENLED = 1;
    tmpP0 = P0;
    tmpADDR0 = ADDR0;
    tmpADDR1 = ADDR1;
}
/* �ָ�LED��̬ɨ�裬�ָ�������ŵ�ֵ */
void LedScanContinue()
{
    ADDR0 = tmpADDR0;
    ADDR1 = tmpADDR1;
    P0 = tmpP0;
    ENLED = 0;
}
/* �ȴ�Һ��׼���� */
void LcdWaitReady()
{
    uint8 sta;
    
    LCD1602_DB = 0xFF;
    LCD1602_RS = 0;
    LCD1602_RW = 1;
    do {
        LCD1602_E = 1;
        sta = LCD1602_DB; //��ȡ״̬��
        LCD1602_E = 0;
    } while (sta & 0x80); //bit7����1��ʾҺ����æ���ظ����ֱ�������0Ϊֹ
}
/* ��LCD1602Һ��д��һ�ֽ����cmd-��д������ֵ */
void LcdWriteCmd(uint8 cmd)
{
    LedScanPause();
    LcdWaitReady();
    LCD1602_RS = 0;
    LCD1602_RW = 0;
    LCD1602_DB = cmd;
    LCD1602_E  = 1;
    LCD1602_E  = 0;
    LedScanContinue();
}
/* ��LCD1602Һ��д��һ�ֽ����ݣ�dat-��д������ֵ */
void LcdWriteDat(uint8 dat)
{
    LedScanPause();
    LcdWaitReady();
    LCD1602_RS = 1;
    LCD1602_RW = 0;
    LCD1602_DB = dat;
    LCD1602_E  = 1;
    LCD1602_E  = 0;
    LedScanContinue();
}
/* ���� */
void LcdClearScreen()
{
	LcdWriteCmd(0x01);
}
/* �򿪹�����˸Ч�� */
void LcdOpenCursor()
{
	LcdWriteCmd(0x0F);
}
/* �رչ����ʾ */
void LcdCloseCursor()
{
	LcdWriteCmd(0x0C);
}
/* ������ʾRAM��ʼ��ַ���༴���λ�ã�(x,y)-��Ӧ��Ļ�ϵ��ַ����� */
void LcdSetCursor(uint8 x, uint8 y)
{
    uint8 addr;
    
    if (y == 0)  //���������Ļ���������ʾRAM�ĵ�ַ
        addr = 0x00 + x;  //��һ���ַ���ַ��0x00��ʼ
    else
        addr = 0x40 + x;  //�ڶ����ַ���ַ��0x40��ʼ
    LcdWriteCmd(addr | 0x80);  //����RAM��ַ
}
/* ��Һ������ʾ�ַ�����(x,y)-��Ӧ��Ļ�ϵ���ʼ���꣬str-�ַ���ָ�� */
void LcdShowStr(uint8 x, uint8 y, uint8 *str)
{
    LcdSetCursor(x, y);   //������ʼ��ַ
    while (*str != '\0')  //����д���ַ������ݣ�ֱ����⵽������
    {
        LcdWriteDat(*str++);
    }
}
/* ��Һ������ʾһ���ַ���(x,y)-��Ӧ��Ļ�ϵ���ʼ���꣬chr-�ַ�ASCII�� */
void LcdShowChar(uint8 x, uint8 y, uint8 chr)
{
    LcdSetCursor(x, y);  //������ʼ��ַ
    LcdWriteDat(chr);    //д��ASCII�ַ�
}
/* ��ʼ��1602Һ�� */
void InitLcd1602()
{
    LcdWriteCmd(0x38);  //16*2��ʾ��5*7����8λ���ݽӿ�
    LcdWriteCmd(0x0C);  //��ʾ���������ر�
    LcdWriteCmd(0x06);  //���ֲ�������ַ�Զ�+1
    LcdWriteCmd(0x01);  //����
}
