#include "get_sensor_data.h"
#include "adc.h"
#include "sbus.h"
#include "ds18b20.h"
#include "comm_struct.h"
#include "ult.h"
paddy_t paddy;
plant_protect_t plant_protect;
devid_t devid;
/*传感器数值初始化*/
void get_sensor_data_init(void)
{
	/*初始化各个传感器值*/
	paddy.silt_t = 0;
	paddy.water_do = 0;
	paddy.water_l = 0;
	paddy.water_ph = 0;
	paddy.water_t = 0;
	plant_protect.bugs_n = 0;
	devid.dev_id = 1;
	/*上传数据*/
	sbus_set(DEVID_INFO_ADDR, DEVID_INFO_LENGTH, (const uint16_t*)(&devid));
	sbus_set(PADDY_INFO_ADDR, PADDY_INFO_LENGTH, (const uint16_t*)(&paddy));
	sbus_set(PLANT_PROTECT_INFO_ADDR, PLANT_PROTECT_INFO_LENGTH, (const uint16_t*)(&plant_protect));
	
	
}
/*获取水田水温*/
void get_water_temperature(void)
{
	paddy.water_t = get_temperature_after_filter();
	
}
/*获取水田水位*/
void get_water_depth(void)
{
	uint16_t l_val = adc_val(0);
	if(l_val < 750)
		l_val = 750;
	paddy.water_l = l_val*0.336 - 250;  /*实际值乘以10*/
}
/*获取水田PH*/
void get_paddy_ph(void)
{
	uint16_t ph_val = adc_val(1);
	if(ph_val < 750)
		ph_val = 750;
	paddy.water_ph = ph_val*0.47 - 350; /*实际值*100*/
}
/*获取水田溶解氧*/
void get_paddy_do(void)
{
	uint16_t do_val = adc_val(2);
	if(do_val < 750)
		do_val = 750;
	paddy.water_do = do_val*0.67 - 500;
	
}
/*获取水田泥温*/
void get_paddy_silt_t(void)
{
	paddy.silt_t = 0;
}

/*获取杀虫灯计数*/
void get_plant_protect(void)
{
	
	//plant_protect.bugs_n = adc_val(3);
}
/*获取设备id*/
void get_devid(void)
{
	devid.dev_id = 17;
}
/*获取水田传感器值*/
void get_paddy_data(void)
{
	/*获取温度值*/
	get_water_temperature();
	/*获取水田水位*/
	get_water_depth();
	/*获取水田PH*/
	get_paddy_ph();
	/*获取水田溶解氧*/
	get_paddy_do();
	/*获取水田泥温*/
	get_paddy_silt_t();
	sbus_set(PADDY_INFO_ADDR, PADDY_INFO_LENGTH, (const uint16_t*)(&paddy));	
}
