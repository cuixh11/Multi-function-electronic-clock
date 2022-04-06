/*********************************************************************************** 
;����˵��:�๦�ܵ�����
;�ļ�����:LedBuzzer.c 
;�汾�ţ�v1.0.0  
;΢������:STC89C52
;���뻷��:Keil uVision V4.10 
;��    ��:Cui Xinghai 
;��������:2022.4.3   
*******************************************************************************/

#define  _LED_BUZZER_C
#include "config.h"
#include "LedBuzzer.h"

uint8 code LedChar[] = {  //�������ʾ�ַ�ת����
    0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8,
    0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E
};

bit staBuzzer = 0; //������״̬����λ��1-���С�0-�ر�
struct sLedBuff ledBuff; //LED��ʾ��������Ĭ�ϳ�ֵȫ0�����ôﵽ�ϵ�ȫ����Ч��

/* LED��ʼ����������ʼ��IO�����ö�ʱ�� */
void InitLed()
{
    //��ʼ��IO��
    P0 = 0xFF;
    ENLED = 0;

    //����T2��Ϊ��̬ɨ�趨ʱ
    T2CON = 0x00;  //����T2������16λ�Զ����ض�ʱ��ģʽ
    RCAP2H = ((65536-SYS_MCLK/1500)>>8);  //��������ֵ��ÿ�����1500���жϣ�
    RCAP2L = (65536-SYS_MCLK/1500);       //��ʹˢ���ʴﵽ100Hz����˸��Ч��
    TH2 = RCAP2H;  //���ó�ֵ��������ֵ
    TL2 = RCAP2L;
    ET2 = 1;       //ʹ��T2�ж�
    PT2 = 1;       //����T2�ж�Ϊ�����ȼ�
    TR2 = 1;       //����T2
}
/* ��ˮ��ʵ�ֺ������������ʵ������Ч�� */
void FlowingLight()
{
    static uint8 i = 0;
    const uint8 code tab[] = {  //������
        0x7F, 0x3F, 0x1F, 0x0F, 0x87, 0xC3, 0xE1, 0xF0, 0xF8, 0xFC, 0xFE, 0xFF
    };
    
    ledBuff.alone = tab[i];   //���ж�Ӧֵ�͵�����LED����ʾ������
    if (i < (sizeof(tab)-1))  //��������ѭ������������������
        i++;
    else
        i = 0;
}

/* ���������ʾһλ���֣�index-�����λ����(���ҵ����Ӧ0��5)��
**     num-����ʾ�����֣�point-�����Ƿ���ʾ��λ�ϵ�С���� */
void ShowLedNumber(uint8 index, uint8 num, uint8 point)
{
    ledBuff.number[index] = LedChar[num];  //��������ת��Ϊ������ַ�0��F
    if (point != 0)
    {
        ledBuff.number[index] &= 0x7F;  //0111 1111,point��Ϊ0ʱ������ǰλ��С����
    }
}

/* ��������ʾһ֡ͼƬ��ptr-����ʾͼƬָ�� */
void ShowLedArray(uint8 *ptr)
{
    uint8 i;
    
    for (i=0; i<sizeof(ledBuff.array); i++)
    {
        ledBuff.array[i] = *ptr++;
    }
}
/* T2�жϷ�������LED��̬ɨ�衢���������� */
void InterruptTimer2() interrupt 5
{
    static uint8 i = 0;  //LEDλѡ����
    
    TF2 = 0;  //����T2�жϱ�־
    //ȫ��LED��̬ɨ����ʾ
    if (ENLED == 0)  //LEDʹ��ʱ�Ž��ж�̬ɨ��
    {
        P0 = 0xFF;                       //�ر����ж�ѡλ����ʾ����
        P1 = (P1 & 0xF0) | i;            //λѡ����ֵ��ֵ��P1�ڵ�4λ
        P0 = *((uint8 data*)&ledBuff+i); //������������λ�õ������͵�P0��
        if (i < (sizeof(ledBuff)-1))     //��������ѭ������������������
            i++;
        else
            i = 0;
    }
    //�ɷ�����״̬λ���Ʒ�����
    if (staBuzzer == 1)
        BUZZER = ~BUZZER;  //����������
    else
        BUZZER = 1;        //����������
}
