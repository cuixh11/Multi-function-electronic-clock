/*********************************************************************************** 
;功能说明:多功能电子钟
;文件名称:DS1302.c
;描    述：实时时钟芯片DS1302驱动模块的头文件     
;版本号：v1.0.0  
;微处理器:STC89C52
;编译环境:Keil uVision V4.10 
;作    者:Cui Xinghai 
;创建日期:2022.4.5   
;***********************************************************************************/


#ifndef _DS1302_H
#define _DS1302_H

struct sTime {  //日期时间结构
    uint16 year; //年
    uint8 mon;   //月
    uint8 day;   //日
    uint8 hour;  //时
    uint8 min;   //分
    uint8 sec;   //秒
    uint8 week;  //星期
};

#ifndef _DS1302_C

#endif

void InitDS1302();
void GetRealTime(struct sTime *time);
void SetRealTime(struct sTime *time);

#endif
