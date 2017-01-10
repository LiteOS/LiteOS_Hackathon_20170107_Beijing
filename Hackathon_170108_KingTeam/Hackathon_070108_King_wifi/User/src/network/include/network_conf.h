#ifndef  _NETWORK_CONF_H_
#define  _NETWORK_CONF_H_
#include "stm32f10x.h"
#include "types.h"


/*W5500有8个socket线程可以用*/
#define SOCKET0 0
#define SOCKET1 1
#define SOCKET2 2
#define SOCKET3 3
#define SOCKET4 4
#define SOCKET5 5
#define SOCKET6 6
#define SOCKET7 7
typedef struct _lnet_config_t{

    uint8_t mac[6];  /*本机的MAC地址*/
    uint8_t lip[4];  /*本机的IP地址*/
    uint8_t sub[4];  /*子网掩码*/
    uint8_t gw[4];   /*网关*/
    uint8_t dns[4];  /*设备DSN服务器*/
    uint8_t lport_h; /*本地端口*/
    uint8_t lport_l; /*本地端口*/
    
}net_config_t;

typedef struct _snet_config_t{
    uint8_t sip[4];   /*服务器IP*/
    uint8_t sport_h;   /*服务器端口*/
    uint8_t sport_l;
    uint8_t domain[32]; /*服务端域名*/
}server_config_t;
void Reset_W5500(void);
void socket_poll(SOCKET s, uint8_t *server_ip, uint16_t server_port);
//void socket_poll(SOCKET s, uint8_t *server_ip, uint16_t server_port );
void network_init(net_config_t local_net_config);
/*设置心跳包*/
void setkeepalive(SOCKET s);
#endif
