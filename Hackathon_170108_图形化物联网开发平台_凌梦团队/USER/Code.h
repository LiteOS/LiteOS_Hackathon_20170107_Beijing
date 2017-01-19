
#include "typedef.h"

const uint8 N_CLI =		220;
const uint8 N_SEI =		221;

const uint8 N_Interface = 1;	//接口调用
const uint8 N_SetStack = 2;		//设置堆栈
const uint8 N_Goto = 3;			//跳转指令
const uint8 N_Call = 4;			//函数调用
const uint8 N_Ret = 5;			//返回指令
const uint8 N_Reti =	6;			//中断返回指令
const uint8 N_StackInc = 7;		//跳转指令
const uint8 N_StackDec = 8;		//跳转指令

//const uint8 N_TransXValue = 9;
const uint8 N_SaveCurrent = 10;
const uint8 N_LoadCurrent = 11;
//const uint8 N_Nop = 12;

const uint8 N_Pop =	13;
const uint8 N_Push =	14;

const uint8 N_SETA_uint8 =	15;			//设置A寄存器
const uint8 N_SETA_uint16 =	205;		//设置A寄存器
const uint8 N_SETA_uint32 =	206;		//设置A寄存器
const uint8 N_SETA_sint8 =	207;		//设置A寄存器
const uint8 N_SETA_sint16 =	208;		//设置A寄存器
const uint8 N_SETA_sint32 =	209;		//设置A寄存器

const uint8 N_Not_bool_bool =			19;
const uint8 N_And_bool_bool_bool =		20;
const uint8 N_Or_bool_bool_bool =		21;

const uint8 N_GotoDec_x =	50;	//跳转递减

const uint8 N_Load_PC_local_uint =	151;
const uint8 N_InsAddr =				152;

const uint8 N_ReadAddr_local_uint32 =				16;
const uint8 N_AddrOffset_uintx_uintx_uintx =		17;
const uint8 N_StructOffset_uintx_uintx =			18;

const uint8 N_LoadCODE_uint8 =	77;
const uint8 N_LoadCODE_uint16 =	78;
const uint8 N_LoadCODE_uint32 =	80;
const uint8 N_LoadBASE_uint8 =	81;
const uint8 N_LoadBASE_uint16 =	82;
const uint8 N_LoadBASE_uint32 =	84;
const uint8 N_SaveBASE_uint8 =	85;
const uint8 N_SaveBASE_uint16 =	86;
const uint8 N_SaveBASE_uint32 =	88;

const uint8 N_ReadSys_sint8 =	63;
const uint8 N_ReadSys_sint16 =	64;
const uint8 N_ReadSys_sint32 =	66;

const uint8 N_AutoSwitch_int16_int8 =		202;		
const uint8 N_AutoSwitch_int32_int8 =		203;		
const uint8 N_AutoSwitch_int32_int16 =		204;

const uint8 N_UserSwitch_sint8_uint8 =				198;
const uint8 N_UserSwitch_sint16_uint16 =			199;
const uint8 N_UserSwitch_sint32_uint32 =			201;

const uint8 N_UserSwitch_uint8_sint8 =				216;
const uint8 N_UserSwitch_uint16_sint16 =			217;
const uint8 N_UserSwitch_uint32_sint32 =			219;

const uint8 N_UserSwitch_uint8_uint16 =				192;
const uint8 N_UserSwitch_uint8_uint32 =				193;
const uint8 N_UserSwitch_uint16_uint32 =			194;
const uint8 N_UserSwitch_sint8_sint16 =				195;
const uint8 N_UserSwitch_sint8_sint32 =				196;
const uint8 N_UserSwitch_sint16_sint32 =			197;

const uint8 N_Neg_int8_int8 =		22;
const uint8 N_Neg_int16_int16 =		153;
const uint8 N_Neg_int32_int32 =		155;
const uint8 N_Abs_x_x =				23;

const uint8 N_Add_uint8_uint8_uint8 =		156;
const uint8 N_Add_uint16_uint16_uint16 =	157;
const uint8 N_Add_uint32_uint32_uint32 =	159;
const uint8 N_Add_int8_int8_int8 =			160;
const uint8 N_Add_int16_int16_int16 =		161;
const uint8 N_Add_int32_int32_int32 =		163;

const uint8 N_Sub_uint8_uint8_uint8 =		25;
const uint8 N_Sub_uint16_uint16_uint16 =	164;
const uint8 N_Sub_uint32_uint32_uint32 =	166;
const uint8 N_Sub_int8_int8_int8 =			167;
const uint8 N_Sub_int16_int16_int16 =		168;
const uint8 N_Sub_int32_int32_int32 =		170;

const uint8 N_Mult_uint8_uint8_uint8 =		26;
const uint8 N_Mult_uint16_uint16_uint16 =	171;
const uint8 N_Mult_uint32_uint32_uint32 =	173;
const uint8 N_Mult_int8_int8_int8 =			174;
const uint8 N_Mult_int16_int16_int16 =		175;
const uint8 N_Mult_int32_int32_int32 =		177;

const uint8 N_Div_uint8_uint8_uint8 =		27;
const uint8 N_Div_uint16_uint16_uint16 =	178;
const uint8 N_Div_uint32_uint32_uint32 =	180;
const uint8 N_Div_int8_int8_int8 =			181;
const uint8 N_Div_int16_int16_int16 =		182;
const uint8 N_Div_int32_int32_int32 =		184;

const uint8 N_Mod_uint8_uint8_uint8 =		28;
const uint8 N_Mod_uint16_uint16_uint16 =	185;
const uint8 N_Mod_uint32_uint32_uint32 =	187;
const uint8 N_Mod_int8_int8_int8 =			188;
const uint8 N_Mod_int16_int16_int16 =		189;
const uint8 N_Mod_int32_int32_int32 =		191;

