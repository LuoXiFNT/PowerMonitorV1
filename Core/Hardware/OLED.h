#ifndef __OLED_H
#define __OLED_H

#include <stdint.h>
#include "OLED_Data.h"

/*参数宏定义*********************/

/*FontSize参数取值*/
/*此参数值不仅用于判断，而且用于计算横向字符偏移，默认值为字体像素宽度*/
#define OLED_8X16				8
#define OLED_6X8				6

/*IsFilled参数数值*/
#define OLED_UNFILLED			0
#define OLED_FILLED				1

/*I2C接口，定义OLED屏使用哪个I2C接口，根据自己的硬件配置进行修改*/
#define OLED_I2C            hi2c2
extern  I2C_HandleTypeDef   hi2c2;
/*OLED从机地址*/
#define OLED_ADDRESS 0x3C << 1	// 0x3C是OLED的7位地址，左移1位最后位做读写位变成0x78

/*DMA发送队列长度（节点数）*/
#define OLED_I2C_TX_QUEUE_LEN 96

/*DMA发送队列节点类型：命令*/
#define OLED_I2C_TX_TYPE_CMD  0U
/*DMA发送队列节点类型：数据*/
#define OLED_I2C_TX_TYPE_DATA 1U

/**
  * 结 构 体：OLED_I2C_TxNode_t
  * 说    明：OLED I2C DMA发送节点
  */
typedef struct
{
	uint8_t type;
	uint8_t cmd;
	uint8_t size;
	const uint8_t *data_ptr;
} OLED_I2C_TxNode_t;

/**
  * 结 构 体：OLED_I2C_Tx_Queue_t
  * 说    明：OLED I2C DMA发送队列上下文
  */
typedef struct
{
	OLED_I2C_TxNode_t queue[OLED_I2C_TX_QUEUE_LEN];
	uint8_t head;
	uint8_t tail;
	uint8_t busy;
	uint8_t work_buf[129];
} OLED_I2C_Tx_Queue_t;

/*********************参数宏定义*/


/*函数声明*********************/

/*初始化函数*/
void OLED_Init(void);

/*I2C DMA通信函数*/
uint8_t OLED_IsTxIdle(void);
uint8_t OLED_I2C_QueuePush(uint8_t type, uint8_t command, const uint8_t *data, uint8_t size);
void OLED_I2C_QueuePopCurrent(void);
void OLED_I2C_TryStartNext(void);
void OLED_I2C_OnTxCplt(void);
void OLED_I2C_OnError(void);

/*更新函数*/
void OLED_Update(void);
void OLED_UpdateArea(int16_t X, int16_t Y, uint8_t Width, uint8_t Height);

/*显存控制函数*/
void OLED_Clear(void);
void OLED_ClearArea(int16_t X, int16_t Y, uint8_t Width, uint8_t Height);
void OLED_Reverse(void);
void OLED_ReverseArea(int16_t X, int16_t Y, uint8_t Width, uint8_t Height);

/*显示函数*/
void OLED_ShowChar(int16_t X, int16_t Y, char Char, uint8_t FontSize);
void OLED_ShowString(int16_t X, int16_t Y, char *String, uint8_t FontSize);
void OLED_ShowNum(int16_t X, int16_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize);
void OLED_ShowSignedNum(int16_t X, int16_t Y, int32_t Number, uint8_t Length, uint8_t FontSize);
void OLED_ShowHexNum(int16_t X, int16_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize);
void OLED_ShowBinNum(int16_t X, int16_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize);
void OLED_ShowFloatNum(int16_t X, int16_t Y, double Number, uint8_t IntLength, uint8_t FraLength, uint8_t FontSize);
void OLED_ShowImage(int16_t X, int16_t Y, uint8_t Width, uint8_t Height, const uint8_t *Image);
void OLED_Printf(int16_t X, int16_t Y, uint8_t FontSize, char *format, ...);

/*绘图函数*/
void OLED_DrawPoint(int16_t X, int16_t Y);
uint8_t OLED_GetPoint(int16_t X, int16_t Y);
void OLED_DrawLine(int16_t X0, int16_t Y0, int16_t X1, int16_t Y1);
void OLED_DrawRectangle(int16_t X, int16_t Y, uint8_t Width, uint8_t Height, uint8_t IsFilled);
void OLED_DrawTriangle(int16_t X0, int16_t Y0, int16_t X1, int16_t Y1, int16_t X2, int16_t Y2, uint8_t IsFilled);
void OLED_DrawCircle(int16_t X, int16_t Y, uint8_t Radius, uint8_t IsFilled);
void OLED_DrawEllipse(int16_t X, int16_t Y, uint8_t A, uint8_t B, uint8_t IsFilled);
void OLED_DrawArc(int16_t X, int16_t Y, uint8_t Radius, int16_t StartAngle, int16_t EndAngle, uint8_t IsFilled);

/*********************函数声明*/

#endif


/*****************江协科技|版权所有****************/
/*****************jiangxiekeji.com*****************/
