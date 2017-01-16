#include "pm25.h"
#include "dprintf.h"

static u8 pm25data[7];
static u8 enabled = 0;//indicate if the data is ok
#define START (1)
#define END (0)
#define A_Ratio (800.0)

//pm25 init is uart_init
void PM25_init(void){
    //init
}

//if return 1,get right value
u8 getPM25Index(float* result){
	u16 vout = 0;
    //check the data is OK
    if(0 == enabled){
        return 0;
    }

	//verify the data is good
	u8 checkCode = 0;
	checkCode = pm25data[1]+pm25data[2]+pm25data[3]+pm25data[4];
	if(checkCode != pm25data[5])
		return 0;
	//get vout
	vout |= pm25data[1];
	vout <<= 8;
	vout |= pm25data[2];
	//cal the pm2.5
	*result = vout*1.0*A_Ratio/1024.0*5;

    //disable the data
    enabled = 0;
    return 1;
}

void PM25HOOK(u8 data){
    static u8 mState = END,cnt=0;
	//start byte
    if(0xaa == data){
		mState = START;
		cnt = 0;
	}
	//end byte
	else if(0xff == data && 6 == cnt){
		mState = END;//change the state
		enabled = 1;
        //dprintfln("getpm25");
	}
	//get the byte
	if(START == mState){
		pm25data[cnt++] = data;
	}
	
}


