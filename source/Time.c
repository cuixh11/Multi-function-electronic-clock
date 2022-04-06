/*********************************************************************************** 
;����˵��:�๦�ܵ�����
;�ļ�����:Time.c  
; ��   ����������Ҫ�����ļ�
;�汾�ţ�v1.0.0  
;΢������:STC89C52
;���뻷��:Keil uVision V4.10 
;��    ��:Cui Xinghai 
;��������:2022.4.5   
;***********************************************************************************/


#define  _TIME_C
#include "config.h"
#include "DS1302.h"
#include "LedBuzzer.h"
#include "Lcd1602.h"
#include "Time.h"
#include "main.h"

uint8 code WeekMod[] = {  //���������X�ַ�ͼƬ��
    0xFF, 0x99, 0x00, 0x00, 0x00, 0x81, 0xC3, 0xE7,  //������(����)
    0xEF, 0xE7, 0xE3, 0xE7, 0xE7, 0xE7, 0xE7, 0xC3,  //����1
    0xC3, 0x81, 0x9D, 0x87, 0xC3, 0xF9, 0xC1, 0x81,  //����2
    0xC3, 0x81, 0x9D, 0xC7, 0xC7, 0x9D, 0x81, 0xC3,  //����3
    0xCF, 0xC7, 0xC3, 0xC9, 0xC9, 0x81, 0xCF, 0xCF,  //����4
    0x81, 0xC1, 0xF9, 0xC3, 0x87, 0x9D, 0x81, 0xC3,  //����5
    0xC3, 0x81, 0xF9, 0xC3, 0x81, 0x99, 0x81, 0xC3,  //����6
    0xFF, 0x99, 0x00, 0x00, 0x00, 0x81, 0xC3, 0xE7,  //������
};

bit staMute = 0;  //������־λ
uint8 AlarmHour = 0x14;  //����ʱ���Сʱ��
uint8 AlarmMin  = 0x01;  //����ʱ��ķ�����
struct sTime CurTime;    //��ǰ����ʱ��

uint8 SetIndex = 0;  //����λ����
uint8 pdata SetAlarmHour;    //����Сʱ�����û���
uint8 pdata SetAlarmMin;     //���ӷ��������û���
struct sTime pdata SetTime;  //����ʱ�����û�����

