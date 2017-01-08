#include "gsm.h"
#include "bsp_usart.h"
#include "bsp_lcd.h"

 char AT_AT[] = {"AT\r\n"};
const char AT_CBC[] ={"AT+CBC\r\n"};  //??????,?????????????+CBC: 0,97,4164
const char AT_CREG[] = {"AT+CREG?\r\n"}; //????????
const char AT_CREGINIT[] = {"AT+CREG=1\r\n"}; //?????????
const char AT_ATE[] = {"ATE0\r\n"};  //?????????
const char AT_Save[] = {"AT&W\r\n"};  //???????

/* 0 disable slow clock   1 enable slow clock */
const char AT_CSCLK[] = {"AT+CSCLK=1\r\n"}; //enable slow clock

/* 0 minimum functionality 1  full functionality (Default) 4 disable phone both transmit and receive RF circuits */
const char AT_CFUN[] = {"AT+CFUN=1\r\n"}; //?????????????????
const char AT_CGMR[] = {"AT+CGMR\r\n"}; //??????
const char AT_CCID[] = {"AT+CCID\r\n"}; //??SIM???
const char AT_CGSN[]={"AT+CGSN\r\n"}; //IMEI
const char AT_CSQ[]={"AT+CSQ\r\n"}; //????
const char AT_CSMINS[]={"AT+CSMINS?\r\n"}; //??SIM?????+CSMINS: 0,1
const char AT_COPS[]={"AT+COPS?\r\n"}; 
const char AT_CPOL[]={"AT+CPOL?\r\n"}; 
const char AT_REG_COPS[]={"AT+CPOL=1,2,\""};

//------------------------------------------------------------------
const char Gprs_class[] = {"AT+CGCLASS=\"B\"\r\n"}; //length = 16B ??GPRS???????  
const char Gprs_apn[] = {"AT+CSTT=\""};    //GPRS?????
const char Gprs_CSTT[]={"AT+CIPCSGP=1,\""}; //??GPRS????????
const char Gprs_cgatt[] = {"AT+CGATT=1\r\n"}; //length = 12B??GPRS??????
const char Gprs_cgreg[] = {"AT+CGREG?\r\n"}; //??GPRS??????
const char Gprs_cgreginit[] = {"AT+CGREG=1\r\n"}; //???GPRS??????
const char Gprs_T_S[] = {"AT+CIPSTART=\"TCP\",\""}; //??TCP??;??????CONNECT OK
char Gprs_T_S_ALL[] = {"AT+CIPSTART=\"TCP\",\"220.196.42.155\",8086\r\n"}; //??TCP??;??????CONNECT OK
const char Gprs_U_S[] = {"AT+CIPSTART=\"UDP\",\""}; //??UDP??
 char Gprs_ipshut[] = {"AT+CIPSHUT\r\n"}; //  //?GPRS?PDP??
const char Gprs_send[] = {"AT+CIPSEND\r\n"}; //????;
const char Gprs_dpdp[] = {"AT+CIPDPDP=1,60,3\r\n"}; //????GPRS???????;60??,??3?
 char Gprs_close[] = {"AT+CIPCLOSE\r\n"}; //????GPRS???????;60??,??3?

const char SetBand[]= "AT+IPR=115200\r\n";    //??????????????9600

/*
GPRS Moderm??TCP???AT????????:
   AT\r
   AT+ccid\r //??????SIM ?
   AT+cgmr\r //??????,5.0 ?????GPRS ????
   AT+csq\r //??????,???????????;???10--31,0??????????,
            ?????99,99,??????????????????
   AT+CGCLASS="B"\r  //????????
   AT+CGDCONT=1,"IP","CMNET"\r 
   AT+CGATT=1  //??PDP,??IP
   AT+CIPCSGP=1,"CMNET"\r
   AT+CIPSTART="TCP",211.147.18.79,1433\r
   AT+CIPSEND\r
   >   //??“>”?????????,???CTRL+Z,??????????????IP?????
   AT+CIPCLOSE\r  //???TCP/UDP??CONNECT OK???????OK,????ERROR
   AT+CIPSHUT\r   //??????
*/
void SendByteByCom(UINT8 ch)
{
		Usart_SendByte(USART1, ch);
}

void SendByCom(char* data)
{
		Usart_SendString(USART1,(char *)data);
}

void GSM_Init(void)
{
	SendByCom(AT_AT);
	//SendByCom(AT_CCID);
	//SendByCom(AT_CGMR);
	//SendByCom(AT_CSQ);
	//SendByCom(Gprs_class);
	//printf(??);
	//SendByCom(Gprs_cgatt);
	//SendByCom(Gprs_T_S);
	
}
void SendByte(UINT8 ch)
{
	  char buffer[4]; 
		memset(buffer,0,sizeof(buffer));
		SendByteByCom(ch);
		sprintf(buffer,"%d",ch);
		ILI9341_DispString_EN( 30, 180, buffer, macWHITE, macBLUE );	
}

void Send(const char* data)
{
	UINT8 len;
	char buffer[64]; 
	memset(buffer,0,sizeof(buffer));
	sprintf(buffer,"\r\n%s%s",Gprs_send,data);
	len = strlen(buffer);
	buffer[len] = 0x1A;
	buffer[len + 1] = 0;	
	SendByCom(buffer);
	
	ILI9341_DrawRectangle(30,180,210,60,macWHITE,1);
	ILI9341_DispString_EN(30,180,buffer, macWHITE,macBLUE);	
	
}

void Receive(void)
{
	UINT8 i = 0;
	char ch = 0;
	char buffer[64];
	memset(buffer,0,sizeof(buffer));
	while( (ch = fgetc(NULL)) != NULL )
	{
		buffer[i] = ch;	
		i++;
		if( i >  sizeof(buffer)) 
			i = 0;
	}
	ILI9341_DrawRectangle(0,80,240,60,macWHITE,1);
	ILI9341_DispString_EN ( 30, 80, buffer, macWHITE, macBLUE );	
}

//Connect  tcp/ip
void Connect(void)
{
		ILI9341_DispString_EN ( 10, 280, "Server: 220.196.42.155", macWHITE, macBLACK );
		ILI9341_DispString_EN ( 10, 300, " Port : 8086", macWHITE, macBLACK );
		SendByCom(Gprs_T_S_ALL);
}

void Disconnect(void)
{
		SendByCom(Gprs_close);
		SendByCom(Gprs_ipshut);
}

