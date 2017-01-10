#include "sht10.h"
#include "stm32f10x.h"
#include "stdio.h"
#include "ult.h"
#include "math.h"
#include "xassert.h"
#include "usart.h"
#define SHT10_Delay()   delay_ms(1)   //定义时钟延时
/**
   * @brief  初始化SHT10 IO口
  * @param  None
   * @retval None
*/
void SHT10_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
     
     //使能GPIOB时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB ,ENABLE);
   
     // SDA 推挽输出
    GPIO_InitStructure.GPIO_Pin = SHT10_SDA;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    // SCK 推挽输出
    GPIO_InitStructure.GPIO_Pin = SHT10_SCK;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**
   * @brief  配置为输入状态
  * @param  None
   * @retval None
*/
 void SHT10_SDAIn(void)
 {
     GPIO_InitTypeDef GPIO_InitStructure;
     
     //PB0 SDA 浮动输入，外部有上拉电阻
     GPIO_InitStructure.GPIO_Pin = SHT10_SDA;
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
     GPIO_Init(GPIOB, &GPIO_InitStructure);   
 }

 /**
   * @brief  配置为输出状态
  * @param  None
   * @retval None
   */
 void SHT10_SDAOut(void)
 {
     GPIO_InitTypeDef GPIO_InitStructure;
     
     //PB0 SDA 推挽输出
     GPIO_InitStructure.GPIO_Pin = SHT10_SDA;
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
     GPIO_Init(GPIOB, &GPIO_InitStructure);
 }

 /**
   * @brief  配置为输出状态
  * @param  写数据
  * @retval 应答
  */
uint8_t SHT10_WriteByte(uint8_t value)
 { 
     uint8_t i,error=0;  
     //SDA输出
     SHT10_SDAOut();
     
     for( i = 0x80 ; i>0 ; i/=2)             
     { 
         if ( i & value) 
             SDA_H();            
         else 
             SDA_L();
         
         SHT10_Delay();                       
         SCK_H();                          
         SHT10_Delay();                       
         SCK_L();
         SHT10_Delay();                     
     }
     //SDA输入
     SHT10_SDAIn();
     
     SCK_H();                            
     error = SDA_R();   //读应答位                 
     SCK_L(); 
     return error;      //error=1 in case of no acknowledge        
 }

 /**
   * @brief  读数据
  * @param  应答
  * @retval 返回数据
  */
uint8_t SHT10_ReadByte(uint8_t Ack)
 { 
     uint8_t i,val=0;
      //输入状态
     SHT10_SDAIn();  
     for (i=0x80;i>0;i/=2)         
     { 
         SHT10_Delay();  
         SCK_H();   
         SHT10_Delay();  
         if (SDA_R()) 
             val=(val | i);        //读数据
        SCK_L();                                           
     }
     //输出状态
     SHT10_SDAOut();  
     if(Ack)
         SDA_L();                //应答为低电平
     else
         SDA_H();

     SHT10_Delay();  
     SCK_H();                     
     SHT10_Delay();  
     SCK_L();
     SHT10_Delay();                                              
     return val;
 }

 /**
   * @brief  启动
  * @param  无
  * @retval 无
  */
void SHT10_Start(void)
 {  
     //SDA输出
     SHT10_SDAOut();
     SCK_L();                   
     SHT10_Delay();          
     SCK_H();
     SHT10_Delay();          
     SDA_L();
     SHT10_Delay();          
     SCK_L();  
     SHT10_Delay();          
     SCK_H();
     SHT10_Delay();          
     SDA_H();                   
     SHT10_Delay();          
     SCK_L();                   
 }

 /**
   * @brief  重新连接
  * @param  无
  * @retval 无
  */
void SHT10_ConReset(void)
 {
     uint8_t i;
     //输出
     SHT10_SDAOut();
     SDA_H();    //输出高电平
     SCK_L();
     for(i = 0 ; i < 9 ; i++)                  
     { 
         SCK_H();
         SHT10_Delay();
         SCK_L();
         SHT10_Delay();
     }
     SHT10_Start();                   
 }

 /**
   * @brief  软件重启
  * @param  无
  * @retval 无
  */
uint8_t SHT10_SoftReset(void)
 { 
     uint8_t error=0;  
     SHT10_ConReset();              
     error += SHT10_WriteByte(S_RESET);  
     delay_ms(15);  //Wait minimum 11 ms before next command
     return error;        //error=1 in case of no response form the sensor
            
 }

 /**
   * @brief  温度或湿度测量
  * @param  温度或者湿度指针数据，校验值指针，模式
  * @retval 错误
  */
