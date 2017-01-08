/*-----------------------------------------------------------------------*/
/* 将从平台得到的数据解析完之后传输给从机包括采集的上下限                    */
/* 从机01 传感设备1 从机02 传感设备2  从机设备3 控制设备3 从机设备4 控制设备4*/
/* ----------------------------------------------------------------------*/
/* 格式如下：                                                             */
/* 从机地址 头帧  长度 指令   指令    指令     指令    指令   CRC-8         */
/*  00 01  55 AA      00 01  00 02  00 03     00 04  00 05  HEX          */
#include "msg_lower_set.h"
#include "sbus.h"
#include "comm_struct.h"
#include "stdint.h"
#include "usart.h"

#define set_msg_lower(c)    USART1_Putc((c))
/*传输 传感器设备的上下限*/
void msg_set_sensor_limit_info(void)
{
        sensor_limit_t sensor_limit;   /*环境传感器上下限*/
        static uint8_t roll_flag = 0;  /*循环发送传感器限值*/
        uint16_t sensor_limit_val_region[12];
        /*从当前总线中获取内容*/
        sbus_get(SENSOR_LIMIT_INFO_ADDR, SENSOR_LIMIT_INFO_LENGTH, (uint16_t *)(&sensor_limit));
        /*空气温度 上下限*/
        sensor_limit_val_region[0] = sensor_limit.air_t_h;
        sensor_limit_val_region[1] = sensor_limit.air_t_l;
        /*空气湿度 上下限*/
        sensor_limit_val_region[2] = sensor_limit.air_h_h;
        sensor_limit_val_region[3] = sensor_limit.air_h_l;
        /*土壤温度 上下限*/
        sensor_limit_val_region[4] = sensor_limit.soil_t_h;
        sensor_limit_val_region[5] = sensor_limit.soil_t_l;
        /*土壤湿度 上下限*/
        sensor_limit_val_region[6] = sensor_limit.soil_h_h;
        sensor_limit_val_region[7] = sensor_limit.soil_h_l;
        /*二氧化碳    上下限*/
        sensor_limit_val_region[8] = sensor_limit.air_co2_h;
        sensor_limit_val_region[9] = sensor_limit.air_co2_l;
        /*光照 上下限*/
        sensor_limit_val_region[10] = sensor_limit.air_l_h;
        sensor_limit_val_region[11] = sensor_limit.air_l_l;
        
        /*将数据发送到下面----->控制设备*/
        set_msg_lower(0x00);
        set_msg_lower(0x03);   /*设备地址，针对于zigbee主从模式*/
        set_msg_lower(0x55);
        set_msg_lower(0xAA);
        set_msg_lower(0x01);   /*用 0x01 表示传输的上下限*/
        set_msg_lower(0x05);
        /*未加入校验*/
        /*通过轮询，减少每次发送的长度，分6次传完*/
        switch(roll_flag)
        {
            case 0:
                /*空气温度上下限*/
                set_msg_lower(0x01);
                set_msg_lower((int8_t)(sensor_limit_val_region[0] >> 8));
                set_msg_lower((int8_t)(sensor_limit_val_region[0] & 0x00ff));
                set_msg_lower((uint8_t)(sensor_limit_val_region[1] >> 8));
                set_msg_lower((uint8_t)(sensor_limit_val_region[1] & 0x00ff));
                
                roll_flag = 1;
                break;
            case 1:
                /*空气湿度上下限*/
                set_msg_lower(0x02);
                set_msg_lower((uint8_t)(sensor_limit_val_region[2] >> 8));
                set_msg_lower((uint8_t)(sensor_limit_val_region[2] & 0x00ff));
                set_msg_lower((uint8_t)(sensor_limit_val_region[3] >> 8));
                set_msg_lower((uint8_t)(sensor_limit_val_region[3] & 0x00ff));
            
                roll_flag = 2;
                break;
            case 2:
                /*土壤温度上下限*/
                set_msg_lower(0x03);
                set_msg_lower((int8_t)(sensor_limit_val_region[4] >> 8));
                set_msg_lower((int8_t)(sensor_limit_val_region[4] & 0x00ff));
                set_msg_lower((uint8_t)(sensor_limit_val_region[5] >> 8));
                set_msg_lower((uint8_t)(sensor_limit_val_region[5] & 0x00ff));
                roll_flag = 3;
                break;
            case 3:
                /*土壤湿度上下限*/
                set_msg_lower(0x04);
                set_msg_lower((uint8_t)(sensor_limit_val_region[6] >> 8));
                set_msg_lower((uint8_t)(sensor_limit_val_region[6] & 0x00ff));
                set_msg_lower((uint8_t)(sensor_limit_val_region[7] >> 8));
                set_msg_lower((uint8_t)(sensor_limit_val_region[7] & 0x00ff));
                roll_flag = 4;
                break;
            case 4:
                /*二氧化碳浓度上下限*/
                set_msg_lower(0x05);
                set_msg_lower((uint8_t)(sensor_limit_val_region[8] >> 8));
                set_msg_lower((uint8_t)(sensor_limit_val_region[8] & 0x00ff));
                set_msg_lower((uint8_t)(sensor_limit_val_region[9] >> 8));
                set_msg_lower((uint8_t)(sensor_limit_val_region[9] & 0x00ff));
                roll_flag = 5;
                break;
            case 5:
                /*光照上下限*/
                set_msg_lower(0x06);
                set_msg_lower((uint8_t)(sensor_limit_val_region[10] >> 8));
                set_msg_lower((uint8_t)(sensor_limit_val_region[10] & 0x00ff));
                set_msg_lower((uint8_t)(sensor_limit_val_region[11] >> 8));
                set_msg_lower((uint8_t)(sensor_limit_val_region[11] & 0x00ff));
                roll_flag = 0;
                break;
            default:
                roll_flag = 0;
                break;
            
        }
   
}
/*传输控制指令*/
void msg_set_ctr_info(void)
{
        ctr_cmd_t ctr_cmd;
        uint16_t ctr_val_region[4];
        sbus_get(CTR_CMD_INFO_ADDR, CTR_CMD_INFO_LENGTH, (uint16_t *)(&ctr_cmd));
        
        /*灌溉 卷膜 卷帘 补光灯*/
        ctr_val_region[0] = (uint8_t)(ctr_cmd.water_ctr & 0x00ff);
        ctr_val_region[1] = (uint8_t)(ctr_cmd.roll_ctr & 0x00ff);
        ctr_val_region[2] = (uint8_t)(ctr_cmd.shutter_ctr & 0x00ff);
        ctr_val_region[3] = (uint8_t)(ctr_cmd.light_ctr & 0x00ff);
        /*发送指令----->控制设备*/
        set_msg_lower(0x00);
        set_msg_lower(0x03);   /*设备地址，针对于zigbee主从模式*/
        set_msg_lower(0x55);
        set_msg_lower(0xAA);
        //set_msg_lower(0x02);   /*用0x02表示传输控制指令*/
        set_msg_lower(0x04);
        set_msg_lower(ctr_val_region[0]);  /*灌溉*/
        set_msg_lower(ctr_val_region[1]);  /*卷膜*/
        set_msg_lower(ctr_val_region[2]);  /*卷帘*/
        set_msg_lower(ctr_val_region[3]);  /*补光灯*/   
        /*未加入校验*/
}
