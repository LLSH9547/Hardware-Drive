/*********************************************************************/
 /*
 *			TIME： 2020/09/23
 *
 *		AUTHOR： LLSH
 *
 *		 BRIEF： 本文件为IIC模拟驱动，
 *
 *       TIP： 1.如果驱动使用不正常可以对	Delayus 函数中的循环量大小进行提升
 *						 （上限255不够请自行增加循环量）；同理若驱动使用正常，也可以
 *					   通过适当减小循环量，以提高效率；
 *						 2.所有驱动移植更改只需要在	IIC_CONF	头文件中进行更改,包括上面
 *						 的延时更改.
 *
 */
/************************************************************************/



#define IIC
#include "IIC.h"




/****************************************/
/*

 * brief  ：延时函数函数

 * @param ：NOP

 * @retval：NOP

 */
/***************************************/

 void Delayus(void)		//@11.0592MHz
{
	unsigned int i;

	i = DELAY_TIME;
	while (--i);
}


/****************************************/
/*

 * brief  ：IIC开始函数

 * @param ：NOP

 * @retval：NOP

 */
/***************************************/

void IIC_START(void)
{
	SCL(0);
	Delayus();
	SDA(1);
	Delayus();
	SCL(1);
	Delayus();	
	SDA(0);
	Delayus();
	SCL(0);
	Delayus();
}


/****************************************/
/*

 * brief  ：IIC停止函数

 * @param ：NOP

 * @retval：NOP

 */
/***************************************/

void IIC_STOP(void)
{

	SDA(0);
	Delayus();
	SCL(1);
	Delayus();
	SDA(1);
	Delayus();
}


/****************************************/
/*

 * brief  ：IIC回应函数

 * @param ：NOP

 * @retval：NOP

 */
/***************************************/

void IIC_ACK(void)
{

	SDA(0);
	Delayus();
	SCL(1);
	Delayus();
	SCL(0);
	Delayus();

}


/****************************************/
/*

 * brief  ：IIC不回应函数

 * @param ：NOP

 * @retval：NOP

 */
/***************************************/

void IIC_NOACK(void)
{

	SDA(1);
	Delayus();
	SCL(1);
	Delayus();
	SCL(0);
	Delayus();
}


/****************************************/
/*

 * brief  ：IIC发送字节函数

 * @param ：adata 发送数据单位

 * @retval：NOP

 */
/***************************************/

IIC_STATE IIC_SEND_DATA(uchar adata)
{
	uchar number;
	for(number=0;number<8;number++)
	{
		SCL(0);
		Delayus();
		SDA = (adata<<number)&0x80;
		Delayus();
		SCL(1);	
		Delayus();	
	}	
	SCL(0);
	Delayus();
	SDA(1);
	Delayus();
	SCL(1);	
	Delayus();
	number = 0;
	while(SDA ==1){
		if(number>10)
		{			
			SCL(0); 
			return 1;		
		}
		Delayus();
		number++;
	}
	SCL(0);
	Delayus();
	return IIC_OK;
}


/****************************************/
/*

 * brief  ：IIC读取字节函数

 * @param ：adata 数据保存单位

 * @param ：end 数据尾部标志位(1为尾部，0为非尾部)

 * @retval：返回读取值

 */
/***************************************/

uchar IIC_READ_DATA(uchar *adata, uchar end)
{
	uchar number=0;

	SDA(1);
	Delayus();
	for(;number<8;number++)
	{
		*adata<<=1;
		SCL(1);	
		Delayus();	
		if(GET_SDA())
		{
			*adata+=1;
		}
		SCL(0);
		Delayus();
	}
	if(end == 0)
		IIC_ACK();
	else
		IIC_NOACK();
	return *adata;
}

/****************************************/
/*

 * brief  ：IIC读取函数

 * @param ：adata 数据保存单位

 * @param ：end 数据尾部标志位(1为尾部，0为非尾部)

 * @retval：返回读取值

 */
/***************************************/

void IIC_RECEIVE(uchar Address, uchar* adata, uchar DataSize)
{
	uchar loop_i = 0;
	IIC_START();
	while(IIC_SEND_DATA(Address) == 1);
	for(; loop_i<(DataSize-1); loop_i++)
		IIC_READ_DATA(&adata[loop_i],0);
	IIC_READ_DATA(&adata[DataSize-1],1);
	IIC_STOP();
}

/****************************************/
/*

 * brief  ：IIC读取函数

 * @param ：adata 数据保存单位

 * @param ：end 数据尾部标志位(1为尾部，0为非尾部)

 * @retval：返回读取值

 */
/***************************************/

void IIC_TRANSMISSION(uchar Address, uchar* adata,uchar DataSize)
{
	uchar loop_i = 0;
	IIC_START();
	while(IIC_SEND_DATA(Address) == 1);
	for(;loop_i<DataSize;loop_i++)
	{
		while(IIC_SEND_DATA(adata[loop_i]) == 1);
	}
	IIC_STOP();
}

/****************************************/
/*

 * brief  ：IIC内存读取函数

 * @param ：Address 设备地址（这里基于R/\W的规律，已经对读写地址
						的变化进行了考虑，实际使用只需填写设备地址，尾部位默认0）

 * @param ：MemAddress 内存地址

 * @param ：DataSize 数据个数

 * @retval：返回读取值

 */
/***************************************/

void IIC_MEM_RECEIVE(uchar Address, uchar MemAddress,uchar* adata,uchar DataSize)
{
	uchar loop_i = 0;
	IIC_START();
	while(IIC_SEND_DATA(Address) == 1);
	while(IIC_SEND_DATA(MemAddress) == 1);
	IIC_START();
	while(IIC_SEND_DATA(Address+1) == 1);
	for(; loop_i<DataSize-1; loop_i++)
		IIC_READ_DATA(&adata[loop_i],0);
	IIC_READ_DATA(&adata[DataSize-1],1);
	IIC_STOP();
}

/****************************************/
/*

 * brief  ：IIC发送函数

 * @param ：Address 设备地址（尾部位默认为0）

 * @param ：MemAddress 内存地址

 * @param ：DataSize 数据个数

 * @retval：返回读取值

 */
/***************************************/

void IIC_MEM_TRANSMISSION(uchar Address, uchar MemAddress,uchar* adata,uchar DataSize)
{
	uchar loop_i = 0;
	IIC_START();
	while(IIC_SEND_DATA(Address) == 1);
	while(IIC_SEND_DATA(MemAddress) == 1);
	for(;loop_i<DataSize;loop_i++)
	{
		while(IIC_SEND_DATA(adata[loop_i]) == 1);
	}
	IIC_STOP();
}