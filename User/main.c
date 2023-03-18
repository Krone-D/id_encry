#include "stm32f10x.h"
#include "./usart/bsp_usart.h"
#include "./chipid/bsp_chipid.h"
#include "AES.h"
#include "Utils.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#define UID_GROUP_SIZE      (16U)

extern uint32_t ChipUniqueID[3];

int main(void)
{	
	uint8_t uid[UID_GROUP_SIZE] = { 0 };
	uint8_t uid_encry[UID_GROUP_SIZE] = { 0 };
	uint8_t uid_decry[UID_GROUP_SIZE] = { 0 };
	/* USART1 config 115200 8-N-1 */
	USART_Config();
	/* to get the chipid and put it in ChipUniqueID[3] */
	get_chirpid();	
	/* printf the chipid */
	printf("chipid: 0x%08X-%08X-%08X\r\n",
	        ChipUniqueID[0],ChipUniqueID[1],ChipUniqueID[2]);
	/* 将数据的进行分割,分割为16字节的组 */
	memcpy((void*)uid, (void*)(&ChipUniqueID[0]), 4);
	memcpy((void*)(uid+4), (void*)(&ChipUniqueID[1]), 4);
	memcpy((void*)(uid+8), (void*)(&ChipUniqueID[2]), 4);
	memcpy((void*)(uid+12), (void*)(&ChipUniqueID[3]), 4);
	
	/* 进行加密 */
	aesEncrypt(uid, sizeof(uid)/sizeof(uid[0]), uid, uid_encry, sizeof(uid)/sizeof(uid[0]));
	/* 进行解密 */
	aesDecrypt(uid, sizeof(uid)/sizeof(uid[0]), uid_encry, uid_decry, sizeof(uid)/sizeof(uid[0]));
	/* 判断是否成功 */
	if(!memcmp((void*)uid_decry, (void*)uid, sizeof(uid)/sizeof(uid[0])));
		printf("ok\r\n");
	else
		printf("error\r\n");

	/* printf the flash memory amount */
	// printf("\r\n芯片flash的容量为: %dK \r\n", *(__IO u16 *)(0X1FFFF7E0));
  while(1){};
}
