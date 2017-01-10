
#include "stm32f10x.h"
#include "stm32f10x_i2c.h"
void Iic1_Init(void)
 {
         GPIO_InitTypeDef GPIO_InitStructure;
         I2C_InitTypeDef I2C_InitStructure;
         
         RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
         RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

         //PB6-I2C1_SCL PB7-I2C1_SDA PB10-I2C2_SCL PB11-I2C2_SDA
         /* Configure IO connected to IIC*********************/
         GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
         GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
         GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
         GPIO_Init(GPIOB, &GPIO_InitStructure);
         
         I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
         I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
         I2C_InitStructure.I2C_OwnAddress1 = 0x30;//I2C2_SLAVE_ADDRESS7;
         I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
         I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
         I2C_InitStructure.I2C_ClockSpeed = 100000;
         I2C_Cmd(I2C1, ENABLE);   
         
         I2C_Init(I2C1, &I2C_InitStructure);
         /*??1??1????*/
         I2C_AcknowledgeConfig(I2C1, ENABLE);    
 }


 /**********************************************************************/
 /*IIC?????                                                                               */
 /*                                                                                                                                          */
 /**********************************************************************/
void I2C1_WriteByte(unsigned char id,unsigned char write_address,unsigned char byte)
 {
         while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));

         I2C_GenerateSTART(I2C1,ENABLE);
         //??????
        while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
         //??ACK
          
         I2C_Send7bitAddress(I2C1,id,I2C_Direction_Transmitter);
         //?????????
        while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
         //??ACK

         I2C_SendData(I2C1, write_address);
         //?????
        while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

         //??ACK
         I2C_SendData(I2C1, byte);
         //????
        while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

         //????
        I2C_GenerateSTOP(I2C1, ENABLE);
         //??????

        do
         {                
                 /* Send START condition */
                 I2C_GenerateSTART(I2C1, ENABLE);
                 /* Read I2C1 SR1 register */
                 /* Send EEPROM address for write */
                 I2C_Send7bitAddress(I2C1, 0xA2, I2C_Direction_Transmitter);
         }while(!(I2C_ReadRegister(I2C1, I2C_Register_SR1) & 0x0002));

         /* Clear AF flag */
         I2C_ClearFlag(I2C1, I2C_FLAG_AF);
         /* STOP condition */    
         I2C_GenerateSTOP(I2C1, ENABLE);  
 }

 /**********************************************************************/
 /*IIC???                                                                                         */
 /*                                                                                                                                          */
 /**********************************************************************/
unsigned char I2C1_ReadByte(unsigned char  id, unsigned char read_address)
 {  
         unsigned char temp;         

         while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
           //??I2C

           I2C_GenerateSTART(I2C1, ENABLE);
           //??????
          while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
     
         //EV5
           I2C_Send7bitAddress(I2C1, id, I2C_Direction_Transmitter);
         //????
          while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

           //EV6
           I2C_Cmd(I2C1, ENABLE);
         //????????EV6
           I2C_SendData(I2C1, read_address);  
         //??????
          while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
           
         //EV8 
           I2C_GenerateSTART(I2C1, ENABLE);
         //????
          while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

           //EV5
           I2C_Send7bitAddress(I2C1, id, I2C_Direction_Receiver);
           //?????
          while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

           //EV6  
     I2C_AcknowledgeConfig(I2C1, DISABLE);
     I2C_GenerateSTOP(I2C1, ENABLE);
         //???????????
    while(!(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)));
               
     temp = I2C_ReceiveData(I2C1);
    
           I2C_AcknowledgeConfig(I2C1, ENABLE);

         return temp;
 }