/* ��ȡ��ǰ����ʱ�䣬��ˢ��ʱ������ڵ���ʾ */
void RefreshTime()
{
    GetRealTime(&CurTime);                  //��ȡ��ǰ����ʱ��
    ShowLedNumber(5, CurTime.hour>>4, 0);   //ʱ
    ShowLedNumber(4, CurTime.hour&0xF,1);
    ShowLedNumber(3, CurTime.min>>4,  0);   //��
    ShowLedNumber(2, CurTime.min&0xF, 1);
    ShowLedNumber(1, CurTime.sec>>4,  0);   //��
    ShowLedNumber(0, CurTime.sec&0xF, 0);
    ShowLedArray(WeekMod + CurTime.week*8); //����
}
/* ����ˢ�º�����ops-ˢ��ѡ�Ϊ0ʱֻ�����ڱ仯��ˢ�£���0������ˢ�� */
void RefreshDate(uint8 ops)
{
    uint8 pdata str[12];
    static uint8 backup = 0;
    
    if ((backup!=CurTime.day) || (ops!=0))
    {
        str[0] = ((CurTime.year>>12) & 0xF) + '0';  //4λ�����	& 0x0000 1111
        str[1] = ((CurTime.year>>8) & 0xF) + '0';
        str[2] = ((CurTime.year>>4) & 0xF) + '0';
        str[3] = (CurTime.year & 0xF) + '0';
        str[4] = '-';                        //�ָ���
        str[5] = (CurTime.mon >> 4) + '0';   //�·�
        str[6] = (CurTime.mon & 0xF) + '0';
        str[7] = '-';                        //�ָ���
        str[8] = (CurTime.day >> 4) + '0';   //����
        str[9] = (CurTime.day & 0xF) + '0';
        str[10] = '\0';         //�ַ���������
        LcdShowStr(0, 0, str);  //��ʾ��Һ����
        backup = CurTime.day;   //ˢ���ϴ�����ֵ
    }
}
/* ˢ������ʱ�����ʾ */
void RefreshAlarm()
{
    uint8 pdata str[8];
    
    LcdShowStr(0, 1, "Alarm at ");     //��ʾ��ʾ����
    str[0] = (AlarmHour >> 4) + '0';   //����Сʱ��
    str[1] = (AlarmHour & 0xF) + '0';
    str[2] = ':';                      //�ָ���
    str[3] = (AlarmMin >> 4) + '0';    //���ӷ�����
    str[4] = (AlarmMin & 0xF) + '0';
    str[5] = '\0';                     //�ַ���������
    LcdShowStr(9, 1, str);             //��ʾ��Һ����
}
/* ���Ӽ�غ������ִ��趨������ʱ��ʱִ������ */
void AlarmMonitor()
{
    if ((CurTime.hour==AlarmHour) && (CurTime.min==AlarmMin)) //���ʱ��ƥ��
    {
        if (!staMute)  //����Ƿ���
            staBuzzer = ~staBuzzer;  //ʵ�ַ�������������
        else
            staBuzzer = 0;
    }
    else
    {
        staMute = 0;
        staBuzzer = 0;
    }
}
/* ������ʱ�估������ʾ��ʾ��Һ���� */
void ShowSetTime()
{
    uint8 pdata str[18];
    
    str[0]  = ((SetTime.year>>4) & 0xF) + '0';  //2λ�����
    str[1]  = (SetTime.year & 0xF) + '0';
    str[2]  = '-';
    str[3]  = (SetTime.mon >> 4) + '0';   //�·�
    str[4]  = (SetTime.mon & 0xF) + '0';
    str[5]  = '-';
    str[6]  = (SetTime.day >> 4) + '0';   //����
    str[7]  = (SetTime.day & 0xF) + '0';
    str[8]  = '-';
    str[9]  = (SetTime.week & 0xF) + '0'; //����
    str[10] = ' ';
    str[11] = (SetTime.hour >> 4) + '0';  //Сʱ
    str[12] = (SetTime.hour & 0xF) + '0';
    str[13] = ':';
    str[14] = (SetTime.min >> 4) + '0';   //����
    str[15] = (SetTime.min & 0xF) + '0';
    str[16] = '\0';
    LcdShowStr(0, 0, "Set Date Time");  //��ʾ��ʾ����
    LcdShowStr(0, 1, str);              //��ʾ����ʱ��ֵ
}
/* ���������Ӽ�������ʾ��ʾ��Һ���� */
void ShowSetAlarm()
{
    uint8 pdata str[8];
    
    str[0] = (SetAlarmHour >> 4) + '0';   //Сʱ
    str[1] = (SetAlarmHour & 0xF) + '0';
    str[2] = ':';
    str[3] = (SetAlarmMin >> 4) + '0';    //����
    str[4] = (SetAlarmMin & 0xF) + '0';
    str[5] = '\0';
    LcdShowStr(0, 0, "Set Alarm");  //��ʾ��ʾ����
    LcdShowStr(0, 1, str);          //��ʾ�趨����ֵ
}
/* ȡ����ǰ���ã�������������״̬ */
void CancelCurSet()
{
    staSystem = E_NORMAL;
    LcdCloseCursor();  //�رչ��
    LcdClearScreen();  //Һ������
    RefreshTime();   //ˢ�µ�ǰʱ��
    RefreshDate(1);  //����ˢ��������ʾ
    RefreshTemp(1);  //����ˢ���¶���ʾ
    RefreshAlarm();  //�����趨ֵ��ʾ
}
/* ʱ�����������ʱ������λ����һλ����ͷ���ۻ� */
void SetRightShift()
{
    if (staSystem == E_SET_TIME)
    {
        switch (SetIndex)
        {
            case 0: SetIndex=1;  LcdSetCursor(1, 1); break;
            case 1: SetIndex=2;  LcdSetCursor(3, 1); break;
            case 2: SetIndex=3;  LcdSetCursor(4, 1); break;
            case 3: SetIndex=4;  LcdSetCursor(6, 1); break;
            case 4: SetIndex=5;  LcdSetCursor(7, 1); break;
            case 5: SetIndex=6;  LcdSetCursor(9, 1); break;
            case 6: SetIndex=7;  LcdSetCursor(11,1); break;
            case 7: SetIndex=8;  LcdSetCursor(12,1); break;
            case 8: SetIndex=9;  LcdSetCursor(14,1); break;
            case 9: SetIndex=10; LcdSetCursor(15,1); break;
            default: SetIndex=0; LcdSetCursor(0, 1); break;
        }
    }
    else if (staSystem == E_SET_ALARM)
    {
        switch (SetIndex)
        {
            case 0: SetIndex=1;  LcdSetCursor(1,1); break;
            case 1: SetIndex=2;  LcdSetCursor(3,1); break;
            case 2: SetIndex=3;  LcdSetCursor(4,1); break;
            default: SetIndex=0; LcdSetCursor(0,1); break;
        }
    }
}
/* ʱ�����������ʱ������λ����һλ����ͷ���ۻ� */
void SetLeftShift()
{
    if (staSystem == E_SET_TIME)
    {
        switch (SetIndex)
        {
            case 0: SetIndex=10; LcdSetCursor(15,1); break;
            case 1: SetIndex=0;  LcdSetCursor(0, 1); break;
            case 2: SetIndex=1;  LcdSetCursor(1, 1); break;
            case 3: SetIndex=2;  LcdSetCursor(3, 1); break;
            case 4: SetIndex=3;  LcdSetCursor(4, 1); break;
            case 5: SetIndex=4;  LcdSetCursor(6, 1); break;
            case 6: SetIndex=5;  LcdSetCursor(7, 1); break;
            case 7: SetIndex=6;  LcdSetCursor(9, 1); break;
            case 8: SetIndex=7;  LcdSetCursor(11,1); break;
            case 9: SetIndex=8;  LcdSetCursor(12,1); break;
            default: SetIndex=9; LcdSetCursor(14,1); break;
        }
    }
    else if (staSystem == E_SET_ALARM)
    {
        switch (SetIndex)
        {
            case 0: SetIndex=3;  LcdSetCursor(4,1); break;
            case 1: SetIndex=0;  LcdSetCursor(0,1); break;
            case 2: SetIndex=1;  LcdSetCursor(1,1); break;
            default: SetIndex=2; LcdSetCursor(3,1); break;
        }
    }
}
/* �����������֣��޸Ķ�Ӧ������λ������ʾ�����֣�ascii-�������ֵ�ASCII�� */
void InputSetNumber(uint8 ascii)
{
    uint8 num;
    
    num = ascii - '0';
    if (num <= 9)  //ֻ��Ӧ0��9������
    {
        if (staSystem == E_SET_TIME)
        {
            switch (SetIndex)
            {
                case 0: SetTime.year = (SetTime.year&0xFF0F)|(num<<4);
                        LcdShowChar(0, 1, ascii);  break;      //��ݸ�λ����
                case 1: SetTime.year = (SetTime.year&0xFFF0)|(num);
                        LcdShowChar(1, 1, ascii);  break;      //��ݵ�λ����
                case 2: SetTime.mon = (SetTime.mon&0x0F)|(num<<4);
                        LcdShowChar(3, 1, ascii);  break;      //�·ݸ�λ����
                case 3: SetTime.mon = (SetTime.mon&0xF0)|(num);
                        LcdShowChar(4, 1, ascii);  break;      //�·ݵ�λ����
                case 4: SetTime.day = (SetTime.day&0x0F)|(num<<4);
                        LcdShowChar(6, 1, ascii);  break;      //���ڸ�λ����
                case 5: SetTime.day = (SetTime.day&0xF0)|(num);
                        LcdShowChar(7, 1, ascii);  break;      //���ڵ�λ����
                case 6: SetTime.week = (SetTime.week&0xF0)|(num);
                        LcdShowChar(9, 1, ascii);  break;      //��������
                case 7: SetTime.hour = (SetTime.hour&0x0F)|(num<<4);
                        LcdShowChar(11,1, ascii);  break;      //Сʱ��λ����
                case 8: SetTime.hour = (SetTime.hour&0xF0)|(num);
                        LcdShowChar(12,1, ascii);  break;      //Сʱ��λ����
                case 9: SetTime.min = (SetTime.min&0x0F)|(num<<4);
                        LcdShowChar(14,1, ascii);  break;      //���Ӹ�λ����
                default:SetTime.min = (SetTime.min&0xF0)|(num);
                        LcdShowChar(15,1, ascii);  break;      //���ӵ�λ����
            }
            SetRightShift();  //��ɸ�λ���ú��Զ�����
        }
        else if (staSystem == E_SET_ALARM)
        {
            switch (SetIndex)
            {
                case 0: SetAlarmHour = (SetAlarmHour&0x0F) | (num<<4);
                        LcdShowChar(0,1, ascii); break;      //Сʱ��λ����
                case 1: SetAlarmHour = (SetAlarmHour&0xF0) | (num);
                        LcdShowChar(1,1, ascii); break;      //Сʱ��λ����
                case 2: SetAlarmMin = (SetAlarmMin&0x0F) | (num<<4);
                        LcdShowChar(3,1, ascii); break;      //���Ӹ�λ����
                default:SetAlarmMin = (SetAlarmMin&0xF0) | (num);
                        LcdShowChar(4,1, ascii); break;      //���ӵ�λ����
            }
            SetRightShift();  //��ɸ�λ���ú��Զ�����
        }
    }
}
/* �л�ϵͳ����״̬ */
void SwitchSystemSta()
{
    if (staSystem == E_NORMAL)  //���������л���ʱ������
    {
        staSystem = E_SET_TIME;
        SetTime.year = CurTime.year;  //��ǰʱ�俽����ʱ�����û�������
        SetTime.mon  = CurTime.mon;
        SetTime.day  = CurTime.day;
        SetTime.hour = CurTime.hour;
        SetTime.min  = CurTime.min;
        SetTime.sec  = CurTime.sec;
        SetTime.week = CurTime.week;
        LcdClearScreen();  //Һ������
        ShowSetTime();     //��ʾ����ʱ��
        SetIndex = 255;    //�������������һ�𽫹����������ߵ�λ����
        SetRightShift();
        LcdOpenCursor();   //�������
    }
    else if (staSystem == E_SET_TIME)  //ʱ�������л�����������
    {
        staSystem = E_SET_ALARM;
        SetTime.sec = 0;          //�����㣬��������ʱ����0�뿪ʼ��ʱ
        SetRealTime(&SetTime);    //�趨ʱ��д��ʵʱʱ��
        SetAlarmHour = AlarmHour; //��ǰ����ֵ���������û�����
        SetAlarmMin  = AlarmMin;
        LcdClearScreen();  //Һ������
        ShowSetAlarm();    //��ʾ��������
        SetIndex = 255;    //�������������һ�𽫹����������ߵ�λ����
        SetRightShift();
    }
    else  //���������л�����������
    {
        staSystem = E_NORMAL;
        AlarmHour = SetAlarmHour;  //�趨������ֵд������ʱ��
        AlarmMin  = SetAlarmMin;
        LcdCloseCursor();  //�رչ��
        LcdClearScreen();  //Һ������
        RefreshTime();   //ˢ�µ�ǰʱ��
        RefreshDate(1);  //����ˢ��������ʾ
        RefreshTemp(1);  //����ˢ���¶���ʾ
        RefreshAlarm();  //�����趨ֵ��ʾ
    }
}
/* �����������������ݼ���ִ����Ӧ�Ĳ�����keycode-�������� */
void KeyAction(uint8 keycode)
{
    if  ((keycode>='0') && (keycode<='9'))  //���ּ����뵱ǰλ�趨ֵ
    {
        InputSetNumber(keycode);
    }
    else if (keycode == 0x25)  //������������л�����λ
    {
        SetLeftShift();
    }
    else if (keycode == 0x27)  //���Ҽ��������л�����λ
    {
        SetRightShift();
    }
    else if (keycode == 0x0D)  //�س������л�����״̬/��������
    {
        SwitchSystemSta();
    }
    else if (keycode == 0x1B)  //Esc��������/ȡ����ǰ����
    {
        if (staSystem == E_NORMAL) //������������״̬ʱ���徲��
        {
            staMute = 1;
        }
        else                       //��������״̬ʱ�˳�����
        {
            CancelCurSet();
        }
    }
}
