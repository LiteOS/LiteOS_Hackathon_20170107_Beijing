/*------------------------------------------------------------------*/
/* 将得到的json包进行解析，解析内容包括：控制以及上下线设置             */
/*------------------------------------------------------------------*/
#include "json_parser.h"
#include "jsmn.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "stddef.h"
#include "comm_struct.h"
#include "sbus.h"
#include "types.h"
#include "usart3.h"
#include "io_ctr.h"
uint8_t rcv_jsonpack_flag;   //接收到一帧json数据包
extern uint8 rx_buf_len;
char *json_pack;
enum type_t{sensor, cmd, log}; //枚举类型
static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
			strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}
uint8_t json_parse(void)
{
        int i;
        int r;
        static uint8_t id;
	    uint8_t pw;
	    uint8_t sta;
	    enum type_t type;
	    char* message_type;
        jsmn_parser p;
        jsmntok_t t[16]; /* We expect no more than 128 tokens */
        char *str[2];  /*这里还没搞明白，为什么是2*/
	    
        jsmn_init(&p);
	    //usart3_printf("1\r\n");
	    if(rcv_jsonpack_flag == 1){
					rcv_jsonpack_flag = 0;
					/*每次解析的数据长度应该为接收的长度，而不是缓冲区的总长度*/
					r = jsmn_parse(&p, (const char *)USART3_RX_BUF, rx_buf_len, t, sizeof(t)/sizeof(t[0]));
					//把长度清零，重新开始接收一帧数据
					rx_buf_len = 0;
					//加入，接收到一帧数据后，先进行处理，是否需要关闭串口接收中断
					/* Assume the top-level element is an object */
					if (r < 1 || t[0].type != JSMN_OBJECT) {
							return 1;
					}
					/* Loop over all keys of the root object */
					for (i = 1; i < r; i++) {
							/*控制类*/
							 /*ID判断*/
							 if (jsoneq((const char *)USART3_RX_BUF, &t[i], "id") == 0) {
									/* We may want to do strtol() here to get numeric value */
									id = strtol((const char *)USART3_RX_BUF + t[i+1].start, str, 10);
									
									i++;
							}
							 /*信息类型*/             
							 else if (jsoneq((const char *)USART3_RX_BUF, &t[i], "message_type") == 0) {
								     //解析字符串
									 message_type = (char*)malloc( t[i+1].end-t[i+1].start + 1);
									 message_type = strncpy(message_type,(const char *)USART3_RX_BUF + t[i+1].start,  t[i+1].end-t[i+1].start + 1);
								     message_type[t[i+1].end-t[i+1].start] = '\0';
									 usart3_printf("mess = %s", message_type);
								     if(strcmp((const char*)message_type, "sensor") == 0)
										 type = sensor;
									 if(strcmp((const char*)message_type, "cmd") == 0)
										 type = cmd;
									 if(strcmp((const char*)message_type, "log") == 0)
										 type = log;
									 free(message_type);
									 usart3_printf("type = %d\r\n", type);
									 i++;
							}
							//如果接收的是控制指令，那么需要再解析两个tokens
							if(type == cmd){
								/*电源控制*/
								 if (jsoneq((const char *)USART3_RX_BUF, &t[i], "pw") == 0) {
										/* We may want to do strtol() here to get numeric value */
										 pw = strtol((const char *)USART3_RX_BUF + t[i+1].start,str, 10);
										 usart3_printf("pw = %d\r\n", pw);
										 i++;
								}
								 /*状态标识*/
								 else if (jsoneq((const char *)USART3_RX_BUF, &t[i], "sta") == 0) {
										/* We may want to do strtol() here to get numeric value */
										 sta = strtol((const char *)USART3_RX_BUF + t[i+1].start,str, 10);
										 usart3_printf("sta = %d\r\n", sta);
										 i++;
								}
								
								
							}
							//如果是日志或者传感器，只需解析sta
							else if(type == sensor || type == log){
								/*状态标识*/
								 if (jsoneq((const char *)USART3_RX_BUF, &t[i], "sta") == 0) {
										/* We may want to do strtol() here to get numeric value */
										 sta = strtol((const char *)USART3_RX_BUF + t[i+1].start,str, 10);
									     usart3_printf("sta = %d\r\n", sta);
										 i++;
								}
							}
							 
					}
				 
				//进水
				if(id == 1 )
			  {
					//打开进水阀
					 if(pw == 1){
						 //usart3_printf("pw1 = %d", pw);
						
						 //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
						 //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
						 //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
						 //GPIO_Init(GPIOC, &GPIO_InitStructure);
						 GPIO_SetBits(GPIOC,  GPIO_Pin_5);
						   //ivalve_state = 1;
						}
						else{
							//usart3_printf("pw0 = %d", pw);
							
							GPIO_ResetBits(GPIOC,  GPIO_Pin_5);
						} 
						
							
				//返回指令，状态		
			 }
		 }
        //memset(sensor_limit_times, 6, 0);   /*每项计数清空*/
        return 0;
}
