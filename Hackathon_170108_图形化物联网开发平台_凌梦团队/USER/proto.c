

#include "typedef.h"

#include "CODE_LIST.h"
#include "ERROR_TYPE.h"
#include "usart.h"
#include "VM.h"
#include "IO.h"

#include "delay.h"

void Deal( uint8 d );
void SendData( int32 d );

void RunIns( uint8 d );
//void SendData( int32 d );

void SetAddr( uint8 d );
void Read( uint8 d );
void Store( uint8 d );
void Check( uint8 d );
void Error( uint32 ET, uint32 EC );

bool Running;

uint32 ttt;

//==========================================================================

//加载并执行目标文件程序
void LoadAndRun()
{
	CODE_Load();
	VM_Reset();
	
	Running = true;
	
	//这里需要好好研究一下, 为何每次都需要手工启动定时器
	Timer_SetTimeMs( 1 );
}
//系统启动时执行一次
void setup()
{
	//加载并执行目标文件程序
	LoadAndRun();
	
	Running = false;
}
//系统反复调用此函数
void loop()
{
	if( Running ) {
		VM_Run();
		
		//GPIO_ToggleBits(GPIOA,GPIO_Pin_5);
		//Serial_write( PC % 256 );
		//Serial_write( PC / 256 );
		//delay_ms(50);
		
		//判断定时器是否时间到
		if( Timer_isTimeOut() ) {
			Timer_SetTimeMs( 1 );
			
			//触发中断
			VM_ClearSysValue();
			VM_AddInterruptEvent( 0x11 );
			
			/*
			ttt++;
			if( ttt > 100 ) {
				ttt = 0;
				GPIO_ToggleBits(GPIOA,GPIO_Pin_5);
			}
			*/
		}
	}
	
	/*
	if( Serial_OK ) {
		Serial_OK = false;
		Deal( Serial_Data );
		GPIO_ToggleBits(GPIOA,GPIO_Pin_5);
	}
	*/
}


//----------------------------------------------------------------------------------
//协议解析器

uint32 ProIndex;
uint8 vm_status;
const uint8 VM_RUN = 0xAA;		//运行指令
const uint8 VM_STORE = 0xAB;	//存储指令
const uint8 VM_SETADDR = 0xAC;	//设置指令地址
const uint8 VM_READ = 0xAD;		//读取指令
const uint8 VM_CHECK = 0xAE;	//读取校验和

void Deal( uint8 d )
{
	//判断是否为合法数据头部
	if( ProIndex == 0 ) {
		vm_status = d;
	}
	//如果是运行指令状态, 接收一个指令并解码运行
	if( vm_status == VM_RUN ) {
		RunIns( d );
		return;
	}
	
	//关闭虚拟机, 防止共同占用数据通道造成异常
	//Running = false;
	
	//如果是设置指令地址
	switch( vm_status ) {
		case VM_SETADDR:	Running = false; SetAddr( d ); break;
		case VM_READ:		Running = false; Read( d ); break;
		case VM_STORE:		Running = false; Store( d ); break;
		case VM_CHECK:		Running = false; Check( d ); break;
		default:			Error( E_ProError, vm_status ); ProIndex = 0; break;
	}
}
//----------------------------------------------------------------------------------
//运行一个指令

//指令执行解码缓冲区
uint8 rBuffer[100];

