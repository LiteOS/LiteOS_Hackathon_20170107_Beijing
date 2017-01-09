#include "socket.h"
#include "config.h"
#include "stdio.h"
#include "w5500.h"
#include "ult.h"
#include "stdarg.h"	 	 	 	 
#include "string.h"
#include "types.h"
#include "usart3.h"
static uint16 local_port;
extern uint16 sent_ptr;
uint8_t socket_muer[MAX_TX_DATA];
/**
@brief   This Socket function initialize the channel in perticular mode, and set the port and wait for W5200 done it.
@return  1 for sucess else 0.
*/
uint8 socket(SOCKET s, uint8 protocol, uint16 port, uint8 flag)
{
   uint8 ret;
   if (
        ((protocol&0x0F) == Sn_MR_TCP)    ||
        ((protocol&0x0F) == Sn_MR_UDP)    ||
        ((protocol&0x0F) == Sn_MR_IPRAW)  ||
        ((protocol&0x0F) == Sn_MR_MACRAW) ||
        ((protocol&0x0F) == Sn_MR_PPPOE)
      )
   {
      close(s);
      IINCHIP_WRITE(Sn_MR(s) ,protocol | flag);
      if (port != 0) {
         IINCHIP_WRITE( Sn_PORT0(s) ,(uint8)((port & 0xff00) >> 8));
         IINCHIP_WRITE( Sn_PORT1(s) ,(uint8)(port & 0x00ff));
      } else {
         local_port++; // if don't set the source port, set local_port number.
         IINCHIP_WRITE(Sn_PORT0(s) ,(uint8)((local_port & 0xff00) >> 8));
         IINCHIP_WRITE(Sn_PORT1(s) ,(uint8)(local_port & 0x00ff));
      }
      IINCHIP_WRITE( Sn_CR(s) ,Sn_CR_OPEN); // run sockinit Sn_CR

      /* wait to process the command... */
      while( IINCHIP_READ(Sn_CR(s)) )
         ;
      /* ------- */
      ret = 1;
   }
   else
   {
      ret = 0;
   }
   return ret;
}


/**
@brief   This function close the socket and parameter is "s" which represent the socket number
*/
void close(SOCKET s)
{

   IINCHIP_WRITE( Sn_CR(s) ,Sn_CR_CLOSE);

   /* wait to process the command... */
   while( IINCHIP_READ(Sn_CR(s) ) )
      ;
   /* ------- */
        /* all clear */
   IINCHIP_WRITE( Sn_IR(s) , 0xFF);
}


/**
@brief   This function established  the connection for the channel in passive (server) mode. This function waits for the request from the peer.
@return  1 for success else 0.
*/
uint8 listen(SOCKET s)
{
   uint8 ret;
   if (IINCHIP_READ( Sn_SR(s) ) == SOCK_INIT)
   {
      IINCHIP_WRITE( Sn_CR(s) ,Sn_CR_LISTEN);
      /* wait to process the command... */
      while( IINCHIP_READ(Sn_CR(s) ) )
         ;
      /* ------- */
      ret = 1;
   }
   else
   {
      ret = 0;
   }
   return ret;
}


/**
@brief   This function established  the connection for the channel in Active (client) mode.
      This function waits for the untill the connection is established.

@return  1 for success else 0.
*/
uint8 connect(SOCKET s, uint8 * addr, uint16 port)
{
    uint8 ret;
    if
        (
            ((addr[0] == 0xFF) && (addr[1] == 0xFF) && (addr[2] == 0xFF) && (addr[3] == 0xFF)) ||
            ((addr[0] == 0x00) && (addr[1] == 0x00) && (addr[2] == 0x00) && (addr[3] == 0x00)) ||
            (port == 0x00)
        )
    {
      ret = 0;
    }
    else
    {
        ret = 1;
        // set destination IP
        IINCHIP_WRITE( Sn_DIPR0(s), addr[0]);
        IINCHIP_WRITE( Sn_DIPR1(s), addr[1]);
        IINCHIP_WRITE( Sn_DIPR2(s), addr[2]);
        IINCHIP_WRITE( Sn_DIPR3(s), addr[3]);
        IINCHIP_WRITE( Sn_DPORT0(s), (uint8)((port & 0xff00) >> 8));
        IINCHIP_WRITE( Sn_DPORT1(s), (uint8)(port & 0x00ff));
        IINCHIP_WRITE( Sn_CR(s) ,Sn_CR_CONNECT);
        /* wait for completion */
        while ( IINCHIP_READ(Sn_CR(s) ) ) ;

        while ( IINCHIP_READ(Sn_SR(s)) != SOCK_SYNSENT )
        {
            if(IINCHIP_READ(Sn_SR(s)) == SOCK_ESTABLISHED)
            {
                break;
            }
            if (getSn_IR(s) & Sn_IR_TIMEOUT)
            {
                IINCHIP_WRITE(Sn_IR(s), (Sn_IR_TIMEOUT));  // clear TIMEOUT Interrupt
                ret = 0;
                break;
            }
        }
    }

   return ret;
}



