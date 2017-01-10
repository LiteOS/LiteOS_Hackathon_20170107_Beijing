#include <stdio.h>
#include <string.h>

#include "config.h"
#include "socket.h"
#include "w5500.h"
#include "dhcp.h"
#include "sockutil.h"
#include "ult.h"//delay
#include "network_conf.h"
#define DHCP_DEBUG
#define DEVICE_ID "W5500"
uint8*  SRC_MAC_ADDR   =  EXTERN_DHCP_MAC;    // Local MAC address
uint8*   GET_SN_MASK    = EXTERN_DHCP_SN;     // Subnet mask received from the DHCP server
uint8*   GET_GW_IP      = EXTERN_DHCP_GW;     // Gateway ip address received from the DHCP server
uint8*   GET_DNS_IP     = EXTERN_DHCP_DNS;    // DNS server ip address received from the DHCP server
uint8*   DHCP_HOST_NAME = EXTERN_DHCP_NAME;   // HOST NAME
uint8*   GET_SIP        = EXTERN_DHCP_SIP;    // Local ip address received from the DHCP server
uint8    DHCP_SIP[4] = {0,};      // DNS server ip address is discovered
uint8    DHCP_REAL_SIP[4] = {0,}; // For extract my DHCP server in a few DHCP servers
uint8    OLD_SIP[4];        // Previous local ip address received from DHCP server

uint8  dhcp_state       = STATE_DHCP_READY;          // DHCP client status
uint8  dhcp_retry_count = 0;    // retry count
uint8  DHCP_timeout     = 0;        // DHCP Timeout flag
uint32  dhcp_lease_time;    // Leased time
uint32  dhcp_time       = 0;
uint32  next_dhcp_time  = 0;      // DHCP Time 1s
uint32  dhcp_tick_cnt   = 0;                    // 1ms
uint8  DHCP_timer;

uint32  DHCP_XID        = DEFAULT_XID;				
uint8 EXTERN_DHCPBUF[1024];
RIP_MSG*  pRIPMSG = (RIP_MSG*)EXTERN_DHCPBUF;         // Pointer for the DHCP message


void  send_DHCP_DISCOVER(void);// Send the discovery message to the DHCP server
void  send_DHCP_REQUEST(void);// Send the request message to the DHCP server
void  send_DHCP_RELEASE_DECLINE(char msgtype);// send the release message to the DHCP server
uint8 parseDHCPMSG(uint16 length);// Receive the message from DHCP server and parse it.
void  reset_DHCP_time(void);        // Initialize DHCP Timer
void  DHCP_timer_handler(void);     // DHCP Timer handler
uint8 check_leasedIP(void);         // Check the leased IP address
void  check_DHCP_Timeout(void);     // Check DHCP Timeout 

/*
*********************************************************************************************************
*              SEND DHCP DISCOVER
*
* Description: reset timeout value and retry count
* Arguments  :
* Returns    : 
* Note       : 
*********************************************************************************************************
*/
void reset_DHCP_time(void)
{
	dhcp_time = 0;
	dhcp_tick_cnt = 0;
	next_dhcp_time = dhcp_time + DHCP_WAIT_TIME;
	dhcp_retry_count = 0;
}

