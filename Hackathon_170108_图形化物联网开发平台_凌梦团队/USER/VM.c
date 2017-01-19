
/*
remo虚拟机
接口:

VM_Reset()
VM_AddInterruptEvent()
VM_ClearSysValue()
VM_Run()

*/

#include "Mem.h"
#include "proto.h"
#include "ERROR_TYPE.h"
#include "typedef.h"
#include "Code.h"
#include "CODE_LIST.h"


void DealInterface( uint32 v );



//===============================================
//虚拟机配置

//常量 - 占据4个字节
const uint8 ConstByteNumber = 4;
//函数地址 - 占据3个字节
const uint8 GotoByteNumber = 3;
//接口参数 - 占据4个字节
const uint8 InterfaceByteNumber = 4;
//变量地址 - 占据2个字节
const uint8 VarAddrByteNumber = 2;
//读写分量 - 占据1个字节
const uint8 SubValueByteNumber = 1;

//===============================================
//虚拟寄存器, 这是一个基于寄存器的虚拟机(而不是基于栈)

uint16 I_DP, I_TDP, I_TTDP;
uint32 I_A, I_B;

uint32 PC;
uint16 DP;
uint16 TDP; 
uint16 TTDP;
uint16 VSP;

int8 A_s8;
uint8 A_u8;
int16 A_s16;
uint16 A_u16;
int32 A_s32;
uint32 A_u32;
int8 B_s8;
uint8 B_u8;
int16 B_s16;
uint16 B_u16;
int32 B_s32;
uint32 B_u32;

int32 A;
int32 B;
uint32 AU;
uint32 BU;

//===============================================
//运行中断控制
volatile bool EA;
volatile bool isInterrupt;
const uint8 InterruptLength = 20;
uint8 InterruptList[InterruptLength];

volatile uint8 AVMParamNumber;
volatile uint8 InterruptAddIndex;
volatile uint8 InterruptReadIndex;
volatile uint8 VarInterruptIndex;