const uint8 N_Large_bool_uint_uint =		29;
const uint8 N_Large_bool_sint_sint =		210;
const uint8 N_LargeEqual_bool_uint_uint =	30;
const uint8 N_LargeEqual_bool_sint_sint =	211;
const uint8 N_Small_bool_uint_uint =		31;
const uint8 N_Small_bool_sint_sint =		212;
const uint8 N_SmallEqual_bool_uint_uint =	32;
const uint8 N_SmallEqual_bool_sint_sint =	213;
const uint8 N_Equal_bool_uint_uint =		33;
const uint8 N_Equal_bool_sint_sint =		214;
const uint8 N_NotEqual_bool_uint_uint =		34;
const uint8 N_NotEqual_bool_sint_sint =		215;

const uint8 N_TrueGoto_bool =	48;
const uint8 N_FalseGoto_bool =	49;

const uint8 N_Com_bit_bit =					35;
const uint8 N_Com_uint8_uint8 =				36;
const uint8 N_Com_uint16_uint16 = 			37;
const uint8 N_Com_uint24_uint24 =			38;
const uint8 N_Com_uint32_uint32 =			39;
const uint8 N_And_x_x_x =					40;
const uint8 N_Or_x_x_x =					41;
const uint8 N_Xor_x_x_x =					42;
const uint8 N_Left_uint8_uint8_uint8 =		43;
const uint8 N_Left_uint16_uint16_uint8 =	44;
const uint8 N_Left_uint24_uint24_uint8 =	45;
const uint8 N_Left_uint32_uint32_uint8 =	46;
const uint8 N_Right_x_x_x =					47;

const uint8 N_ReadSub_bit_uint8 =				51;
const uint8 N_ReadSub_bit_uint16 =				52;
const uint8 N_ReadSub_bit_uint32 =				53;
const uint8 N_ReadSub_uint8_uint32 =			54;
const uint8 N_ReadSub_uint8_uint16 =			55;
const uint8 N_ReadSub_uint16_uint32 =			56;
const uint8 N_WriteSub_uint8_uint8_bit =		57;
const uint8 N_WriteSub_uint16_uint16_bit =		58;
const uint8 N_WriteSub_uint32_uint32_bit =		59;
const uint8 N_WriteSub_uint16_uint16_uint8 =	60;
const uint8 N_WriteSub_uint32_uint32_uint8 =	61;
const uint8 N_WriteSub_uint32_uint32_uint16 =	62;

const uint8 N_Load_0_static_bool =		89;
const uint8 N_Load_0_static_bit =		90;
const uint8 N_Load_0_static_uint8 =		91;
const uint8 N_Load_0_static_uint16 =	92;
const uint8 N_Load_0_static_uint32 =	94;
const uint8 N_Load_0_static_int8 =		95;
const uint8 N_Load_0_static_int16 =		96;
const uint8 N_Load_0_static_int32 =		98;
const uint8 N_Load_1_static_bool =		99;
const uint8 N_Load_1_static_bit =		100;
const uint8 N_Load_1_static_uint8 =		101;
const uint8 N_Load_1_static_uint16 =	102;
const uint8 N_Load_1_static_uint32 =	104;
const uint8 N_Load_1_static_int8 =		105;
const uint8 N_Load_1_static_int16 =		106;
const uint8 N_Load_1_static_int32 =		108;
const uint8 N_Save_0_static_bool =		109;
const uint8 N_Save_0_static_bit =		110;
const uint8 N_Save_0_static_uint8 =		111;
const uint8 N_Save_0_static_uint16 =	112;
const uint8 N_Save_0_static_uint32 =	114;
const uint8 N_Save_0_static_int8 =		115;
const uint8 N_Save_0_static_int16 =		116;
const uint8 N_Save_0_static_int32 =		118;

const uint8 N_Save_0_local_bool =		139;
const uint8 N_Save_0_local_bit =		140;
const uint8 N_Save_0_local_uint8 =		141;
const uint8 N_Save_0_local_uint16 =		142;
const uint8 N_Save_0_local_uint32 =		144;
const uint8 N_Save_0_local_int8 =		145;
const uint8 N_Save_0_local_int16 =		146;
const uint8 N_Save_0_local_int32 =		148;

const uint8 N_Load_SP_local_uint16 =	149;
const uint8 N_Save_SP_local_uint16 =	150;

const uint8 N_Load_0_local_bool =		119;
const uint8 N_Load_0_local_bit =		120;
const uint8 N_Load_0_local_uint8 =		121;
const uint8 N_Load_0_local_uint16 =		122;
const uint8 N_Load_0_local_uint32 =		124;
const uint8 N_Load_0_local_int8 =		125;
const uint8 N_Load_0_local_int16 =		126;
const uint8 N_Load_0_local_int32 =		128;

const uint8 N_Load_1_local_bool =		129;
const uint8 N_Load_1_local_bit =		130;
const uint8 N_Load_1_local_uint8 =		131;
const uint8 N_Load_1_local_uint16 =		132;
const uint8 N_Load_1_local_uint32 =		134;
const uint8 N_Load_1_local_int8 =		135;
const uint8 N_Load_1_local_int16 =		136;
const uint8 N_Load_1_local_int32 =		138;

const uint8 N_TransVar_bit =			67;
const uint8 N_TransVar_bool =			68;
const uint8 N_TransVar_uint8 =			69;
const uint8 N_TransVar_uint16 =			70;
const uint8 N_TransVar_uint32 =			72;
const uint8 N_TransVar_int8 =			73;
const uint8 N_TransVar_int16 =			74;
const uint8 N_TransVar_int32 =			76;