/*
*********************************************************************************************************
*              SEND DHCP DISCOVER
*
* Description: This function sends DHCP DISCOVER message to DHCP server.
* Arguments  : s - is a socket number.
* Returns    : None.
* Note       : 
*********************************************************************************************************
*/
void send_DHCP_DISCOVER(void)
{
  uint8 ip[4] = {255,255,255,255};
  uint16 i=0;
  //the host name modified
  uint8 host_name[12];
  //*((uint32*)DHCP_SIP)=0;
  //*((uint32*)DHCP_REAL_SIP)=0;
  memset((void*)pRIPMSG,0,sizeof(RIP_MSG));
  
  pRIPMSG->op = DHCP_BOOTREQUEST;
  pRIPMSG->htype = DHCP_HTYPE10MB;
  pRIPMSG->hlen = DHCP_HLENETHERNET;
  pRIPMSG->hops = DHCP_HOPS;
  pRIPMSG->xid = htonl(DHCP_XID);
  pRIPMSG->secs = htons(DHCP_SECS);
  pRIPMSG->flags =htons(DHCP_FLAGSBROADCAST);
  pRIPMSG->chaddr[0] = SRC_MAC_ADDR[0];
  pRIPMSG->chaddr[1] = SRC_MAC_ADDR[1];
  pRIPMSG->chaddr[2] = SRC_MAC_ADDR[2];
  pRIPMSG->chaddr[3] = SRC_MAC_ADDR[3];
  pRIPMSG->chaddr[4] = SRC_MAC_ADDR[4];
  pRIPMSG->chaddr[5] = SRC_MAC_ADDR[5];    
  
  
  /* MAGIC_COOKIE */
  pRIPMSG->OPT[i++] = (uint8)((MAGIC_COOKIE >> 24)& 0xFF);
  pRIPMSG->OPT[i++] = (uint8)((MAGIC_COOKIE >> 16)& 0xFF);
  pRIPMSG->OPT[i++] = (uint8)((MAGIC_COOKIE >> 8)& 0xFF);
  pRIPMSG->OPT[i++] = (uint8)(MAGIC_COOKIE& 0xFF);
  
  /* Option Request Param. */
  pRIPMSG->OPT[i++] = dhcpMessageType;
  pRIPMSG->OPT[i++] = 0x01;
  pRIPMSG->OPT[i++] = DHCP_DISCOVER;
  
  // Client identifier
  pRIPMSG->OPT[i++] = dhcpClientIdentifier;
  pRIPMSG->OPT[i++] = 0x07;
  pRIPMSG->OPT[i++] = 0x01;
  pRIPMSG->OPT[i++] = SRC_MAC_ADDR[0];
  pRIPMSG->OPT[i++] = SRC_MAC_ADDR[1];
  pRIPMSG->OPT[i++] = SRC_MAC_ADDR[2];
  pRIPMSG->OPT[i++] = SRC_MAC_ADDR[3];
  pRIPMSG->OPT[i++] = SRC_MAC_ADDR[4];
  pRIPMSG->OPT[i++] = SRC_MAC_ADDR[5];
  
  // host name
  pRIPMSG->OPT[i++] = hostName;
  // set the host name
  sprintf((char*)host_name,"%.4s-%02X%02X%02X",DEVICE_ID,SRC_MAC_ADDR[3],SRC_MAC_ADDR[4],SRC_MAC_ADDR[5]);
  
  
  pRIPMSG->OPT[i++] = (uint8)strlen((char*)host_name);
  
  strcpy((char*)(&(pRIPMSG->OPT[i])),(char*)host_name);
 
  
  i+=(uint16)strlen((char*)host_name);	
 
  pRIPMSG->OPT[i++] = dhcpParamRequest;
  pRIPMSG->OPT[i++] = 0x06;
  pRIPMSG->OPT[i++] = subnetMask;
  pRIPMSG->OPT[i++] = routersOnSubnet;
  pRIPMSG->OPT[i++] = dns;
  pRIPMSG->OPT[i++] = domainName;
  pRIPMSG->OPT[i++] = dhcpT1value;
  pRIPMSG->OPT[i++] = dhcpT2value;
  pRIPMSG->OPT[i++] = endOption;

  /* send broadcasting packet */
  //printf("send dhcp discover %s\r\n",EXTERN_DHCPBUF);
  //for(uint8 i=0; i<3; i++)
  //Delay_ms(800);
  sendto(SOCK_DHCP, (uint8 *)pRIPMSG, sizeof(RIP_MSG), ip, DHCP_SERVER_PORT);	

#ifdef DHCP_DEBUG	
	printf("sent DHCP_DISCOVER\r\n");
#endif	
}

