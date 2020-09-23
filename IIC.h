#ifndef IIC_LLSH
#define IIC_LLSH

#ifdef __cplusplus
extern "C" {
#endif
	
#include "IIC_CONF.h"

#ifdef IIC
typedef unsigned char uchar;
typedef enum
{
	IIC_OK = 0,
	IIC_ERR0R = 1
}IIC_STATE;
#endif


void IIC_RECEIVE(uchar Address, uchar* adata, uchar DataSize);
void IIC_TRANSMISSION(uchar Address, uchar* adata,uchar DataSize);
void IIC_MEM_RECEIVE(uchar Address, uchar MemAddress,uchar* adata,uchar DataSize);
void IIC_MEM_TRANSMISSION(uchar Address, uchar MemAddress,uchar* adata,uchar DataSize);

#ifdef __cplusplus
}
#endif		

#endif
