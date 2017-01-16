#include "los_hwi.h"
#include "los_typedef.h"

#define UART1_INT (37)
#define UART2_INT (38)
#define UART3_INT (39)

//stm32 head Files
#include "stm32f10x_usart.h"
#include "uartConfig.h"

//Hook functions
extern void PM25HOOK(u8 data);
extern void HookOfEsp8266WiFi(u8 data);
extern void HookOfServerAction(u8 data);

extern void dprintf(u8* str);

#define DebugON 1

//for wifi
void uart1_irqHandler(void)
{
    u16 data;
        
	//receive interrupt
	if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET){
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);//clear receive Interrupt
		
		//USART_ReceiveData(USART1);
		data = (u8)(USART_ReceiveData(USART1));
        //do something
        HookOfEsp8266WiFi(data);//wifi 
        HookOfServerAction(data);//server
        //debug output
        #if DebugON
        //dprintf("uart1_int");
        #endif
	}
}

//for debug print
void uart2_irqHandler(void){
    u16 data;
        
	//receive interrupt
	if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET){
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);//clear receive Interrupt
		//receive the data
		data = (u8)(USART_ReceiveData(USART2));
        //do something
        USARTSendData(data,eUart2);
        //debug output
	}
}

//for pm25 data get
void uart3_irqHandler(void){
    u16 data;
        
	//receive interrupt
	if(USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET){
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);//clear receive Interrupt
		//receive the data
		data = (u8)(USART_ReceiveData(USART3));
        //do something
        PM25HOOK(data);
        //debug output
        #if DebugON
        //dprintByte(data);
        #endif
	}

}

void hwi_setup(){
    UINTPTR uvIntSave;
    uvIntSave = LOS_IntLock();
    LOS_HwiCreate(UART1_INT, 0,0,uart1_irqHandler,0);//create uart1 interrupt
    LOS_HwiCreate(UART2_INT,0,0,uart2_irqHandler,0);//create uart2 interrupt
    LOS_HwiCreate(UART3_INT,0,0,uart3_irqHandler,0);//create uart2 interrupt
    LOS_IntRestore(uvIntSave);
}