/*
*********************************************************************************************************
*              SEND DHCP REQUEST
*
* Description: This function sends DHCP REQUEST message to DHCP server.
* Arguments  : s - is a socket number.
* Returns    : None.
* Note       : 
*********************************************************************************************************
*/
void send_DHCP_REQUEST(void)
{
  uint8  ip[4];
  uint16 i = 0;
  //uint16 len=0;
  
  uint8  host_name[12];

  memset((void*)pRIPMSG,0,sizeof(RIP_MSG));
  
  pRIPMSG->op = DHCP_BOOTREQUEST;
  pRIPMSG->htype = DHCP_HTYPE10MB;
  pRIPMSG->hlen = DHCP_HLENETHERNET;
  pRIPMSG->hops = DHCP_HOPS;
  pRIPMSG->xid = htonl(DHCP_XID);
  pRIPMSG->secs = htons(DHCP_SECS);
  
  if(dhcp_state < STATE_DHCP_LEASED)
    pRIPMSG->flags = htons(DHCP_FLAGSBROADCAST);
  else
  {
    pRIPMSG->flags = 0;		// For Unicast
    memcpy(pRIPMSG->ciaddr,GET_SIP,4);
  }		

  memcpy(pRIPMSG->chaddr,SRC_MAC_ADDR,6);
  
  /* MAGIC_COOKIE */
  pRIPMSG->OPT[i++] = (uint8)((MAGIC_COOKIE >> 24) & 0xFF);
  pRIPMSG->OPT[i++] = (uint8)((MAGIC_COOKIE >> 16) & 0xFF);
  pRIPMSG->OPT[i++] = (uint8)((MAGIC_COOKIE >> 8) & 0xFF);
  pRIPMSG->OPT[i++] = (uint8)(MAGIC_COOKIE & 0xFF);
  
  /* Option Request Param. */
  pRIPMSG->OPT[i++] = dhcpMessageType;
  pRIPMSG->OPT[i++] = 0x01;
  pRIPMSG->OPT[i++] = DHCP_REQUEST;
  
  pRIPMSG->OPT[i++] = dhcpClientIdentifier;
  pRIPMSG->OPT[i++] = 0x07;
  pRIPMSG->OPT[i++] = 0x01;
  pRIPMSG->OPT[i++] = SRC_MAC_ADDR[0];
  pRIPMSG->OPT[i++] = SRC_MAC_ADDR[1];
  pRIPMSG->OPT[i++] = SRC_MAC_ADDR[2];
  pRIPMSG->OPT[i++] = SRC_MAC_ADDR[3];
  pRIPMSG->OPT[i++] = SRC_MAC_ADDR[4];
  pRIPMSG->OPT[i++] = SRC_MAC_ADDR[5];	
  
  
  if(dhcp_state < STATE_DHCP_LEASED)
  {
    pRIPMSG->OPT[i++] = dhcpRequestedIPaddr;
    pRIPMSG->OPT[i++] = 0x04;
    pRIPMSG->OPT[i++] = GET_SIP[0];
    pRIPMSG->OPT[i++] = GET_SIP[1];
    pRIPMSG->OPT[i++] = GET_SIP[2];
    pRIPMSG->OPT[i++] = GET_SIP[3];
  
    pRIPMSG->OPT[i++] = dhcpServerIdentifier;
    pRIPMSG->OPT[i++] = 0x04;
    pRIPMSG->OPT[i++] = DHCP_SIP[0];
    pRIPMSG->OPT[i++] = DHCP_SIP[1];
    pRIPMSG->OPT[i++] = DHCP_SIP[2];
    pRIPMSG->OPT[i++] = DHCP_SIP[3];
  }
  
  // host name
  pRIPMSG->OPT[i++] = hostName;
  //set the host name
  sprintf((char*)host_name,(char*)"%.4s-%02X%02X%02X",DEVICE_ID,SRC_MAC_ADDR[3],SRC_MAC_ADDR[4],SRC_MAC_ADDR[5]);
  pRIPMSG->OPT[i++] = (uint8)strlen((char*)host_name);

  strcpy((char*)&(pRIPMSG->OPT[i]),(char*)host_name);

  i+=strlen((char*)host_name);
	
  
  pRIPMSG->OPT[i++] = dhcpParamRequest;
  pRIPMSG->OPT[i++] = 0x08;
  pRIPMSG->OPT[i++] = subnetMask;
  pRIPMSG->OPT[i++] = routersOnSubnet;
  pRIPMSG->OPT[i++] = dns;
  pRIPMSG->OPT[i++] = domainName;
  pRIPMSG->OPT[i++] = dhcpT1value;
  pRIPMSG->OPT[i++] = dhcpT2value;
  pRIPMSG->OPT[i++] = performRouterDiscovery;
  pRIPMSG->OPT[i++] = staticRoute;
  pRIPMSG->OPT[i++] = endOption;
  
  /* send broadcasting packet */
  if(dhcp_state < STATE_DHCP_LEASED) memset(ip,0xFF,4);
  else 
    memcpy(ip,DHCP_SIP,4);
  //printf("send dhcp request\r\n");
    sendto(SOCK_DHCP, (const uint8 *)pRIPMSG, sizeof(RIP_MSG), ip, DHCP_SERVER_PORT);
  //if(len==0)
    //printf("send request error\r\n");
#ifdef DHCP_DEBUG
	printf("sent DHCP_REQUEST\r\n");
#endif
}


