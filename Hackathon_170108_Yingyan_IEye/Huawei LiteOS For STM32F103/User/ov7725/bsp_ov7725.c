#include "bsp_ov7725.h"
#include "bsp_sccb.h"
#include "bsp_ili9341_lcd.h"
#include "los_hwi.h"
#include "los_typedef.h"

#include "los_membox.h"
#include "los_list.h" 
#include "los_api_list.h"
#include <stdio.h> 
#include <stdlib.h> 
#include "Msg.h"

extern char abuf[50];
extern int SendToMsg(Msg_SendInfoBuf *pMsgPass);

#define HWI_NUM_INT50 50

typedef struct Reg
{
	uint8_t Address;			       /*寄存器地址*/
	uint8_t Value;		           /*寄存器值*/
}Reg_Info;

/* 寄存器参数配置 */
Reg_Info Sensor_Config[] =
{
	{CLKRC,     0x00}, /*clock config*/
	{COM7,      0x46}, /*QVGA RGB565 */
  {HSTART,    0x3f},
	{HSIZE,     0x50},
	{VSTRT,     0x03},
	{VSIZE,     0x78},
	{HREF,      0x00},
	{HOutSize,  0x50},
	{VOutSize,  0x78},
	{EXHCH,     0x00},

	/*DSP control*/
	{TGT_B,     0x7f},
	{FixGain,   0x09},
	{AWB_Ctrl0, 0xe0},
	{DSP_Ctrl1, 0xff},
	{DSP_Ctrl2, 0x20},
	{DSP_Ctrl3,	0x00},
	{DSP_Ctrl4, 0x00},

	/*AGC AEC AWB*/
	{COM8,		  0xf0},
	{COM4,		  0x81}, /*Pll AEC CONFIG*/
	{COM6,		  0xc5},
	{COM9,		  0x21},
	{BDBase,	  0xFF},
	{BDMStep,	  0x01},
	{AEW,		    0x34},
	{AEB,		    0x3c},
	{VPT,		    0xa1},
	{EXHCL,		  0x00},
	{AWBCtrl3,  0xaa},
	{COM8,		  0xff},
	{AWBCtrl1,  0x5d},

	{EDGE1,		  0x0a},
	{DNSOff,	  0x01},
	{EDGE2,		  0x01},
	{EDGE3,		  0x01},

	{MTX1,		  0x5f},
	{MTX2,		  0x53},
	{MTX3,		  0x11},
	{MTX4,		  0x1a},
	{MTX5,		  0x3d},
	{MTX6,		  0x5a},
	{MTX_Ctrl,  0x1e},

	{BRIGHT,	  0x00},
	{CNST,		  0x25},
	{USAT,		  0x65},
	{VSAT,		  0x65},
	{UVADJ0,	  0x81},
	{SDE,		    0x06},
	
    /*GAMMA config*/
	{GAM1,		  0x0c},
	{GAM2,		  0x16},
	{GAM3,		  0x2a},
	{GAM4,		  0x4e},
	{GAM5,		  0x61},
	{GAM6,		  0x6f},
	{GAM7,		  0x7b},
	{GAM8,		  0x86},
	{GAM9,		  0x8e},
	{GAM10,		  0x97},
	{GAM11,		  0xa4},
	{GAM12,		  0xaf},
	{GAM13,		  0xc5},
	{GAM14,		  0xd7},
	{GAM15,		  0xe8},
	{SLOP,		  0x20},

	{HUECOS,	  0x80},
	{HUESIN,	  0x80},
	{DSPAuto,	  0xff},
	{DM_LNL,	  0x00},
	{BDBase,	  0x99},
	{BDMStep,	  0x03},
	{LC_RADI,	  0x00},
	{LC_COEF,	  0x13},
	{LC_XC,		  0x08},
	{LC_COEFB,  0x14},
	{LC_COEFR,  0x17},
	{LC_CTR,	  0x05},
	
	{COM3,		  0xd0},/*Horizontal mirror image*/

	/*night mode auto frame rate control*/
	{COM5,		0xf5},	 /*在夜视环境下，自动降低帧率，保证低照度画面质量*/
	//{COM5,		0x31},	/*夜视环境帧率不变*/
};

