/**
 * @file experiment2.c
 * @author 戸田煉 24-326
 * @brief パラレルポートのD0～D7全てを0/1で周期する
 */

#include <stdio.h>
#include <stdbool.h>
#include "parallelport.h"

/**
 * @brief メイン関数
 */
int main(void) {
    if(pp_init() != 0) return -1;
    pp_out(PP_CONTROL, 0x00);  // 出力modeに設定
    short leds = 0x01;
    short input;
    bool button;
    short ACK = 0x01 << 6;

    while(1) {
        input = pp_in(PP_STATUS);
        button = (input & ACK) != ACK;
        printf("input:%x ,ACK:%x, button:%d\n", input, ACK, button);

        if(button) {
            if(leds == 0x01 << 7) leds = 0x01;

            pp_out(PP_DATA, leds);
            printf("lighting %d\n", leds);
            leds = leds << 1;
            Sleep(1000);
        } else {
            pp_out(PP_DATA, 0x00);
            Sleep(1000);
            pp_out(PP_DATA, 0xff);
            Sleep(1000);
        }
    }
    pp_close();
    return 0;
}