/*
*********************************************************************************************************
*              SEND DHCP RELEASE
*
* Description: This function sends DHCP RELEASE message to DHCP server.
* Arguments  : s - is a socket number.
*              msgtype - 0 : RELEASE, Not Zero : DECLINE
* Returns    : None.
* Note       : 
*********************************************************************************************************
*/
void send_DHCP_RELEASE_DECLINE(char msgtype)
{
  uint16 i =0;
  uint8  ip[4];
  
  memset((void*)pRIPMSG,0,sizeof(RIP_MSG));
  
  pRIPMSG->op = DHCP_BOOTREQUEST;
  pRIPMSG->htype = DHCP_HTYPE10MB;
  pRIPMSG->hlen = DHCP_HLENETHERNET;
  pRIPMSG->hops = DHCP_HOPS;
  pRIPMSG->xid = htonl(DHCP_XID);
  pRIPMSG->secs = htons(DHCP_SECS);
  pRIPMSG->flags =0;// DHCP_FLAGSBROADCAST;
  
  memcpy(pRIPMSG->chaddr,SRC_MAC_ADDR,6);
  
  /* MAGIC_COOKIE */
  pRIPMSG->OPT[i++] = (uint8)((MAGIC_COOKIE >> 24) & 0xFF);
  pRIPMSG->OPT[i++] = (uint8)((MAGIC_COOKIE >> 16) & 0xFF);
  pRIPMSG->OPT[i++] = (uint8)((MAGIC_COOKIE >> 8) & 0xFF);
  pRIPMSG->OPT[i++] = (uint8)(MAGIC_COOKIE & 0xFF);
  
  /* Option Request Param. */
  pRIPMSG->OPT[i++] = dhcpMessageType;
  pRIPMSG->OPT[i++] = 0x01;
  pRIPMSG->OPT[i++] = ((!msgtype) ? DHCP_RELEASE : DHCP_DECLINE);
  
  pRIPMSG->OPT[i++] = dhcpClientIdentifier;
  pRIPMSG->OPT[i++] = 0x07;
  pRIPMSG->OPT[i++] = 0x01;
  pRIPMSG->OPT[i++] = SRC_MAC_ADDR[0];
  pRIPMSG->OPT[i++] = SRC_MAC_ADDR[1];
  pRIPMSG->OPT[i++] = SRC_MAC_ADDR[2];
  pRIPMSG->OPT[i++] = SRC_MAC_ADDR[3];
  pRIPMSG->OPT[i++] = SRC_MAC_ADDR[4];
  pRIPMSG->OPT[i++] = SRC_MAC_ADDR[5];	
  
  pRIPMSG->OPT[i++] = dhcpServerIdentifier;
  pRIPMSG->OPT[i++] = 0x04;
  pRIPMSG->OPT[i++] = DHCP_SIP[0];
  pRIPMSG->OPT[i++] = DHCP_SIP[1];
  pRIPMSG->OPT[i++] = DHCP_SIP[2];
  pRIPMSG->OPT[i++] = DHCP_SIP[3];
  
  if(msgtype)
  {
    pRIPMSG->OPT[i++] = dhcpRequestedIPaddr;
    pRIPMSG->OPT[i++] = 0x04;
    pRIPMSG->OPT[i++] = GET_SIP[0];
    pRIPMSG->OPT[i++] = GET_SIP[1];
    pRIPMSG->OPT[i++] = GET_SIP[2];
    pRIPMSG->OPT[i++] = GET_SIP[3];		
  #ifdef DHCP_DEBUG		
    printf("sent DHCP_DECLINE\r\n");
  #endif		
  }
  #ifdef DHCP_DEBUG	
  else
  {
    printf("sent DHCP_RELEASE\r\n");
  }
  #endif	
  
  pRIPMSG->OPT[i++] = endOption;	
  
  
  if(msgtype) memset(ip,0xFF,4);
  else 
    memcpy(ip,DHCP_SIP,4);
  //printf("send dhcp decline\r\n");
  sendto(SOCK_DHCP, (uint8 *)pRIPMSG, sizeof(RIP_MSG), ip, DHCP_SERVER_PORT);
}