/**
@brief   This function used for disconnect the socket and parameter is "s" which represent the socket number
@return  1 for success else 0.
*/
void disconnect(SOCKET s)
{
   IINCHIP_WRITE( Sn_CR(s) ,Sn_CR_DISCON);

   /* wait to process the command... */
   while( IINCHIP_READ(Sn_CR(s) ) )
      ;
   /* ------- */
}


/**
@brief   This function used to send the data in TCP mode
@return  1 for success else 0.
*/
uint16 send(SOCKET s, const uint8 * buf, uint16 len)
{
  uint8 status=0;
  uint16 ret=0;
  uint16 freesize=0;

  if (len > getIINCHIP_TxMAX(s)) ret = getIINCHIP_TxMAX(s); // check size not to exceed MAX size.
  else ret = len;

  // if freebuf is available, start.
  do
  {
    freesize = getSn_TX_FSR(s);
    status = IINCHIP_READ(Sn_SR(s));
    if ((status != SOCK_ESTABLISHED) && (status != SOCK_CLOSE_WAIT))
    {
      ret = 0;
      break;
    }
  } while (freesize < ret);


  // copy data
  send_data_processing(s, (uint8 *)buf, ret);
  IINCHIP_WRITE( Sn_CR(s) ,Sn_CR_SEND);

  /* wait to process the command... */
  while( IINCHIP_READ(Sn_CR(s) ) );

  while ( (IINCHIP_READ(Sn_IR(s) ) & Sn_IR_SEND_OK) != Sn_IR_SEND_OK )
  {
    status = IINCHIP_READ(Sn_SR(s));
    if ((status != SOCK_ESTABLISHED) && (status != SOCK_CLOSE_WAIT) )
    {
      usart3_printf("SEND_OK Problem!!\r\n");
      close(s);
      return 0;
    }
  }
  IINCHIP_WRITE( Sn_IR(s) , Sn_IR_SEND_OK);

#ifdef __DEF_IINCHIP_INT__
   putISR(s, getISR(s) & (~Sn_IR_SEND_OK));
#else
   IINCHIP_WRITE( Sn_IR(s) , Sn_IR_SEND_OK);
#endif

   return ret;
}
/*socket 发送，printf函数，支持\r\n, %d， %s*/
uint16_t socket_printf(SOCKET s1, char* fmt,...)  
{  
        const char *s;
        int32_t d;   //应该是有符号的数字
        char buf[64]; //最大32为字符串，转化为数字
        uint16 S_cnt=0;
        va_list ap;
        va_start(ap, fmt);
        //uint8_t xd[2]={0x0d,'\0'};
        //uint8_t xa[2]={0x0d,'\0'};
        //清空缓冲区，意义不大
        memset(socket_muer, 0,sizeof(socket_muer));//每次使用socket_muer缓冲区时，清空一下，目前不知道send函数传输的最大数据长度
        while ( *fmt != 0)     // 
        {                                                          
                if ( *fmt == 0x5c )  //'|'
                {                                                                          
                    switch ( *++fmt )
                    {
                            case 'r':                                                                  
                                    socket_muer[S_cnt] = 0x0d;   //0x0d \r
                                    S_cnt++;
                                    fmt ++;
                                    break;
                        
                            case 'n':                                                                  
                                    socket_muer[S_cnt] = 0x0a; //0x0a \n
                                    S_cnt++;
                                    fmt ++;
                                    break;
                            default:
                                    fmt ++;
                                break;
                    }                         
              }
              else if ( *fmt == '%')
              {                                                                          
                    switch ( *++fmt )
                    {                                
                            case 's':                                                                                 
                                    s = va_arg(ap, const char *); 
                                    for ( ; *s; s++) 
                                    {
                                            socket_muer[S_cnt++] = *s;
                                    }
                                    fmt++;
                                    break;
                
                            case 'd':                                                                                //???
                                    d = va_arg(ap, int32_t);
                                    itoa1(d, buf, 10);
                                    for (s = buf; *s; s++) 
                                    {
                                      socket_muer[S_cnt++] = *s;
                                    }
                                    fmt++;
                                    break;
                            default:
                                    fmt++;
                                    break;
                    }                
            } /* end of else if */
            else 
            {
              
                      socket_muer[S_cnt++] = *fmt++;
            }
      }
            send(s1, socket_muer, S_cnt);
            return S_cnt;
}