uint8_t OV7725_REG_NUM = sizeof(Sensor_Config)/sizeof(Sensor_Config[0]);	  /*结构体数组成员数目*/

volatile uint8_t Ov7725_vsync ;	 /* 帧同步信号标志，在中断函数和main函数里面使用 */


/************************************************
 * 函数名：FIFO_GPIO_Config
 * 描述  ：FIFO GPIO配置
 * 输入  ：无
 * 输出  ：无
 * 注意  ：无
 ************************************************/
static void FIFO_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;	

	  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
		/*PD3(FIFO_WEN--FIFO写使能)*/
	  macOV7725_WE_SCK_APBxClock_FUN ( macOV7725_WE_GPIO_CLK, ENABLE );
		GPIO_InitStructure.GPIO_Pin = macOV7725_WE_GPIO_PIN;
		GPIO_Init(macOV7725_WE_GPIO_PORT, &GPIO_InitStructure);
	
		/*PB5(FIFO_WRST--FIFO写复位)*/
		macOV7725_WRST_SCK_APBxClock_FUN ( macOV7725_WRST_GPIO_CLK, ENABLE );
		GPIO_InitStructure.GPIO_Pin = macOV7725_WRST_GPIO_PIN;
		GPIO_Init(macOV7725_WRST_GPIO_PORT, &GPIO_InitStructure);
	
		/*PA2(FIFO_RRST--FIFO读复位) PA3(FIFO_OE--FIFO输出使能)*/
    macOV7725_RRST_SCK_APBxClock_FUN ( macOV7725_RRST_GPIO_CLK, ENABLE );
		GPIO_InitStructure.GPIO_Pin = macOV7725_RRST_GPIO_PIN;
		GPIO_Init(macOV7725_RRST_GPIO_PORT, &GPIO_InitStructure);
		
		macOV7725_CS_SCK_APBxClock_FUN ( macOV7725_CS_GPIO_CLK, ENABLE );
		GPIO_InitStructure.GPIO_Pin = macOV7725_CS_GPIO_PIN;
		GPIO_Init(macOV7725_CS_GPIO_PORT, &GPIO_InitStructure);
		
		/*PC5(FIFO_RCLK-FIFO读时钟)*/
		macOV7725_RCLK_SCK_APBxClock_FUN ( macOV7725_RCLK_GPIO_CLK, ENABLE );
		GPIO_InitStructure.GPIO_Pin = macOV7725_RCLK_GPIO_PIN;
		GPIO_Init(macOV7725_RCLK_GPIO_PORT, &GPIO_InitStructure);

    /*PB8-PB15(FIFO_DATA--FIFO输出数据)*/
		macOV7725_DATA_SCK_APBxClock_FUN ( macOV7725_DATA_GPIO_CLK, ENABLE );
		GPIO_InitStructure.GPIO_Pin = macOV7725_DATA_0_GPIO_PIN | macOV7725_DATA_1_GPIO_PIN | macOV7725_DATA_2_GPIO_PIN | macOV7725_DATA_3_GPIO_PIN |
		                              macOV7725_DATA_4_GPIO_PIN | macOV7725_DATA_5_GPIO_PIN | macOV7725_DATA_6_GPIO_PIN | macOV7725_DATA_7_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(macOV7725_DATA_GPIO_PORT, &GPIO_InitStructure);
		
		
    FIFO_CS_L();	  					/*拉低使FIFO输出使能*/
    FIFO_WE_H();   						/*拉高使FIFO写允许*/
		
		
}

void Ov7725_GPIO_Config(void)
{
	SCCB_GPIO_Config();
	FIFO_GPIO_Config();
}

/************************************************
 * 函数名：VSYNC_GPIO_Configuration
 * 描述  ：OV7725 GPIO配置
 * 输入  ：无
 * 输出  ：无
 * 注意  ：无
 ************************************************/
static void VSYNC_GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	
	
	  macOV7725_VSYNC_SCK_APBxClock_FUN ( macOV7725_VSYNC_GPIO_CLK, ENABLE );	  /*PA0---VSYNC*/
    GPIO_InitStructure.GPIO_Pin =  macOV7725_VSYNC_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(macOV7725_VSYNC_GPIO_PORT, &GPIO_InitStructure);
}

