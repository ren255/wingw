// jikken1.c　パラレルポートのD0～D7全てに1 (High)を出力するプログラム
// 学籍番号：xx-3xx    氏名：制御太郎
#include <stdio.h>
#include <stdbool.h>
#include "parallelport.h"

/**
 * @brief 
 * 
 * @return int 
 */
int main(void)
{
    if (pp_init() != 0) return 1;
    pp_out(PP_CONTROL, 0x00); // Set to output mode
	
	short leds = 0x01;
	short input;
	bool button;
	short ACK = 0x01 << 6;

	while(1){
		
        input = pp_in(PP_STATUS);
		button = (input & ACK ) != ACK;
		printf("input:%x ,ACK:%x, button:%d\n",input,ACK,button);

		if(button){
			if(leds == 0x01 << 7) leds = 0x01;

	    	pp_out(PP_DATA,leds);
			printf("lighting %d\n",leds);
			leds = leds << 1;
			Sleep(1000);
		} else{
		  	pp_out(PP_DATA,0x00);
			Sleep(1000);
			pp_out(PP_DATA,0xff);
			 Sleep(1000);
        }	
		
	}
    pp_close();
    return 0;
}