/*
*********************************************************************************************************
*              PARSE REPLY MSG
*
* Description: This function parses the reply message from DHCP server.
* Arguments  : s      - is a socket number.
*              length - is a size data to receive.
* Returns    : success - return type, fail - 0
* Note       : 
*********************************************************************************************************
*/
uint8 parseDHCPMSG(uint16 length)
{
  uint8  svr_addr[6];
  uint16 svr_port;
  uint16 len;
  uint8  * p;
  uint8  * e;
  uint8  type;
  uint8  opt_len = 0;
  
  len = recvfrom(SOCK_DHCP, (uint8 *)pRIPMSG, length, svr_addr, &svr_port);
  
  #ifdef DHCP_DEBUG
    printf("DHCP_SIP:%u.%u.%u.%u\r\n",DHCP_SIP[0],DHCP_SIP[1],DHCP_SIP[2],DHCP_SIP[3]);
    printf("DHCP_RIP:%u.%u.%u.%u\r\n",DHCP_REAL_SIP[0],DHCP_REAL_SIP[1],DHCP_REAL_SIP[2],DHCP_REAL_SIP[3]);
    printf("svr_addr:%u.%u.%u.%u\r\n",svr_addr[0],svr_addr[1],svr_addr[2],svr_addr[3]);
  #endif	
  
  if(pRIPMSG->op != DHCP_BOOTREPLY || svr_port != DHCP_SERVER_PORT)
  {
    #ifdef DHCP_DEBUG	   
      printf("DHCP : NO DHCP MSG\r\n");
    #endif		
    return 0;
  }
  if(memcmp(pRIPMSG->chaddr,SRC_MAC_ADDR,6) != 0 || pRIPMSG->xid != htonl(DHCP_XID))
  {
    #ifdef DHCP_DEBUG
      printf("No My DHCP Message. This message is ignored.\r\n");
      printf("\tSRC_MAC_ADDR(%02X.%02X.%02X.",SRC_MAC_ADDR[0],SRC_MAC_ADDR[1],SRC_MAC_ADDR[2]);
      printf("%02X.%02X.%02X)",SRC_MAC_ADDR[3],SRC_MAC_ADDR[4],SRC_MAC_ADDR[5]);
      printf(", pRIPMSG->chaddr(%02X.%02X.%02X.",(char)pRIPMSG->chaddr[0],(char)pRIPMSG->chaddr[1],(char)pRIPMSG->chaddr[2]);
      printf("%02X.%02X.%02X)",pRIPMSG->chaddr[3],pRIPMSG->chaddr[4],pRIPMSG->chaddr[5]);
      printf("\tpRIPMSG->xid(%08X), DHCP_XID(%08X)",pRIPMSG->xid,(DHCP_XID));
      printf("\tpRIMPMSG->yiaddr:%d.%d.%d.%d\r\n",pRIPMSG->yiaddr[0],pRIPMSG->yiaddr[1],pRIPMSG->yiaddr[2],pRIPMSG->yiaddr[3]);
    #endif				
    return 0;
  }
  
  if( *((uint32*)DHCP_SIP) != 0x00000000 )
  {
    if( *((uint32*)DHCP_REAL_SIP) != *((uint32*)svr_addr) && 
            *((uint32*)DHCP_SIP) != *((uint32*)svr_addr) ) 
    {
#ifdef DHCP_DEBUG		
            printf("Another DHCP sever send a response message. This is ignored.\r\n");
            printf("\tIP:%u.%u.%u.%u\r\n",svr_addr[0],svr_addr[1],svr_addr[2],svr_addr[3]);
#endif				
      return 0;								
    }
  }
  
  memcpy(GET_SIP,pRIPMSG->yiaddr,4);
  
  #ifdef DHCP_DEBUG	
    printf("DHCP MSG received\r\n");
    printf("yiaddr : %u.%u.%u.%u\r\n",GET_SIP[0],GET_SIP[1],GET_SIP[2],GET_SIP[3]);
  #endif	
  
  type = 0;
  p = (uint8 *)(&pRIPMSG->op);
  p = p + 240;
  e = p + (len - 240);
  #ifdef DHCP_DEBUG
   printf("p : %08X  e : %08X  len : %d\r\n", (uint32)p, (uint32)e, len);
  #endif
  while ( p < e ) 
  {
    switch ( *p++ ) 
    {
    case endOption :
      return type;
              
      case padOption :
      break;
    case dhcpMessageType :
      opt_len = *p++;
      type = *p;
#ifdef DHCP_DEBUG			
      printf("dhcpMessageType : %02X\r\n", type);
#endif			

      break;
    case subnetMask :
      opt_len =* p++;
      memcpy(GET_SN_MASK,p,4);
#ifdef DHCP_DEBUG
      printf("subnetMask : ");
      printf("%u.%u.%u.%u\r\n",GET_SN_MASK[0],GET_SN_MASK[1],GET_SN_MASK[2],GET_SN_MASK[3]);
#endif			
      break;
    case routersOnSubnet :
      opt_len = *p++;
      memcpy(GET_GW_IP,p,4);
#ifdef DHCP_DEBUG
      printf("routersOnSubnet : ");
      printf("%u.%u.%u.%u\r\n",GET_GW_IP[0],GET_GW_IP[1],GET_GW_IP[2],GET_GW_IP[3]);
#endif			
      break;
    case dns :
      opt_len = *p++;
      memcpy(GET_DNS_IP,p,4);
      break;
    case dhcpIPaddrLeaseTime :
      opt_len = *p++;
      dhcp_lease_time = ntohl(*((uint32*)p));
#ifdef DHCP_DEBUG			
      printf("dhcpIPaddrLeaseTime : %d\r\n", dhcp_lease_time);
#endif			
      break;
    case dhcpServerIdentifier :
      opt_len = *p++;
#ifdef DHCP_DEBUG						
      printf("DHCP_SIP : %u.%u.%u.%u\r\n", DHCP_SIP[0], DHCP_SIP[1], DHCP_SIP[2], DHCP_SIP[3]);
#endif			
      if( *((uint32*)DHCP_SIP) == 0 || 
          *((uint32*)DHCP_REAL_SIP) == *((uint32*)svr_addr) || 
          *((uint32*)DHCP_SIP) == *((uint32*)svr_addr) )
      {
        memcpy(DHCP_SIP,p,4);
        memcpy(DHCP_REAL_SIP,svr_addr,4);	// Copy the real ip address of my DHCP server
#ifdef DHCP_DEBUG						
        printf("My dhcpServerIdentifier : ");
        printf("%u.%u.%u.%u\r\n", DHCP_SIP[0], DHCP_SIP[1], DHCP_SIP[2], DHCP_SIP[3]);
        printf("My DHCP server real IP address : ");
        printf("%u.%u.%u.%u\r\n", DHCP_REAL_SIP[0], DHCP_REAL_SIP[1], DHCP_REAL_SIP[2], DHCP_REAL_SIP[3]);
#endif						
      }
      else
      {
#ifdef DHCP_DEBUG			   
        printf("Another dhcpServerIdentifier : \r\n");
        printf("\tMY(%u.%u.%u.%u) ", DHCP_SIP[0], DHCP_SIP[1], DHCP_SIP[2], DHCP_SIP[3]);
        printf("Another(%u.%u.%u.%u)\r\n", svr_addr[0], svr_addr[1], svr_addr[2], svr_addr[3]);
#endif				
      }
      break;
    default :
      opt_len = *p++;
#ifdef DHCP_DEBUG			
      printf("opt_len : %u\r\n", opt_len);
#endif			
      break;
    } // switch
    p+=opt_len;
  } // while
  
  return 0;
}

