/*-----------------------------------------------------*/
/* 解析通过zigbee传过来的传感器的值和设备的状态值         */
/*-----------------------------------------------------*/
#include "usart_tack.h"
#include "comm_struct.h"
#include "sbus.h"
#include "string.h"
#include "stdio.h"
#include "usart3.h"
extern rx_data_t rcv_data;

/*将拆分后的低,8位和高8位合成一个整型*/
uint16_t chartoint(uint8_t num_h, uint8_t num_l);
void usart_tack_init(void)
{
    memset(rcv_data.rxbuf, 0, sizeof(rcv_data.rxbuf));
    rcv_data.rx_complete_flag = 0;
    rcv_data.rx_length = 0;
}
/* 协议栈 */
/*从设备地址	sensor1_val	sensor1_val	sensor1_val	sensor1_val	…	…	…	..*/
/*Slave_adress	2字节	2字节	2字节	2字节                             */				

void usart_tack_poll(void)
{
    uint8_t slave_addr = 0;
    sensor_assem1_t sensor_assem1;
    sensor_assem2_t sensor_assem2;
    static uint8_t collect_1_completc_flag = 0;
    static uint8_t collect_2_completc_flag = 0;
    //int i;
    /*判断是否完成一次帧的接收*/
    if(rcv_data.rx_complete_flag == 1)
    {
        //for(i = 0; i < rcv_data.rx_length; i++)
        //    printf(" %d ", rcv_data.rxbuf[i]);
        //printf("\r\n");
        slave_addr = rcv_data.rxbuf[0]; /*地址只能是01，02，03*/
        //printf("slave_addr = %d\r\n", slave_addr);
        switch(slave_addr)
        {
            case COLLECT_1:
                collect_1_completc_flag = 1;
                sensor_assem1.air_t1 = chartoint(rcv_data.rxbuf[1], rcv_data.rxbuf[2]);
                sensor_assem1.air_h1 = chartoint(rcv_data.rxbuf[3], rcv_data.rxbuf[4]);
                sensor_assem1.soil_t1 = chartoint(rcv_data.rxbuf[5], rcv_data.rxbuf[6]);
                sensor_assem1.soil_h1 = chartoint(rcv_data.rxbuf[7], rcv_data.rxbuf[8]);
                sensor_assem1.soil_ph1 = chartoint(rcv_data.rxbuf[9], rcv_data.rxbuf[10]);
                sensor_assem1.air_l = chartoint(rcv_data.rxbuf[11], rcv_data.rxbuf[12]); 
                sensor_assem1.air_co2_1 = chartoint(rcv_data.rxbuf[13], rcv_data.rxbuf[14]);            
                break;
            case COLLECT_2:
                collect_2_completc_flag = 1;
                sensor_assem2.air_t2 = chartoint(rcv_data.rxbuf[1], rcv_data.rxbuf[2]);
                sensor_assem2.air_h2 = chartoint(rcv_data.rxbuf[3], rcv_data.rxbuf[4]);
                sensor_assem2.soil_t2 = chartoint(rcv_data.rxbuf[5], rcv_data.rxbuf[6]);
                sensor_assem2.soil_h2 = chartoint(rcv_data.rxbuf[7], rcv_data.rxbuf[8]);
                sensor_assem2.soil_ph2 = chartoint(rcv_data.rxbuf[9], rcv_data.rxbuf[10]);
                //sensor_assem2.bugs_n = chartoint(rcv_data.rxbuf[11], rcv_data.rxbuf[12]);
                //sensor_assem2.air_co2_2 = sensor_assem1.air_co2_1;
                break;
            case CONTROL_0:
                /*解析控制状态指令*/
                /*暂时不添加*/
                break;
            default:
                break;
                            
        }
        if(collect_1_completc_flag == 1)
        {
            
            sbus_set(SENSOR_ASSEM1_INFO_ADDR, SENSOR_ASSEM1_INFO_LENGTH, (const uint16_t *)(&sensor_assem1));
            collect_1_completc_flag = 0;
        }
        if(collect_2_completc_flag == 1) 
        {
            sbus_set(SENSOR_ASSEM2_INFO_ADDR, SENSOR_ASSEM2_INFO_LENGTH, (const uint16_t *)(&sensor_assem2));
            collect_2_completc_flag = 0;

        }            
        
        
    }
    rcv_data.rx_complete_flag = 0;
    //rcv_data.rx_length = 0;
    /*传输到总线上*/
    
    //sbus_set(PADDY_SENSOR_INFO_ADDR, PADDY_SENSOR_INFO_LENGTH, (const uint16_t *)&paddy_sensor);
    //sbus_set(CANAL_SENSOR_INFO_ADDR, CANAL_SENSOR_INFO_LENGTH, (const uint16_t *)&canal_sensor);
}
uint16_t chartoint(uint8_t num_h, uint8_t num_l)
{
    return ((uint16_t)num_h << 8) + num_l;
}