/**
@brief   This function is an application I/F function which is used to receive the data in TCP mode.
      It continues to wait for data as much as the application wants to receive.

@return  received data size for success else -1.
*/
uint16 recv(SOCKET s, uint8 * buf, uint16 len)
{
   uint16 ret=0;
   if ( len > 0 )
   {
      recv_data_processing(s, buf, len);
      IINCHIP_WRITE( Sn_CR(s) ,Sn_CR_RECV);
      /* wait to process the command... */
      while( IINCHIP_READ(Sn_CR(s) ));
      /* ------- */
      ret = len;
   }
   return ret;
}


/**
@brief   This function is an application I/F function which is used to send the data for other then TCP mode.
      Unlike TCP transmission, The peer's destination address and the port is needed.

@return  This function return send data size for success else -1.
*/
uint16 sendto(SOCKET s, const uint8 * buf, uint16 len, uint8 * addr, uint16 port)
{
   uint16 ret=0;

   if (len > getIINCHIP_TxMAX(s)) ret = getIINCHIP_TxMAX(s); // check size not to exceed MAX size.
   else ret = len;

   if( ((addr[0] == 0x00) && (addr[1] == 0x00) && (addr[2] == 0x00) && (addr[3] == 0x00)) || ((port == 0x00)) )//||(ret == 0) )
   {
      /* added return value */
      ret = 0;
   }
   else
   {
      IINCHIP_WRITE( Sn_DIPR0(s), addr[0]);
      IINCHIP_WRITE( Sn_DIPR1(s), addr[1]);
      IINCHIP_WRITE( Sn_DIPR2(s), addr[2]);
      IINCHIP_WRITE( Sn_DIPR3(s), addr[3]);
      IINCHIP_WRITE( Sn_DPORT0(s),(uint8)((port & 0xff00) >> 8));
      IINCHIP_WRITE( Sn_DPORT1(s),(uint8)(port & 0x00ff));
      // copy data
      send_data_processing(s, (uint8 *)buf, ret);
      IINCHIP_WRITE( Sn_CR(s) ,Sn_CR_SEND);
      /* wait to process the command... */
      while( IINCHIP_READ( Sn_CR(s) ) )
         ;
      /* ------- */

      while( (IINCHIP_READ( Sn_IR(s) ) & Sn_IR_SEND_OK) != Sn_IR_SEND_OK )
      {
         if (IINCHIP_READ( Sn_IR(s) ) & Sn_IR_TIMEOUT)
         {
            /* clear interrupt */
            IINCHIP_WRITE( Sn_IR(s) , (Sn_IR_SEND_OK | Sn_IR_TIMEOUT)); /* clear SEND_OK & TIMEOUT */
            return 0;
         }
      }
      IINCHIP_WRITE( Sn_IR(s) , Sn_IR_SEND_OK);
   }
   return ret;
}


/**
@brief   This function is an application I/F function which is used to receive the data in other then
   TCP mode. This function is used to receive UDP, IP_RAW and MAC_RAW mode, and handle the header as well.

@return  This function return received data size for success else -1.
*/
uint16 recvfrom(SOCKET s, uint8 * buf, uint16 len, uint8 * addr, uint16 *port)
{
   uint8 head[8];
   uint16 data_len=0;
   uint16 ptr=0;
   uint32 addrbsb =0;
   if ( len > 0 )
   {
      ptr     = IINCHIP_READ(Sn_RX_RD0(s) );
      ptr     = ((ptr & 0x00ff) << 8) + IINCHIP_READ(Sn_RX_RD1(s));
      addrbsb = (uint32)(ptr<<8) +  (s<<5) + 0x18;
      
      switch (IINCHIP_READ(Sn_MR(s) ) & 0x07)
      {
      case Sn_MR_UDP :
        wiz_read_buf(addrbsb, head, 0x08);        
        ptr += 8;
        // read peer's IP address, port number.
        addr[0]  = head[0];
        addr[1]  = head[1];
        addr[2]  = head[2];
        addr[3]  = head[3];
        *port    = head[4];
        *port    = (*port << 8) + head[5];
        data_len = head[6];
        data_len = (data_len << 8) + head[7];

        addrbsb = (uint32)(ptr<<8) +  (s<<5) + 0x18;
        wiz_read_buf(addrbsb, buf, data_len);                
        ptr += data_len;

        IINCHIP_WRITE( Sn_RX_RD0(s), (uint8)((ptr & 0xff00) >> 8));
        IINCHIP_WRITE( Sn_RX_RD1(s), (uint8)(ptr & 0x00ff));
        break;

      case Sn_MR_IPRAW :
        wiz_read_buf(addrbsb, head, 0x06);        
        ptr += 6;
        addr[0]  = head[0];
        addr[1]  = head[1];
        addr[2]  = head[2];
        addr[3]  = head[3];
        data_len = head[4];
        data_len = (data_len << 8) + head[5];

        addrbsb  = (uint32)(ptr<<8) +  (s<<5) + 0x18;
        wiz_read_buf(addrbsb, buf, data_len);        
        ptr += data_len;

        IINCHIP_WRITE( Sn_RX_RD0(s), (uint8)((ptr & 0xff00) >> 8));
        IINCHIP_WRITE( Sn_RX_RD1(s), (uint8)(ptr & 0x00ff));
        break;

      case Sn_MR_MACRAW :
        wiz_read_buf(addrbsb, head, 0x02);
        ptr+=2;
        data_len = head[0];
        data_len = (data_len<<8) + head[1] - 2;
        if(data_len > 1514)
        {
           usart3_printf("data_len over 1514\r\n");
           while(1);
        }

        addrbsb  = (uint32)(ptr<<8) +  (s<<5) + 0x18;
        wiz_read_buf(addrbsb, buf, data_len);
        ptr += data_len;

        IINCHIP_WRITE( Sn_RX_RD0(s), (uint8)((ptr & 0xff00) >> 8));
        IINCHIP_WRITE( Sn_RX_RD1(s), (uint8)(ptr & 0x00ff));
        break;

      default :
            break;
      }
      IINCHIP_WRITE( Sn_CR(s) ,Sn_CR_RECV);

      /* wait to process the command... */
      while( IINCHIP_READ( Sn_CR(s)) ) ;
      /* ------- */
   }
   return data_len;
}

