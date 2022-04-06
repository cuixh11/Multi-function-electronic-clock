/*********************************************************************************** 
;����˵��:�๦�ܵ�����
;�ļ�����:main.c     
;�汾�ţ�v1.0.0  
;΢������:STC89C52
;���뻷��:Keil uVision V4.10 
;��    ��:Cui Xinghai 
;��������:2022.4.3   
;***********************************************************************************/

#define  _MAIN_C
#include "config.h"
#include "Lcd1602.h"
#include "LedBuzzer.h"
#include "keyboard.h"
#include "DS1302.h"
#include "DS18B20.h"
#include "Infrared.h"
#include "Time.h"
#include "main.h"

bit flag2s = 0;    //2s��ʱ��־λ
bit flag200ms = 0; //200ms��ʱ��־
uint8 T0RH = 0;    //T0����ֵ�ĸ��ֽ�
uint8 T0RL = 0;    //T0����ֵ�ĵ��ֽ�
enum eStaSystem staSystem = E_NORMAL;  //ϵͳ����״̬

void main()
{
    EA = 1;           //�����ж�
    ConfigTimer0(1);  //����T0��ʱ1ms
    InitLed();        //��ʼ��LEDģ��
    InitDS1302();     //��ʼ��ʵʱʱ��ģ��
    InitInfrared();   //��ʼ���������ģ��
    InitLcd1602();    //��ʼ��Һ��ģ��
    Start18B20();     //�����״��¶�ת��
    
    while (!flag2s);  //�ϵ����ʱ2��
    flag2s = 0;
    RefreshTime();    //ˢ�µ�ǰʱ��
    RefreshDate(1);   //����ˢ��������ʾ
    RefreshTemp(1);   //����ˢ���¶���ʾ
    RefreshAlarm();   //�����趨ֵ��ʾ
    
    while (1)  //������ѭ��
    {
        KeyDriver();      //ִ�а�������
        InfraredDriver(); //ִ�к����������
        if (flag200ms)    //ÿ��200msִ�����·�֧
        {
            flag200ms = 0;
            FlowingLight();  //��ˮ��Ч��ʵ��
            RefreshTime();   //ˢ�µ�ǰʱ��
            AlarmMonitor();  //�������
            if (staSystem == E_NORMAL)  //��������ʱˢ��������ʾ
            {
                RefreshDate(0);
            }
        }
        if (flag2s)  //ÿ��2sִ�����·�֧
        {
            flag2s = 0;
            if (staSystem == E_NORMAL)  //��������ʱˢ���¶���ʾ
            {
                RefreshTemp(0);
            }
        }
    }
}
/* �¶�ˢ�º�������ȡ��ǰ�¶Ȳ�������Ҫˢ��Һ����ʾ��
** ops-ˢ��ѡ�Ϊ0ʱֻ���¶ȱ仯��ˢ�£���0������ˢ�� */
void RefreshTemp(uint8 ops)
{
    int16 temp;
    uint8 pdata str[8];
    static int16 backup = 0;
    
    Get18B20Temp(&temp); //��ȡ��ǰ�¶�ֵ
    Start18B20();        //������һ��ת��
    temp >>= 4;          //����4bitС��λ
    if ((backup!=temp) || (ops!=0)) //����Ҫˢ��Һ����ʾ
    {
        str[0] = (temp/10) + '0';  //ʮλתΪASCII��
        str[1] = (temp%10) + '0';  //��λתΪASCII��
        str[2] = '\'';             //��'C�����
        str[3] = 'C';
        str[4] = '\0';             //�ַ���������
        LcdShowStr(12, 0, str);    //��ʾ��Һ����
        backup = temp;             //ˢ���ϴ��¶�ֵ
    }
}
/* ���ò�����T0��ms-T0��ʱʱ�� */
void ConfigTimer0(uint16 ms)
{
    uint32 tmp;
    
    tmp = (SYS_MCLK*ms)/1000; //��������ļ���ֵ
    tmp = 65536 - tmp;        //���㶨ʱ������ֵ
    tmp = tmp + 33;           //�����ж���Ӧ��ʱ��ɵ����   
    T0RH = (uint8)(tmp>>8);   //��ʱ������ֵ���Ϊ�ߵ��ֽ�
    T0RL = (uint8)tmp;
    TMOD &= 0xF0;   //����T0�Ŀ���λ
    TMOD |= 0x01;   //����T0Ϊģʽ1
    TH0 = T0RH;     //����T0����ֵ
    TL0 = T0RL;
    ET0 = 1;        //ʹ��T0�ж�
    TR0 = 1;        //����T0
}
/* T0�жϷ�������ʵ��ϵͳ��ʱ�Ͱ���ɨ�� */
void InterruptTimer0() interrupt 1
{
    static uint8 tmr2s = 0;
    static uint8 tmr200ms = 0;
    
    TH0 = T0RH;  //���¼�������ֵ
    TL0 = T0RL;
    tmr200ms++;  //��ʱ200ms
    if (tmr200ms >= 200)
    {
        tmr200ms = 0;
        flag200ms = 1;
        tmr2s++;  //��ʱ2s
        if (tmr2s >= 10)
        {
            tmr2s = 0;
            flag2s = 1;
        }
    }
    KeyScan();   //ִ�а���ɨ��
}