uint8_t SHT10_Measure(uint16_t* pValue, uint8_t* pCheckSum, uint8_t mode)
 { 
     uint8_t error=0;
     
     uint8_t Value_H = 0;
     uint8_t Value_L = 0;
     
     //启动
     SHT10_Start();                
     switch(mode)
     {                     
        case TEMP: 
            error += SHT10_WriteByte(MEASURE_TEMP); 
         break;
        case HUMI: 
            error += SHT10_WriteByte(MEASURE_HUMI); 
         break;
        default: 
         break;         
     }
     
     //SDA读状态
     SHT10_SDAIn();
     //等待转换完成，代码有待优化
     while(SDA_R())
     {
         ;
     }

     Value_H = SHT10_ReadByte(ACK);    //读高位
     Value_L = SHT10_ReadByte(ACK);    //读低位
   
     *pCheckSum = SHT10_ReadByte(noACK);  //读校验结果
    
     //返回结果
    *pValue = (Value_H << 8) | Value_L;    
     
     return error;
 }


 /**
   * @brief  计算温度和湿度数据
  * @param  温度数据 湿度数据 温度结果 湿度结果
  * @retval 无
  */
void SHT10_Cal(uint16_t Temp,uint16_t Hum, float* pTempValue,float* pHumValue)
 { 
     const float d1 = -40.1;
     const float d2 = 0.01;
     float Temp_C;
     const float C1 = -2.0468;           
     const float C2 = +0.0367;           
     const float C3 = -0.0000015955;     
     const float T1 = +0.01;             
     const float T2 = +0.00008;   
     //湿度线性值
     float RH_Lin;
     //湿度真实值
     float RH_True;  
     
     //温度结果，换算                 
     Temp_C = d1 + d2 * Temp;   

     //RH线性结果
     RH_Lin = C1 + C2 * Hum + C3 * Hum *Hum;
     RH_True = (Temp_C - 25) * (T1 + T2 * Hum) + RH_Lin;
     //限定范围
     if( RH_True > 100 ) 
         RH_True = 100; 
     if( RH_True < 0.01) 
         RH_True = 0.01;
     
     *pTempValue = Temp_C;
     *pHumValue = RH_True; 
 }

 //////////////////////////////////////////////////////////
 //----------------------------------------------------------------------------------
char s_measure(unsigned char *p_value, unsigned char *p_checksum, unsigned char mode)
//----------------------------------------------------------------------------------
// makes a measurement (humidity/temperature) with checksum
{ 
      unsigned char error=0;
      unsigned int i;

       //启动
      SHT10_Start();                   //transmission start
      switch(mode){                     //send command to sensor
        case TEMP	: error += SHT10_WriteByte(MEASURE_TEMP); break;
        case HUMI	: error += SHT10_WriteByte(MEASURE_HUMI); break;
        default     : break;	 
      }
       //SDA读状态
      SHT10_SDAIn();
      for (i=0;i<65535;i++) if(SDA_R()==0) break; //wait until sensor has finished the measurement
      if(SDA_R()) error+=1;                // or timeout (~2 sec.) is reached
      *(p_value)  =SHT10_WriteByte(ACK);    //read the first byte (MSB)
      *(p_value+1)=SHT10_WriteByte(ACK);    //read the second byte (LSB)
      *p_checksum =SHT10_WriteByte(noACK);  //read checksum
      return error;
}
void calc_sth11(float *p_humidity ,float *p_temperature)
//----------------------------------------------------------------------------------------
// calculates temperature [℃] and humidity [%RH] 
// input :  humi [Ticks] (12 bit) 
//          temp [Ticks] (14 bit)
// output:  humi [%RH]
//          temp [℃]
// 供电电压5V
{ 
      const float C1=-2.0468;           // for 12 Bit RH
      const float C2=+0.0367;           // for 12 Bit RH
      const float C3=-0.0000015955;     // for 12 Bit RH
      const float T1=+0.01;             // for 12 Bit RH
      const float T2=+0.00008;          // for 12 Bit RH	

      float rh=*p_humidity;             // rh:      Humidity [Ticks] 12 Bit 
      float t=*p_temperature;           // t:       Temperature [Ticks] 14 Bit
      float rh_lin;                     // rh_lin:  Humidity linear
      float rh_true;                    // rh_true: Temperature compensated humidity
      float t_C;                        // t_C   :  Temperature [℃]

      t_C=t*0.01 - 40.1;                //calc. temperature [℃] from 14 bit temp. ticks @ 5V
      rh_lin=C3*rh*rh + C2*rh + C1;     //calc. humidity from ticks to [%RH]
      rh_true=(t_C-25)*(T1+T2*rh)+rh_lin;   //calc. temperature compensated humidity [%RH]
      if(rh_true>100)rh_true=100;       //cut if the value is outside of
      if(rh_true<0.1)rh_true=0.1;       //the physical possible range

      *p_temperature=t_C;               //return temperature [℃]
      *p_humidity=rh_true;              //return humidity[%RH]
}

//--------------------------------------------------------------------
float calc_dewpoint(float h,float t)
//--------------------------------------------------------------------
// calculates dew point
// input:   humidity [%RH], temperature [℃]
// output:  dew point [℃]
{ 
      float k,dew_point ;
      
      k = (log10(h)-2)/0.4343 + (17.62*t)/(243.12+t);
      dew_point = 243.12*k/(17.62-k);
      return dew_point;
}

//sht10初始化
void sht10_init(void)
{
    uint8_t error = 0;
    SHT10_Config();
    delay_ms(200);
    error = SHT10_SoftReset(); //error=1 in case of no response form the sensor
    xassert(error == 0);   //如果错误等于1，那么说明设备没有在线，或者设备故障
}