void RunIns( uint8 d )
{
	rBuffer[ProIndex] = d;
	++ProIndex;
	
	//判断数据是否接收完毕
	if( ProIndex < 7 ) {
		return;
	}
	ProIndex = 0;
	uint8 Addr = rBuffer[1];
	uint8 Type = rBuffer[2];
	
	uint32 Data0 = rBuffer[3];
	uint32 Data1 = rBuffer[4];
	uint32 Data2 = rBuffer[5];
	uint32 Data3 = rBuffer[6];
	uint32 Data = Data0 + (Data1 << 8) + (Data2 << 16) + (Data3 << 24);
	
	//是否为写入端口
	if( WriteNotRead ) {
		
		switch( Type ) {
			case 0:		IO_DirWrite( Addr, (uint8)Data ); break;
			case 1:		IO_OutWrite( Addr, (uint8)Data ); break;
			//case 2:	IO_InWrite( Addr, (uint8)Data ); break;
			case 3:		IO_PullWrite( Addr, (uint8)Data ); break;
			case 4:		IO_AnalogOpen( Addr, (uint8)Data ); break;
			
			/*
			case 5:
				if( Addr == 0 ) {
					Serial_write( (uint8)Data );
				}
				else {
					Serial_SetBaud( (int32)Data );
				}
				break;
			*/
			
			//case 6:		Target[Addr] = Data; break;
			
			
			
			default:	Error( E_RunError, 1 ); break;
		}
	}
	else {
		switch( Type ) {
			//case 0:	SendData( IO_DirRead( Addr ) ); break;
			case 1:		SendData( IO_OutRead( Addr ) ); break;
			case 2:		SendData( IO_InRead( Addr ) ); break;
			//case 3:	SendData( IO_PullRead( Addr ) ); break;
			case 4:		SendData( IO_AnalogRead( Addr ) ); break;
			
			/*
			case 5:		SendData( Serial_Data ); Serial_Used = true; break;
			
			
			case 6:		SendData( Target[Addr] ); break;
			*/
			
			default:	Error( E_RunError, 1 ); break;
		}
	}
}
//--------------------------------------------
//发送一个int数据到上位机
void SendData( int32 d )
{
	ReadData = d;
	
	/*
	uint32 dd = d;
	Serial_write( 0xAA );
	Serial_write( dd % 256 ); dd /= 256;
	Serial_write( dd % 256 ); dd /= 256;
	Serial_write( dd % 256 ); dd /= 256;
	Serial_write( dd % 256 ); dd /= 256;
	*/
}
//==================================================================================
//指令接收器

uint8 CheckSum;

void Proto_Init()
{
	ProIndex = 0;
	vm_status = 0;
	CheckSum = 0;
}
//--------------------------------------------
//接收地址并设置
void SetAddr( uint8 d )
{
	ProIndex++;
	if( ProIndex == 1 ) {
		return;
	}
	if( ProIndex == 2 ) {
		ProIndex = 0;
		
		CheckSum = 0;
		CODE_Start();
		
		Serial_write( VM_SETADDR );
		Serial_write( d );
	}
}
//--------------------------------------------
//读取指令
uint16 AddrH;
void Read( uint8 d )
{
	ProIndex++;
	if( ProIndex == 1 ) {
		return;
	}
	if( ProIndex == 2 ) {
		AddrH = d;
		return;
	}
	if( ProIndex == 3 ) {
		ProIndex = 0;
		uint16 Addr = AddrH * 256 + d;
		for( uint32 i = 0; i < 256; ++i ) {
			Serial_write( CODE_ReadIns( Addr + i ) );
		}
	}
}
//--------------------------------------------
//接收一个指令并存储
uint32 Number;

//指令存储缓冲区
uint8 ByteCodeBuffer[256];
uint32 CIndex;

void Store( uint8 d )
{
	ProIndex++;
	if( ProIndex == 1 ) {
		return;
	}
	if( ProIndex == 2 ) {
		Number = d;
		if( Number == 0 ) {
			Number = 256;
		}
		CIndex = 0;
		return;
	}
	CheckSum ^= d;
	ByteCodeBuffer[ CIndex ] = d;
	++CIndex;
	if( ProIndex == Number + 2 ) {
		ProIndex = 0;
		
		for( uint16 i = 0; i < CIndex; ++i ) {
			CODE_AddIns( ByteCodeBuffer[ i ] );
		}
		Serial_write( VM_STORE );
	}
}
//--------------------------------------------
//发回校验和
void Check( uint8 d )
{
	ProIndex = 0;
	Serial_write( VM_CHECK );
	Serial_write( CheckSum );
	
	CODE_Save();
	
	//加载并执行目标文件程序
	LoadAndRun();
}
//--------------------------------------------
//协议出错处理程序
void Error( uint32 ET, uint32 EC )
{
	Serial_write( 0xEE );
	Serial_write( ET );
	Serial_write( EC );
}