/*
*********************************************************************************************************
*              CHECK DHCP STATE
*
* Description: This function checks the state of DHCP.
* Arguments  : None.
* Returns    : None.
* Note       : 
*********************************************************************************************************
*/
uint8 check_DHCP_state(SOCKET s) 
{
  uint16 len;
  uint8  type;
  
  type = 0;
  if(getSn_SR(s)!=SOCK_CLOSED)
  {
    if ((len = getSn_RX_RSR(s)) > 0)
    {
      type = parseDHCPMSG(len);
    }
  }
  else
  {
    if(dhcp_state == STATE_DHCP_READY)
    {
       //init_dhcp_client();
    #ifdef DHCP_DEBUG			
     printf("state : STATE_DHCP_READY\r\n");
    #endif	   	
    }
    
    
    if(!socket(SOCK_DHCP,Sn_MR_UDP,DHCP_CLIENT_PORT,0x00))
    {
    #ifdef DHCP_DEBUG	   
               printf("Fail to create the DHCPC_SOCK(%u)\r\n",SOCK_DHCP);
    #endif   
     return DHCP_RET_ERR;
    }
    
  }
  
  switch ( dhcp_state )
  {
    case STATE_DHCP_READY:
      DHCP_timeout = 0;
      reset_DHCP_time();
      send_DHCP_DISCOVER();
      
      DHCP_timer = 0;//set_timer0(DHCP_timer_handler);   	
      dhcp_state = STATE_DHCP_DISCOVER;
      break;	   
    case STATE_DHCP_DISCOVER :
      if (type == DHCP_OFFER) 
      {
        reset_DHCP_time();
        send_DHCP_REQUEST();
        dhcp_state = STATE_DHCP_REQUEST;
#ifdef DHCP_DEBUG			
        printf("state : STATE_DHCP_REQUEST\r\n");
#endif			
      }
      else 
        check_DHCP_Timeout();
      break;
    
    case STATE_DHCP_REQUEST :
      if (type == DHCP_ACK) 
      {
        reset_DHCP_time();
        if (check_leasedIP()) 
        {
#ifdef DHCP_DEBUG					
          printf("state : STATE_DHCP_LEASED\r\n");
#endif		
          dhcp_state = STATE_DHCP_LEASED;
          return DHCP_RET_UPDATE;
        } 
        else 
        {
#ifdef DHCP_DEBUG					
          printf("state : STATE_DHCP_DISCOVER\r\n");
#endif				
          dhcp_state = STATE_DHCP_DISCOVER;
          return DHCP_RET_CONFLICT;
        }
      }
      else if (type == DHCP_NAK) 
      {
        reset_DHCP_time();
        dhcp_state = STATE_DHCP_DISCOVER;
#ifdef DHCP_DEBUG				
        printf("state : STATE_DHCP_DISCOVER\r\n");
#endif			
      }
      else 
        check_DHCP_Timeout();
      break;
    
    case STATE_DHCP_LEASED :
      if ((dhcp_lease_time != 0xffffffff) && (dhcp_time>(dhcp_lease_time/2))) 
      {
        type = 0;
        memcpy(OLD_SIP,GET_SIP,4);
        DHCP_XID++;
        send_DHCP_REQUEST();
        dhcp_state = STATE_DHCP_REREQUEST;
#ifdef DHCP_DEBUG
        printf("state : STATE_DHCP_REREQUEST\r\n");
#endif
        reset_DHCP_time();
      }
      break;
    
    case STATE_DHCP_REREQUEST :
      if (type == DHCP_ACK) 
      {
        if(memcmp(OLD_SIP,GET_SIP,4)!=0)	
        {
#ifdef DHCP_DEBUG
          printf("The IP address from the DHCP server is updated.\r\n");
          printf("OLD_SIP=%u.%u.%u.%u",OLD_SIP[0],OLD_SIP[1],OLD_SIP[2],OLD_SIP[3]);
          printf("GET_SIP=%u.%u.%u.%u\r\n",GET_SIP[0],GET_SIP[1],GET_SIP[2],GET_SIP[3]);
#endif
          return DHCP_RET_UPDATE;
        }
#ifdef DHCP_DEBUG
        else
        {
          printf("state : STATE_DHCP_LEASED : same IP\r\n");
        }
#endif
        reset_DHCP_time();
        dhcp_state = STATE_DHCP_LEASED;
      } 
      else if (type == DHCP_NAK) 
      {
        reset_DHCP_time();
        dhcp_state = STATE_DHCP_DISCOVER;
#ifdef DHCP_DEBUG
        printf("state : STATE_DHCP_DISCOVER\r\n");
#endif
      } 
      else 
        check_DHCP_Timeout();
      break;
    default :
    dhcp_state = STATE_DHCP_READY;
    break;
  }
  
  if (DHCP_timeout == 1)
  {
    //kill_timer0(DHCP_timer);
    dhcp_state = STATE_DHCP_READY;
    return DHCP_RET_TIMEOUT;
  }
  return DHCP_RET_NONE;
}

