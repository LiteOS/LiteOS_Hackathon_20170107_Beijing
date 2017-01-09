/*------------------------------------------------------------------------/
/  W5500网络配置初始化接口函数
/-------------------------------------------------------------------------/
/
/  Copyright (C) 2015, Zg, all right reserved.
/
/ * This software is a free software and there is NO WARRANTY.
/ * No restriction on use. You can use, modify and redistribute it for
/   personal, non-profit or commercial products UNDER YOUR RESPONSIBILITY.
/ * Redistributions of source code must retain the above copyright notice.
/
/-------------------------------------------------------------------------*/
#include "mcu_init.h"
#include "stm32f10x.h"
#include "ult.h"
#include <stdio.h>
#include "usart.h"
#include "usart3.h"
#include "xassert.h"
#include "config.h"
#include "network_conf.h"
#include "socket.h"
#include "w5500.h"
#include "md5.h"
#include "spi2.h"
#include "sensor_upload.h"
#include "stdlib.h"
#include "dns.h"
//extern uint8 txsize[];
//extern uint8 rxsize[];
//extern char json_pack[64]; /*json数据包，来自json_parser*/
char *json_pack;
uint8_t recv_complete_flag = 0;
uint16_t socket_recv_len = 0;   /*socket接收数据的长度*/
CONFIG_MSG  ConfigMsg, RecvMsg;
server_config_t server_config;  /*服务端配置*/
uint8 txsize[MAX_SOCK_NUM] = {2,2,2,2,2,2,2,2};
uint8 rxsize[MAX_SOCK_NUM] = {2,2,2,2,2,2,2,2};
uint16_t anyport = 6000;
uint16_t local_port[3] = { 54581, 54582, 54583};  /*本地开放端口*/
void (*socket_task[3])(void);
void Reset_W5500(void)
{
  GPIO_ResetBits(GPIOB, WIZ_RESET);
  delay_us(2);  
  GPIO_SetBits(GPIOB, WIZ_RESET);
  delay_ms(1600);
}
/*---------------------------------------------*/
/* socket 循环池，接收、发送数据                 */
/* SOCKET0, 192.168.1.120, 2015                */
/*---------------------------------------------*/
void socket_poll(SOCKET s, uint8_t *server_ip, uint16_t server_port)
{
    //uint8_t dns_retry_cnt=0;
    //uint8_t dns_ok=0;
        switch(getSn_SR(s))       /*获取socketx的状态*/
         {
               case SOCK_INIT:    /*socket初始化完成*/
                    connect(s, server_ip ,server_port);/*在TCP模式下向服务器发送连接请求*/
                    break;
               case SOCK_ESTABLISHED:/*socket连接建立*/
                    if(getSn_IR(s) & Sn_IR_CON)
                    {
                        setSn_IR(s, Sn_IR_CON);/*Sn_IR的第0位置1*/
                    }
                    if(s == SOCKET0)   /*socket0 进行传感器值传输*/
                    {
                        socket_task[0]();
                        
                    }
                    
                    if(s == SOCKET1)
                    {
                        socket_recv_len = getSn_RX_RSR(s);/*len为已接收数据的大小*/
                        /*接收到数据，而且这次数据被处理掉，进行下次接收*/
                        if(socket_recv_len > 0 && (recv_complete_flag == 0))
                        {
                            json_pack = (char *)malloc(sizeof(char)*(socket_recv_len+1));  /* 多申请一个字节，用来存储'\0'*/
                            recv(s, (uint8_t *)json_pack, socket_recv_len); 
                            json_pack[socket_recv_len] = '\0';
                            /*将数据返回，告知收到数据，这里不合理，应该在解析完数据之后返回*/
                            send(s, (uint8_t *)json_pack, socket_recv_len);/*just for test*/
                            recv_complete_flag = 1;
                        }
                    }
                    /*W5500向Server发送测试数据*/
                    /*处理数据，可以发送数据、也可以接收数据*/
                    break;
               case SOCK_CLOSE_WAIT:/*socket等待关闭状态*/
                    disconnect(s);
                    break;
               case SOCK_CLOSED:    /*socket关闭*/
                    socket(s,Sn_MR_TCP,anyport++,Sn_MR_ND);/*打开socket0的一个端口*/
                     break;
         }
  
}
/*-------------------------------------------------------*/
/* 本地网络配置，mac、ip、子网掩码、网关                   */
/*-------------------------------------------------------*/
void network_init(net_config_t local_net_config)
{
      uint8 ip[4];
      //net_config_t local_net_config;                /*定义本地网络配置*/
      //uint8 mac[6]={0x00,0x08,0xdc,0x11,0x11,0x11}; /*定义Mac变量*/
      //uint8 lip[4]={192,168,1,110};                 /*定义lp变量*/
      //uint8 sub[4]={255,255,255,0};                 /*定义subnet变量*/
      //uint8 gw[4]={192,168,1,1};                    /*定义gateway变量*/
      uint8 dns[4]={8,8,8,8};  
      Reset_W5500();            /*硬重启W5500*/
      WIZ_SPI_Init();           /*初始化SPI接口*/ 
      setSHAR(local_net_config.mac);             /*配置Mac地址*/
      setSUBR(local_net_config.sub);             /*配置子网掩码*/
      setGAR(local_net_config.gw);               /*配置默认网关*/
      setSIPR(local_net_config.lip);             /*配置Ip地址*/
        //Init. TX & RX Memory size of w5500
      sysinit(txsize, rxsize); /*初始化8个socket*/
      setRTR(2000);             /*设置溢出时间值*/
      setRCR(3);                /*设置最大重新发送次数*/
      getSIPR (ip);
      getSUBR(ip);
      getGAR(ip);
      memcpy(ConfigMsg.dns,dns,4);  /*服务端DNS配置*/

      sprintf((char*)ConfigMsg.domain,"%s", "m.zhuogan.com.cn"); 
      
}

/*设置心跳包*/
void setkeepalive(SOCKET s)

{

    //IINCHIP_WRITE(Sn_KPALVTR(s), KEEP_ALIVE_TIME);  /*30s*/
}