//----------------------------------------------------------------------------------
//虚拟机初始化
void VM_Reset()
{
	PC = 0;
	DP = 0;
	TDP = 0;
	TTDP = 0;
	
    I_DP = 0;
    I_TDP = 0;
    I_TTDP = 0;
    I_A = 0;
    I_B = 0;
    
    VSP = BASE_LENGTH - 1;
	A_s8 = 0;
	A_u8 = 0;
	A_s16 = 0;
	A_u16 = 0;
	A_s32 = 0;
	A_u32 = 0;
	B_s8 = 0;
	B_u8 = 0;
	B_s16 = 0;
	B_u16 = 0;
	B_s32 = 0;
	B_u32 = 0;
	A = 0;
	B = 0;
	AU = 0;
	BU = 0;
	
	EA = false;
	isInterrupt = false;
	
	Mem_Clear();
	
	AVMParamNumber = 0;
	InterruptAddIndex = 0;
	InterruptReadIndex = 0;
	VarInterruptIndex = 0;
}
//----------------------------------------------------------------------------------
//向虚拟机添加一个系统中断
void VM_AddInterruptEvent( uint8 Addr )
{
	InterruptList[ InterruptAddIndex ] = Addr;
	++InterruptAddIndex;
	if( InterruptAddIndex == InterruptLength ) {
		InterruptAddIndex = 0;
	}
}
//----------------------------------------------------------------------------------
void VM_ClearSysValue()
{
	AVMParamNumber = 0;
}
//----------------------------------------------------------------------------------
void ReadSys_int8()
{
	if( ( A & 0x80 ) != 0 ) {
		A = -( ( A ^ 0xff ) + 1 );
	}
}
void ReadSys_int16()
{
	if( ( A & 0x8000 ) != 0 ) {
		A = -( ( A ^ 0xffff ) + 1 );
	}
}
void ReadSys_int32()
{
	//...
}
//----------------------------------------------------------------------------------
uint32 ReadConstValue()
{
	uint32 d = 0;
	uint32 B = 1;
	for( uint8 i = 0; i < ConstByteNumber; ++i ) {
		d += B * CODE_ReadIns( PC );
		B *= 0x100;
		PC++;
	}
	return d;
}
//----------------------------------------------------------------------------------
uint32 ReadInterfaceValue()
{
	uint32 d = 0;
	uint32 B = 1;
	for( uint8 i = 0; i < InterfaceByteNumber; ++i ) {
		d += B * CODE_ReadIns( PC );
		B *= 0x100;
		PC++;
	}
	return d;
}
//----------------------------------------------------------------------------------
uint32 ReadInsAddrValue()
{
	uint32 d = 0;
	uint32 B = 1;
	for( uint8 i = 0; i < GotoByteNumber; ++i ) {
		d += B * CODE_ReadIns( PC );
		B *= 0x100;
		PC++;
	}
	return d;
}
//----------------------------------------------------------------------------------
uint32 ReadVarAddrValue()
{
	uint32 d = 0;
	uint32 B = 1;
	for( uint8 i = 0; i < VarAddrByteNumber; ++i ) {
		d += B * CODE_ReadIns( PC );
		B *= 0x100;
		PC++;
	}
	return d;
}
//----------------------------------------------------------------------------------
uint32 ReadSubValueValue()
{
	uint32 d = 0;
	uint32 B = 1;
	for( uint8 i = 0; i < SubValueByteNumber; ++i ) {
		d += B * CODE_ReadIns( PC );
		B *= 0x100;
		PC++;
	}
	return d;
}
//----------------------------------------------------------------------------------
//执行一条虚拟机指令
void VM_Run()
{
	uint32 temp;
	uint32 BB;
	uint32 NBB;
	uint32 index;
	uint32 CA;
	
	PC++;
	uint8 Code = CODE_ReadIns( PC - 1 );
	uint32 v;
	switch( Code ) {
		//接口调用
		case N_Interface:		DP = TTDP; v = ReadInterfaceValue(); DealInterface( v ); break;
		case N_SetStack:		DP = ReadVarAddrValue(); break;
		case N_Goto:			PC = ReadInsAddrValue(); break;
		case N_StackInc:		TTDP = DP + ReadVarAddrValue(); TDP = TTDP; break;
		case N_StackDec:		DP -= ReadVarAddrValue(); break;
		//函数调用
		case N_Call:
			DP = TTDP;
			temp = PC + GotoByteNumber;
			for( uint8 i = 0; i < GotoByteNumber; ++i ) {
				BASE[ VSP ] = (uint8)(temp%256);
				--VSP;
				temp /= 256;
			}
			PC = ReadInsAddrValue();
			break;
		//返回指令
		case N_Reti:
			EA = true;
			isInterrupt = false;
			//不需要break, 继续往下运行
		case N_Ret:
			temp = 0;
			for( uint8 i = 0; i < GotoByteNumber; ++i ) {
				++VSP;
				temp <<= 8;
				temp += BASE[ VSP ];
			}
			PC = temp;
			break;
		case N_Pop:				++VSP; DP = 256*BASE[VSP]; ++VSP; DP += BASE[VSP]; break;
		case N_Push:			BASE[VSP] = DP%256; --VSP; BASE[VSP] = DP/256; --VSP; break;
		case N_CLI:				EA = false; break;
		case N_SEI:				EA = true; break;
		case N_SETA_uint8:					A = (uint8)ReadConstValue(); break;
		case N_SETA_uint16:					A = (uint16)ReadConstValue(); break;
		case N_SETA_uint32:					A = (uint32)ReadConstValue(); break;
		case N_SETA_sint8:					A = (int8)ReadConstValue(); break;
		case N_SETA_sint16:					A = (int16)ReadConstValue(); break;
		case N_SETA_sint32:					A = (int32)ReadConstValue(); break;
		//----------------------------------------------------------
		case N_SaveCurrent:
			I_DP = DP;
			I_TDP = TDP;
			I_TTDP = TTDP;
			I_A = A;
			I_B = B;
			//设置中断函数的局部变量堆栈
			DP = BASE_LENGTH - 1000;
			break;
		case N_LoadCurrent:
			DP = I_DP;
			TDP = I_TDP;
			TTDP = I_TTDP;
			A = I_A;
			B = I_B;
			break;
		//case N_TransXValue:
		//	uint8 Number = ReadVarAddrValue();
		//	for( uint8 i = 0; i < Number; ++i ) {
		//		BASE[DP + i] = AVMParam[ VarInterruptIndex ][ i ];
		//	}
		//	break;
		//----------------------------------------------------------
		case N_Load_SP_local_uint16:		VSP = Mem_Get_uint16( DP + ReadVarAddrValue() ); break;
		case N_Save_SP_local_uint16:		Mem_Set_uint16( DP + ReadVarAddrValue(), VSP ); break;
		//----------------------------------------------------------
		case N_InsAddr:							A = ReadInsAddrValue(); break;
		case N_Load_PC_local_uint:
			if( GotoByteNumber == 2 ) PC = Mem_Get_uint16( DP + ReadVarAddrValue() );
			if( GotoByteNumber == 3 ) PC = Mem_Get_uint24( DP + ReadVarAddrValue() );
			if( GotoByteNumber == 4 ) PC = Mem_Get_uint32( DP + ReadVarAddrValue() );
			break;
		case N_ReadAddr_local_uint32:			A = DP + ReadConstValue(); break;
		case N_AddrOffset_uintx_uintx_uintx:	A += B * ReadVarAddrValue(); break;
		case N_StructOffset_uintx_uintx:		A += ReadVarAddrValue(); break;
		//----------------------------------------------------------
		case N_LoadCODE_uint8:		A = Mem_GetCode_uint8( B ); break;
		case N_LoadCODE_uint16:		A = Mem_GetCode_uint16( B ); break;
		case N_LoadCODE_uint32:		A = Mem_GetCode_uint32( B ); break;
		case N_LoadBASE_uint8:		A = Mem_Get_uint8( B ); break;
		case N_LoadBASE_uint16:		A = Mem_Get_uint16( B ); break;
		case N_LoadBASE_uint32:		A = Mem_Get_uint32( B ); break;
		case N_SaveBASE_uint8:		Mem_Set_uint8( B, A ); break;
		case N_SaveBASE_uint16:		Mem_Set_uint16( B, A ); break;
		case N_SaveBASE_uint32:		Mem_Set_uint32( B, A ); break;
		//----------------------------------------------------------
		case N_ReadSys_sint8:				ReadSys_int8(); break;
		case N_ReadSys_sint16:				ReadSys_int16(); break;
		case N_ReadSys_sint32:				ReadSys_int32(); break;
		//----------------------------------------------------------
		case N_Save_0_local_bool:		Mem_Set_uint8( DP + ReadVarAddrValue(), A ); break;
		case N_Save_0_local_bit:		Mem_Set_uint8( DP + ReadVarAddrValue(), A ); break;
		case N_Save_0_local_uint8:		Mem_Set_uint8( DP + ReadVarAddrValue(), A ); break;
		case N_Save_0_local_uint16:		Mem_Set_uint16( DP + ReadVarAddrValue(), A ); break;
		case N_Save_0_local_uint32:		Mem_Set_uint32( DP + ReadVarAddrValue(), A ); break;
		case N_Save_0_local_int8:		Mem_Set_int8( DP + ReadVarAddrValue(), A ); break;
		case N_Save_0_local_int16:		Mem_Set_int16( DP + ReadVarAddrValue(), A ); break;
		case N_Save_0_local_int32:		Mem_Set_int32( DP + ReadVarAddrValue(), A ); break;
		
		case N_Load_0_local_bool:		A = Mem_Get_uint8( DP + ReadVarAddrValue() ); break;
		case N_Load_0_local_bit:		A = Mem_Get_uint8( DP + ReadVarAddrValue() ); break;
		case N_Load_0_local_uint8:		A = Mem_Get_uint8( DP + ReadVarAddrValue() ); break;
		case N_Load_0_local_uint16:		A = Mem_Get_uint16( DP + ReadVarAddrValue() ); break;
		case N_Load_0_local_uint32:		A = Mem_Get_uint32( DP + ReadVarAddrValue() ); break;
		case N_Load_0_local_int8:		A = Mem_Get_int8( DP + ReadVarAddrValue() ); break;
		case N_Load_0_local_int16:		A = Mem_Get_int16( DP + ReadVarAddrValue() ); break;
		case N_Load_0_local_int32:		A = Mem_Get_int32( DP + ReadVarAddrValue() ); break;
		
		case N_Load_1_local_bool:		B = Mem_Get_uint8( DP + ReadVarAddrValue() ); break;
		case N_Load_1_local_bit:		B = Mem_Get_uint8( DP + ReadVarAddrValue() ); break;
		case N_Load_1_local_uint8:		B = Mem_Get_uint8( DP + ReadVarAddrValue() ); break;
		case N_Load_1_local_uint16:		B = Mem_Get_uint16( DP + ReadVarAddrValue() ); break;
		case N_Load_1_local_uint32:		B = Mem_Get_uint32( DP + ReadVarAddrValue() ); break;
		case N_Load_1_local_int8:		B = Mem_Get_int8( DP + ReadVarAddrValue() ); break;
		case N_Load_1_local_int16:		B = Mem_Get_int16( DP + ReadVarAddrValue() ); break;
		case N_Load_1_local_int32:		B = Mem_Get_int32( DP + ReadVarAddrValue() ); break;
		//----------------------------------------------------------
		case N_Save_0_static_bool:		Mem_Set_uint8( ReadVarAddrValue(), A ); break;
		case N_Save_0_static_bit:		Mem_Set_uint8( ReadVarAddrValue(), A ); break;
		case N_Save_0_static_uint8:		Mem_Set_uint8( ReadVarAddrValue(), A ); break;
		case N_Save_0_static_uint16:	Mem_Set_uint16( ReadVarAddrValue(), A ); break;
		case N_Save_0_static_uint32:	Mem_Set_uint32( ReadVarAddrValue(), A ); break;
		case N_Save_0_static_int8:		Mem_Set_int8( ReadVarAddrValue(), A ); break;
		case N_Save_0_static_int16:		Mem_Set_int16( ReadVarAddrValue(), A ); break;
		case N_Save_0_static_int32:		Mem_Set_int32( ReadVarAddrValue(), A ); break;
		
		case N_Load_0_static_bool:		A = Mem_Get_uint8( ReadVarAddrValue() ); break;
		case N_Load_0_static_bit:		A = Mem_Get_uint8( ReadVarAddrValue() ); break;
		case N_Load_0_static_uint8:		A = Mem_Get_uint8( ReadVarAddrValue() ); break;
		case N_Load_0_static_uint16:	A = Mem_Get_uint16( ReadVarAddrValue() ); break;
		case N_Load_0_static_uint32:	A = Mem_Get_uint32( ReadVarAddrValue() ); break;
		case N_Load_0_static_int8:		A = Mem_Get_int8( ReadVarAddrValue() ); break;
		case N_Load_0_static_int16:		A = Mem_Get_int16( ReadVarAddrValue() ); break;
		case N_Load_0_static_int32:		A = Mem_Get_int32( ReadVarAddrValue() ); break;
		
		case N_Load_1_static_bool:		B = Mem_Get_uint8( ReadVarAddrValue() ); break;
		case N_Load_1_static_bit:		B = Mem_Get_uint8( ReadVarAddrValue() ); break;
		case N_Load_1_static_uint8:		B = Mem_Get_uint8( ReadVarAddrValue() ); break;
		case N_Load_1_static_uint16:	B = Mem_Get_uint16( ReadVarAddrValue() ); break;
		case N_Load_1_static_uint32:	B = Mem_Get_uint32( ReadVarAddrValue() ); break;
		case N_Load_1_static_int8:		B = Mem_Get_int8( ReadVarAddrValue() ); break;
		case N_Load_1_static_int16:		B = Mem_Get_int16( ReadVarAddrValue() ); break;
		case N_Load_1_static_int32:		B = Mem_Get_int32( ReadVarAddrValue() ); break;
		//----------------------------------------------------------
		case N_AutoSwitch_int16_int8:	A &= 0xff; ReadSys_int8(); break;
		case N_AutoSwitch_int32_int8:	A &= 0xff; ReadSys_int8(); break;
		case N_AutoSwitch_int32_int16:	A &= 0xffff; ReadSys_int16(); break;
		//----------------------------------------------------------
		case N_UserSwitch_sint8_uint8:		A &= 0xFF; ReadSys_int8(); break;
		case N_UserSwitch_sint16_uint16:	A &= 0xFFFF; ReadSys_int8(); break;
		case N_UserSwitch_sint32_uint32:	break;
		case N_UserSwitch_uint8_sint8:		A &= 0xFF; break;
		case N_UserSwitch_uint16_sint16:	A &= 0xFFFF; break;
		case N_UserSwitch_uint32_sint32:	break;
		
		case N_UserSwitch_uint8_uint16:		A &= 0xFF; break;
		case N_UserSwitch_uint8_uint32:		A &= 0xFF; break;
		case N_UserSwitch_uint16_uint32:	A &= 0xFFFF; break;
		
		case N_UserSwitch_sint8_sint16:		A &= 0xFF; ReadSys_int8(); break;
		case N_UserSwitch_sint8_sint32:		A &= 0xFF; ReadSys_int8(); break;
		case N_UserSwitch_sint16_sint32:	A &= 0xFFFF; ReadSys_int16(); break;
		//----------------------------------------------------------
		case N_Not_bool_bool:				if( A == 0 ) A = 1; else A = 0; break;
		case N_And_bool_bool_bool:			if( A != 0 && B != 0 ) A = 1; else A = 0; break;
		case N_Or_bool_bool_bool:			if( A != 0 || B != 0 ) A = 1; else A = 0; break;
		//----------------------------------------------------------
		case N_Neg_int8_int8:				A = -A; A &= 0xff; ReadSys_int8(); break;
		case N_Neg_int16_int16:				A = -A; A &= 0xffff; ReadSys_int8(); break;
		case N_Neg_int32_int32:				A = -A; break;
		case N_Abs_x_x:						if( A < 0 ) A = -A; break;
		//----------------------------------------------------------
		case N_Add_uint8_uint8_uint8:		A += B; A &= 0xff; break;
		case N_Add_uint16_uint16_uint16:	A += B; A &= 0xffff; break;
		case N_Add_uint32_uint32_uint32:	A += B; break;
		case N_Add_int8_int8_int8:			A += B; A &= 0xff; break;
		case N_Add_int16_int16_int16:		A += B; A &= 0xffff; break;
		case N_Add_int32_int32_int32:		A += B; break;
		//----------------------------------------------------------
		case N_Sub_uint8_uint8_uint8:		A -= B; A &= 0xff; break;
		case N_Sub_uint16_uint16_uint16:	A -= B; A &= 0xffff; break;
		case N_Sub_uint32_uint32_uint32:	A -= B; break;
		case N_Sub_int8_int8_int8:			A -= B; A &= 0xff; break;
		case N_Sub_int16_int16_int16:		A -= B; A &= 0xffff; break;
		case N_Sub_int32_int32_int32:		A -= B; break;
		//----------------------------------------------------------
		case N_Mult_uint8_uint8_uint8:		AU = (uint8)A; BU = (uint8)B; AU *= BU; A = (int8)AU; A &= 0xff; break;
		case N_Mult_uint16_uint16_uint16:	AU = (uint16)A; BU = (uint16)B; AU *= BU; A = (int16)AU; A &= 0xffff; break;
		case N_Mult_uint32_uint32_uint32:	AU = (uint32)A; BU = (uint32)B; AU *= BU; A = (int32)AU; break;
		case N_Mult_int8_int8_int8:			A *= B; A &= 0xff; break;
		case N_Mult_int16_int16_int16:		A *= B; A &= 0xffff; break;
		case N_Mult_int32_int32_int32:		A *= B; break;
		//----------------------------------------------------------
		case N_Div_uint8_uint8_uint8:		AU = (uint8)A; BU = (uint8)B; AU /= BU; A = (int8)AU; A &= 0xff; break;
		case N_Div_uint16_uint16_uint16:	AU = (uint16)A; BU = (uint16)B; AU /= BU; A = (int16)AU; A &= 0xffff; break;
		case N_Div_uint32_uint32_uint32:	AU = (uint32)A; BU = (uint32)B; AU /= BU; A = (int32)AU; break;
		case N_Div_int8_int8_int8:			A /= B; A &= 0xff; break;
		case N_Div_int16_int16_int16:		A /= B; A &= 0xffff; break;
		case N_Div_int32_int32_int32:		A /= B; break;
		//----------------------------------------------------------
		case N_Mod_uint8_uint8_uint8:		AU = (uint8)A; BU = (uint8)B; AU %= BU; A &= 0xff; A = (int8)AU; break;
		case N_Mod_uint16_uint16_uint16:	AU = (uint16)A; BU = (uint16)B; AU %= BU; A &= 0xffff; A = (int16)AU; break;
		case N_Mod_uint32_uint32_uint32:	AU = (uint32)A; BU = (uint32)B; AU %= BU; A = (int32)AU; break;
		case N_Mod_int8_int8_int8:			A %= B; A &= 0xff; break;
		case N_Mod_int16_int16_int16:		A %= B; A &= 0xffff; break;
		case N_Mod_int32_int32_int32:		A %= B; break;
		//----------------------------------------------------------
		case N_Large_bool_uint_uint:		AU = A; BU = B; if( AU > BU ) AU = 1; else AU = 0;		A = AU; break;
		case N_Large_bool_sint_sint:		if( A > B ) A = 1; else A = 0; break;
		case N_LargeEqual_bool_uint_uint:	AU = A; BU = B; if( AU >= BU ) AU = 1; else AU = 0;		A = AU; break;
		case N_LargeEqual_bool_sint_sint:	if( A >= B ) A = 1; else A = 0; break;
		case N_Small_bool_uint_uint:		AU = A; BU = B; if( AU < BU ) AU = 1; else AU = 0;		A = AU; break;
		case N_Small_bool_sint_sint:		if( A < B ) A = 1; else A = 0; break;
		case N_SmallEqual_bool_uint_uint:	AU = A; BU = B; if( AU <= BU ) AU = 1; else AU = 0;		A = AU; break;
		case N_SmallEqual_bool_sint_sint:	if( A <= B ) A = 1; else A = 0; break;
		case N_Equal_bool_uint_uint:
		case N_Equal_bool_sint_sint:		if( A == B ) A = 1; else A = 0; break;
		case N_NotEqual_bool_uint_uint:
		case N_NotEqual_bool_sint_sint:		if( A != B ) A = 1; else A = 0; break;
		//----------------------------------------------------------
		case N_TrueGoto_bool:
			if( A == 1 ) PC = ReadInsAddrValue(); else PC += GotoByteNumber; break;
		case N_FalseGoto_bool:
			if( A == 0 ) PC = ReadInsAddrValue(); else PC += GotoByteNumber; break;
		//----------------------------------------------------------
		case N_Com_bit_bit:					AU = A; AU ^= 0x01; A = AU; break;
		case N_Com_uint8_uint8:				AU = A; AU ^= 0xFF; A = AU; break;
		case N_Com_uint16_uint16:			AU = A; AU ^= 0xFFFF; AU /= BU; A = AU; break;
		case N_Com_uint32_uint32:			AU = A; AU ^= 0xFFFFFFFF; AU /= BU; A = AU; break;
		case N_And_x_x_x:					AU = A; BU = B; AU &= BU; A = AU; break;
		case N_Or_x_x_x:					AU = A; BU = B; AU |= BU; A = AU; break;
		case N_Xor_x_x_x:					AU = A; BU = B; AU ^= BU; A = AU; break;
		case N_Right_x_x_x:					AU = A; BU = B; AU >>= BU; A = AU; break;
		case N_Left_uint8_uint8_uint8:		AU = A; BU = B; AU <<= BU; AU &= 0xFF; A = AU; break;
		case N_Left_uint16_uint16_uint8:	AU = A; BU = B; AU <<= BU; AU &= 0xFFFF; A = AU; break;
		case N_Left_uint32_uint32_uint8:	AU = A; BU = B; AU <<= BU; A = AU; break;
		//----------------------------------------------------------
		case N_ReadSub_bit_uint8:			A >>= ReadSubValueValue(); A &= 0x01; break;
		case N_ReadSub_bit_uint16:			A >>= ReadSubValueValue(); A &= 0x01; break;
		case N_ReadSub_bit_uint32:			A >>= ReadSubValueValue(); A &= 0x01; break;
		case N_ReadSub_uint8_uint32:		A >>= ReadSubValueValue(); A &= 0xFF; break;
		case N_ReadSub_uint8_uint16:		A >>= ReadSubValueValue(); A &= 0xFF; break;
		case N_ReadSub_uint16_uint32:		A >>= ReadSubValueValue(); A &= 0xFFFF; break;
		case N_WriteSub_uint8_uint8_bit:
			BB = ((uint32)1 << ReadSubValueValue()) & 0xFF;
			NBB = BB ^ 0xFF;
			AU = A;
			if( B == 0 ) AU &= NBB; else AU |= BB;
			A = AU;
			break;
		case N_WriteSub_uint16_uint16_bit:
			BB = ((uint32)1 << ReadSubValueValue()) & 0xFFFF;
			NBB = BB ^ 0xFFFF;
			AU = A;
			if( B == 0 ) AU &= NBB; else AU |= BB;
			A = AU;
			break;
		case N_WriteSub_uint32_uint32_bit:
			BB = ((uint32)1 << ReadSubValueValue());
			NBB = BB ^ 0xFFFFFFFF;
			AU = A;
			if( B == 0 ) AU &= NBB; else AU |= BB;
			A = AU;
			break;
		case N_WriteSub_uint16_uint16_uint8:
			index = ReadSubValueValue();
			BU = B;
			BB = (BU << index);
			CA = ((uint32)0xFF << index) ^ 0xFFFF;
			AU = A;
			AU &= CA;
			AU |= BB;
			A = AU;
			break;
		case N_WriteSub_uint32_uint32_uint16:
			index = ReadSubValueValue();
			BU = B;
			BB = (BU << index);
			CA = ((uint32)0xFFFF << index) ^ 0xFFFFFFFF;
			AU = A;
			AU &= CA;
			AU |= BB;
			A = AU;
			break;
		case N_WriteSub_uint32_uint32_uint8:
			index = ReadSubValueValue();
			BU = B;
			BB = (BU << index);
			CA = ((uint32)0xFF << index) ^ 0xFFFFFFFF;
			AU = A;
			AU &= CA;
			AU |= BB;
			A = AU;
			break;
		//----------------------------------------------------------
		case N_TransVar_bit:			Mem_Set_uint8( TDP, A ); TDP += 1; break;
		case N_TransVar_bool:			Mem_Set_uint8( TDP, A ); TDP += 1; break;
		case N_TransVar_uint8:			Mem_Set_uint8( TDP, A ); TDP += 1; break;
		case N_TransVar_uint16:			Mem_Set_uint16( TDP, A ); TDP += 2; break;
		case N_TransVar_uint32:			Mem_Set_uint32( TDP, A ); TDP += 4; break;
		case N_TransVar_int8:			Mem_Set_int8( TDP, A ); TDP += 1; break;
		case N_TransVar_int16:			Mem_Set_int16( TDP, A ); TDP += 2; break;
		case N_TransVar_int32:			Mem_Set_int32( TDP, A ); TDP += 4; break;
		
		//跳转递减
		case N_GotoDec_x:
			if( A == 0 ) {
				PC = ReadInsAddrValue();
			}
			else {
				--A;
				PC += GotoByteNumber;
			}
			break;
		//出错处理
		default:
			Running = false;
			//Serial.print( "ERR:" );
			//Serial.write( PC/256 );
			//Serial.write( PC%256 );
			//Serial.print( " " );
			Error( E_VM_InsError, Code );
			break;
	}
	//处理虚拟机中断
	if( EA && !isInterrupt && InterruptReadIndex != InterruptAddIndex ) {
		isInterrupt = true;
		uint32 Addr = PC;
		for( uint8 i = 0; i < GotoByteNumber; ++i ) {
			BASE[ VSP ] = (uint8)(Addr%256);
			--VSP;
			Addr /= 256;
		}
		uint8 TPC = InterruptList[ InterruptReadIndex ];
		PC = TPC * (1 + GotoByteNumber);
		VarInterruptIndex = InterruptReadIndex;
		++InterruptReadIndex;
		if( InterruptReadIndex == InterruptLength ) {
			InterruptReadIndex = 0;
		}
	}
}