/************************************************
 * 函数名：VSYNC_NVIC_Configuration
 * 描述  ：VSYNC中断配置
 * 输入  ：无
 * 输出  ：无
 * 注意  ：无
 ************************************************/

/* ov7725 场中断 服务程序 */
void macOV7725_VSYNC_EXTI_INT_FUNCTION_IEye (void)
{
    if ( EXTI_GetITStatus(macOV7725_VSYNC_EXTI_LINE) != RESET ) 	//检查EXTI_Line0线路上的中断请求是否发送到了NVIC 
    {
        if( Ov7725_vsync == 0 )
        {
            FIFO_WRST_L(); 	                      //拉低使FIFO写(数据from摄像头)指针复位
            FIFO_WE_H();	                        //拉高使FIFO写允许
            
            Ov7725_vsync = 1;	   	
            FIFO_WE_H();                          //使FIFO写允许
            FIFO_WRST_H();                        //允许使FIFO写(数据from摄像头)指针运动
        }
        else if( Ov7725_vsync == 1 )
        {
            FIFO_WE_L();                          //拉低使FIFO写暂停
            Ov7725_vsync = 2;
        }        
        EXTI_ClearITPendingBit(macOV7725_VSYNC_EXTI_LINE);		    //清除EXTI_Line0线路挂起标志位        
    }    
}
extern void Example_Exti3_Init();
static void VSYNC_NVIC_Configuration(void)
{
    UINTPTR uvIntSave;
    uvIntSave = LOS_IntLock();
//    Example_Exti3_Init();
    LOS_HwiCreate(9, 0,0,macOV7725_VSYNC_EXTI_INT_FUNCTION_IEye,0);//
    LOS_IntRestore(uvIntSave);
		return;
	
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    NVIC_InitStructure.NVIC_IRQChannel = macOV7725_VSYNC_EXTI_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/************************************************
 * 函数名：VSYNC_EXTI_Configuration
 * 描述  ：OV7725 VSYNC中断管脚配置
 * 输入  ：无
 * 输出  ：无
 * 注意  ：无
 ************************************************/
/*               ___                            ___
 * VSYNC:     __|   |__________________________|   |__     
 */
static void VSYNC_EXTI_Configuration(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;

    GPIO_EXTILineConfig(macOV7725_VSYNC_EXTI_SOURCE_PORT, macOV7725_VSYNC_EXTI_SOURCE_PIN);
    EXTI_InitStructure.EXTI_Line = macOV7725_VSYNC_EXTI_LINE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    //EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising ; /*上升沿触发*/
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling ; 
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    EXTI_GenerateSWInterrupt(macOV7725_VSYNC_EXTI_LINE);	/*中断挂到 EXTI_Line0  线*/
}

/************************************************
 * 函数名：VSYNC_Init
 * 描述  ：OV7725 VSYNC中断相关配置
 * 输入  ：无
 * 输出  ：无
 * 注意  ：无
 ************************************************/
void VSYNC_Init(void)
{
    VSYNC_GPIO_Configuration();
    VSYNC_EXTI_Configuration();
    VSYNC_NVIC_Configuration();
}

/************************************************
 * 函数名：Sensor_Init
 * 描述  ：Sensor初始化
 * 输入  ：无
 * 输出  ：返回1成功，返回0失败
 * 注意  ：无
 ************************************************/
ErrorStatus Ov7725_Init(void)
{
	uint16_t i = 0;
	uint8_t Sensor_IDCode = 0;	
	
	//DEBUG("ov7725 Register Config Start......");
	
	if( 0 == SCCB_WriteByte ( 0x12, 0x80 ) ) /*复位sensor */
	{
		//DEBUG("sccb write data error");		
		return ERROR ;
	}	

	if( 0 == SCCB_ReadByte( &Sensor_IDCode, 1, 0x0b ) )	 /* 读取sensor ID号*/
	{
		//DEBUG("read id faild");		
		return ERROR;
	}
	//DEBUG("Sensor ID is 0x%x", Sensor_IDCode);	
	
	if(Sensor_IDCode == OV7725_ID)
	{
		for( i = 0 ; i < OV7725_REG_NUM ; i++ )
		{
			if( 0 == SCCB_WriteByte(Sensor_Config[i].Address, Sensor_Config[i].Value) )
			{                
				//DEBUG("write reg faild", Sensor_Config[i].Address);
				return ERROR;
			}
		}
	}
	else
	{
		return ERROR;
	}
	//DEBUG("ov7725 Register Config Success");
	
	return SUCCESS;
}


//static  uint16_t *p_num = NULL;
//static  uint16_t uwBlkSize = 10, uwBoxSize = 100;
//#if (LOSCFG_BASE_MEM_NODE_INTEGRITY_CHECK == YES)
//UINT8 pBoxMem[sizeof(OS_MEMBOX_S) + ((80 + 4 + 3) & (~3)) * 40 + 4];
//#else
//UINT8 pBoxMem[sizeof(OS_MEMBOX_S) + ((80 + 3) & (~3)) * 40];
//#endif
/*       320
 * -------------------
 *|                   |
 *|                   |
 *|                   |  240
 *|                   |
 *|                   |
 * -------------------
 */

uint16_t pBuf[80][80];
uint16_t pBufOld1[76][76];
static int iCount = 0;

void ImgDeal()
{
	int i = 0;
	int j = 0;
	
	char str[10];
	UINT32 uwRet = LOS_OK;
	UINT32 count=0, iSend=0;
	UINT32 uwlen = sizeof(abuf);	
	count++;
	
	sprintf(str,"%d",count);
	
	if(iCount == 0)
	{
		for(i = 0; i < 76; i++)
		{
			for(j = 0; j < 76; j++)
			{
				pBufOld1[i][j] = 0;
			}
		}
	}
	
	uint16_t pArrMao[25];
	uint16_t iTmpMao = 0;
	for(i = 2; i < 78;i++)
	{
		for(j = 2; i < 78;i++)
		{
			for(int i5 = 0; i5 < 5; i5++)
			{
				for(int j5 = 0; j5 < 5; j5++)
				{
					pArrMao[j5 + i5 * 5] = pBuf[i - 2 + i5][j - 2 + j5];
				}
			}
			for(int iMao = 0; iMao < 25; iMao++)
			{
				for(int jMao = iMao; jMao < 25; jMao++)
				{
					if(pArrMao[jMao] > pArrMao[jMao + 1])
					{
						iTmpMao = pArrMao[jMao + 1];
						pArrMao[jMao + 1] = pArrMao[jMao];
						pArrMao[jMao] = iTmpMao;
					}
				}
			}
			pBuf[i][j] = pArrMao[12];
		}
	}
	
	for(i = 2; i < 78;i++)
	{
		for(j = 2; i < 78;i++)
		{
			uint16_t iRec = pBuf[i][j] - pBufOld1[i][j];
			pBufOld1[i][j] = pBuf[i][j];
			pBuf[i][j] = iRec;
		}
	}
	float fCountRes = 0;
	for(i = 0; i < 240; i++)
	{
		for(j = 0; j < 320; j++)
		{
			if(((i > 1)&&(i < 79)) && ((j < 79)&&(j > 1)))
			{
				//fCountRes += pBuf[i][j] / (78 * 78);
				//ILI9341_Write_Data(pBuf[i][j]);
			}
			else
			{
				//ILI9341_Write_Data(0);
			}
		}
	}
	if(iCount != 0)
	{
		for(i = 0; i < 80; i++)
		{
			for(j = 0; j < 80; j++)
			{
				pBufOld1[i][j] = pBuf[i][j];
			}
		}
	}
	
	abuf[uwlen -2] = '0' + i;
	i++;
	Msg_SendInfoBuf msgBuf;
	msgBuf.sStayTime = 3;
	memcpy(msgBuf.cProductrID,"IEyePro",16);
	msgBuf.ucPkgNum = 1;
	msgBuf.ucIsEnd = 1;
	uwRet = SendToMsg(&msgBuf);
	if(uwRet != LOS_OK)
	{
			uwRet = ERROR;
	}
}

void ImagDisp(void)
{
		uint16_t i, j;
		uint16_t Camera_Data;
		uint16_t Camera_Data1;
		uint16_t Camera_Data2;
		uint16_t Camera_Data3;
	
	for(i = 0; i < 80; i++)
	{
		for(j = 0; j < 80; j++)
		{
			pBuf[i][j] = 0;
		}
	}
//	  UINT32 uwRet;
//    uwRet = LOS_MemboxInit( pBoxMem, sizeof(pBoxMem), 80);
//    if(uwRet != LOS_OK)
//    {
//        dprintf("Mem box init failed\n");
//        return;
//    }
//    else
//    {
//        dprintf("Mem box init ok!\n");
//    }

//    p_num = (uint16_t*)LOS_MemboxAlloc(pBoxMem);

//    if (NULL == p_num) 
//    {
//        dprintf("Mem box alloc failed!\n");
//        return;
//    }
//    dprintf("Mem box alloc ok\n");
		
//	  LOS_DL_LIST* head;
//		head = (LOS_DL_LIST*)malloc(sizeof(LOS_DL_LIST));
//	
//		LOS_ListInit(head);
//    if (!LOS_ListEmpty(head))
//    {
//        printf("initial failed\n");
//        return;
//    }
//		
//		LOS_DL_LIST* node1 = (LOS_DL_LIST*)malloc(sizeof(LOS_DL_LIST));
//    LOS_DL_LIST* node2 = (LOS_DL_LIST*)malloc(sizeof(LOS_DL_LIST));
//    LOS_DL_LIST* tail = (LOS_DL_LIST*)malloc(sizeof(LOS_DL_LIST));
//		
//    LOS_ListAdd(head,node1);
//    LOS_ListAdd(node1,node2);
//    if((node1->pstPrev == head) || (node2->pstPrev == node1))
//    {
//        printf("add node success\n");
//    }
// 
//    LOS_ListTailInsert(head,tail);
//    if(tail->pstPrev == node2)
//    {
//        printf("add tail success\n");
//    }
// 
//    printf("delete node......\n");
//    LOS_ListDelete(node1);
//    free(node1);
//    if(head->pstNext == node2)
//    {
//        printf("delete node success\n");
//    }


	/* 设置液晶扫描方向为 左下角->右上角 */
	ILI9341_GramScan( 2 );
	
	for(i = 0; i < 80; i++)
	{		
		for(j = 0; j < 320; j++)
		{
			READ_FIFO_PIXEL(Camera_Data);		/* 从FIFO读出一个rgb565像素到Camera_Data变量 */
				ILI9341_Write_Data(Camera_Data);
		}
		for(j = 0; j < 80; j++)
		{
			READ_FIFO_PIXEL(Camera_Data);		/* 从FIFO读出一个rgb565像素到Camera_Data变量 */
				ILI9341_Write_Data(Camera_Data);
			READ_FIFO_PIXEL(Camera_Data1);		/* 从FIFO读出一个rgb565像素到Camera_Data变量 */
				ILI9341_Write_Data(Camera_Data);
			READ_FIFO_PIXEL(Camera_Data2);		/* 从FIFO读出一个rgb565像素到Camera_Data变量 */
				ILI9341_Write_Data(Camera_Data);
			READ_FIFO_PIXEL(Camera_Data3);		/* 从FIFO读出一个rgb565像素到Camera_Data变量 */
				ILI9341_Write_Data(Camera_Data);
			pBuf[i][j] = (Camera_Data + Camera_Data1 + Camera_Data2 + Camera_Data3) / 4;
		}
		for(j = 0; j < 320; j++)
		{
			READ_FIFO_PIXEL(Camera_Data);		/* 从FIFO读出一个rgb565像素到Camera_Data变量 */
			ILI9341_Write_Data(Camera_Data);
		}
	}
	
//	for(i = 0; i < 240; i++)
//	{
//		for(j = 0; j < 320; j++)
//		{
//			READ_FIFO_PIXEL(Camera_Data);		/* 从FIFO读出一个rgb565像素到Camera_Data变量 */
//		}
//	}
	//ImgDeal();
}