/*
*********************************************************************************************************
*              CHECK TIMEOUT
*
* Description: This function checks the timeout of DHCP in each state.
* Arguments  : None.
* Returns    : None.
* Note       : 
*********************************************************************************************************
*/
void check_DHCP_Timeout(void)
{
  if (dhcp_retry_count < MAX_DHCP_RETRY) 
  {
    if (dhcp_time > next_dhcp_time) 
    {
      dhcp_time = 0;
      next_dhcp_time = dhcp_time + DHCP_WAIT_TIME;
      dhcp_retry_count++;
      switch ( dhcp_state ) 
      {
      case STATE_DHCP_DISCOVER :
  #ifdef DHCP_DEBUG			   
        printf("<<timeout>> state : STATE_DHCP_DISCOVER\r\n");
  #endif				
        send_DHCP_DISCOVER();
        break;
      case STATE_DHCP_REQUEST :
  #ifdef DHCP_DEBUG			   
        printf("<<timeout>> state : STATE_DHCP_REQUEST\r\n");
  #endif				
        send_DHCP_REQUEST();
        break;

      case STATE_DHCP_REREQUEST :
  #ifdef DHCP_DEBUG			   
        printf("<<timeout>> state : STATE_DHCP_REREQUEST\r\n");
  #endif				
        send_DHCP_REQUEST();
        break;
      default :
        break;
      }
    }
  } 
  else 
  {
    reset_DHCP_time();
    DHCP_timeout = 1;
    
    send_DHCP_DISCOVER();
    dhcp_state = STATE_DHCP_DISCOVER;
#ifdef DHCP_DEBUG		
    printf("timeout : state : STATE_DHCP_DISCOVER\r\n");
#endif		
  }
}