//#ifdef VM_INTERFACE

bool WriteNotRead;
uint32 ReadData;


void REMO_DataChannelWrite( uint32 d, uint32 d1 )
{
	WriteNotRead = true;
	
	Deal( 0xAA );
	
	Deal( (uint8)(d % 256 ) );
	Deal( (uint8)(d / 256 ) );
	
	Deal( (uint8)(d1 % 256) ); d1 /= 256;
	Deal( (uint8)(d1 % 256) ); d1 /= 256;
	Deal( (uint8)(d1 % 256) ); d1 /= 256;
	Deal( (uint8)(d1 % 256) );
}

void REMO_DataChannelRead( uint32 d )
{
	WriteNotRead = false;
	
	Deal(0xAA );
	
	Deal( (uint8)(d % 256 ) );
	Deal( (uint8)(d / 256 ) );
	
	Deal( 0 );
	Deal( 0 );
	Deal( 0 );
	Deal( 0 );
	A = ReadData;
}

void DealInterface( uint32 v )
{
	uint32 V0 = 0;
	uint32 V1 = 0;
	switch( v ) {
		
		//系统接口
		case 1:
			//... 开启对应的中断
			break;
		case 3:
			//cout << "DEBUG: " << Mem_Get_uint32( DP + 0 ) << endl;
			break;
		case 4:
			V0 = Mem_Get_uint32( DP + 0 );
			V1 = Mem_Get_uint32( DP + 4 );
			REMO_DataChannelWrite( V0, V1 );
			break;
		case 5:
			V0 = Mem_Get_uint32( DP + 0 );
			REMO_DataChannelRead( V0 );
			break;
		
		default:
			Error( E_VM_InterfaceError, (uint8)v );
			break;
	}
}


//#endif






