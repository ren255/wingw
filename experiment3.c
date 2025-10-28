/**
 * @file experiment3.c
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
    while(true) {
        pp_out(PP_DATA, 0xff);  // 8bit全て1に設定
        Sleep(1000);
        pp_out(PP_DATA, 0x00);  // 8bit全て0に設定
        Sleep(1000);
    }
    pp_close();
    return 0;
}