/*
*********************************************************************************************************
* Description: check if a leased IP is valid
* Arguments  : None.
* Returns    : None.
* Note       : 
*********************************************************************************************************
*/
uint8 check_leasedIP(void)
{
  
  #ifdef DHCP_DEBUG
  printf("<Check the IP Conflict %d.%d.%d.%d: ",GET_SIP[0],GET_SIP[1],GET_SIP[2],GET_SIP[3]);
  #endif
  // sendto is complete. that means there is a node which has a same IP.
  
  if ( sendto(SOCK_DHCP, (const uint8*)"CHECK_IP_CONFLICT", 17, GET_SIP, 5000))
  {
  #ifdef DHCP_DEBUG
    printf(" Conflict>\r\n");
  #endif
    send_DHCP_RELEASE_DECLINE(1);
    return 0;
  }
  #ifdef DHCP_DEBUG
    printf(" No Conflict>\r\n");
  #endif
  return 1;

}	

/*
*********************************************************************************************************
*              
*
* Description: timer interrupt handler(For checking dhcp lease time).
* Arguments  : irq - interrupt offset
*              p   - pointer to parameter
* Returns    : None.
* Note       : Increase 'my_time' each one second.
*********************************************************************************************************
*/
void DHCP_timer_handler(void)
{
  if(dhcp_tick_cnt++ > 1000)
  {
    dhcp_tick_cnt = 0;
    dhcp_time++;
  }
}

/*
*********************************************************************************************************
*              
*
* Description: Initialize the DHCP client
* Arguments  : s - Socket number for the DHCP client
               ip_update - handler called when the leased IP address is updated
               ip_conflict - handler called when the leased IP address is conflict
*              p   - pointer to parameter
* Returns    : None.
* Note       : 
*********************************************************************************************************
*/
void init_dhcp_client(void)
{
  uint8 txsize[MAX_SOCK_NUM] = {2,2,2,2,2,2,2,2};
  uint8 rxsize[MAX_SOCK_NUM] = {2,2,2,2,2,2,2,2};
  //uint8 ip_broadcast[4]={255,};
  uint8 ip_0[4]={0,};
  DHCP_XID = 0x12345678;
  memset(OLD_SIP,0,sizeof(OLD_SIP));
  memset(DHCP_SIP,0,sizeof(DHCP_SIP));
  memset(DHCP_REAL_SIP,0,sizeof(GET_SN_MASK));

  iinchip_init();
  
  setSHAR(ConfigMsg.mac);
  setSUBR(ip_0);
  setGAR(ip_0);
  setSIPR(ip_0);
  printf("mac=%02x:%02x:%02x:%02x:%02x:%02x\r\n",SRC_MAC_ADDR[0],SRC_MAC_ADDR[1],SRC_MAC_ADDR[2],SRC_MAC_ADDR[3],SRC_MAC_ADDR[4],SRC_MAC_ADDR[5]);  
  sysinit(txsize, rxsize); 
  //clear ip setted flag

  dhcp_state = STATE_DHCP_READY;
#ifdef DHCP_DEBUG
   printf("init_dhcp_client:%u\r\n",SOCK_DHCP);
#endif   
}


