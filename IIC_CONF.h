#ifndef IIC_CONF_LLSH
#define IIC_CONF_LLSH

#define C51   //C51需要使用这个宏定义

#ifdef C51
#ifdef IIC
#include <reg52.h>
sbit	SCL = P2^1;			//SCL对应引脚更改
sbit	SDA = P2^0;			//SDA对应引脚更改
#endif
#endif

#define DELAY_TIME	150

#define SCL(x) 				SCL = x  //SCL引脚驱动宏定义更改
#define SDA(x) 				SDA = x	 //SDA引脚驱动宏定义更改
#define GET_SDA() 			SDA      //SDA引脚状态获取驱动宏定义更改处


#endif