#ifdef __MACRAW__
void macraw_open(void)
{
  uint8 sock_num;
  uint16 dummyPort = 0;
  uint8 mFlag = 0;
  sock_num = 0;


  close(sock_num); // Close the 0-th socket
  socket(sock_num, Sn_MR_MACRAW, dummyPort,mFlag);  // OPen the 0-th socket with MACRAW mode
}


uint16 macraw_send( const uint8 * buf, uint16 len )
{
   uint16 ret=0;
   uint8 sock_num;
   sock_num =0;


   if (len > getIINCHIP_TxMAX(sock_num)) ret = getIINCHIP_TxMAX(sock_num); // check size not to exceed MAX size.
   else ret = len;

   send_data_processing(sock_num, (uint8 *)buf, len);

   //W5500 SEND COMMAND
   IINCHIP_WRITE(Sn_CR(sock_num),Sn_CR_SEND);
   while( IINCHIP_READ(Sn_CR(sock_num)) );
   while ( (IINCHIP_READ(Sn_IR(sock_num)) & Sn_IR_SEND_OK) != Sn_IR_SEND_OK );
   IINCHIP_WRITE(Sn_IR(sock_num), Sn_IR_SEND_OK);

   return ret;
}

uint16 macraw_recv( uint8 * buf, uint16 len )
{
   uint8 sock_num;
   uint16 data_len=0;
   uint16 dummyPort = 0;
   uint16 ptr = 0;
   uint8 mFlag = 0;
   sock_num = 0;

   if ( len > 0 )
   {

      data_len = 0;

      ptr = IINCHIP_READ(Sn_RX_RD0(sock_num));
      ptr = (uint16)((ptr & 0x00ff) << 8) + IINCHIP_READ( Sn_RX_RD1(sock_num) );
      //-- read_data(s, (uint8 *)ptr, data, len); // read data
      data_len = IINCHIP_READ_RXBUF(0, ptr);
      ptr++;
      data_len = ((data_len<<8) + IINCHIP_READ_RXBUF(0, ptr)) - 2;
      ptr++;

      if(data_len > 1514)
      {
         usart3_printf("data_len over 1514\r\n");
         usart3_printf("\r\nptr: %X, data_len: %X", ptr, data_len);
         //while(1);
         /** recommand : close and open **/
         close(sock_num); // Close the 0-th socket
         socket(sock_num, Sn_MR_MACRAW, dummyPort,mFlag);  // OPen the 0-th socket with MACRAW mode
         return 0;
      }

      IINCHIP_READ_RXBUF_BURST(sock_num, ptr, data_len, (uint8*)(buf));
      ptr += data_len;

      IINCHIP_WRITE(Sn_RX_RD0(sock_num),(uint8)((ptr & 0xff00) >> 8));
      IINCHIP_WRITE(Sn_RX_RD1(sock_num),(uint8)(ptr & 0x00ff));
      IINCHIP_WRITE(Sn_CR(sock_num), Sn_CR_RECV);
      while( IINCHIP_READ(Sn_CR(sock_num)) ) ;
   }

   return data_len;
}
#endif

