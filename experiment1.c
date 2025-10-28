/**
 * @file experiment1.c
 * @author 戸田煉 24-326
 * @brief パラレルポートのD0～D7全てに1 (High)を出力するプログラム
 */

#include <stdio.h>
#include <stdbool.h>
#include <windows.h>  //< パラレルポートを操作するため

#define PARALLEL_PORT_BASE 0x3fd8
#define PP_DATA            (PARALLEL_PORT_BASE + 0)
#define PP_STATUS          (PARALLEL_PORT_BASE + 1)
#define PP_CONTROL         (PARALLEL_PORT_BASE + 2)

HINSTANCE pp_dll;

// プロトタイプ宣言
void(__stdcall *pp_out)(short, short) = NULL;
short(__stdcall *pp_in)(short) = NULL;
int pp_init(void);
void pp_close(void);

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

/**
 * @brief パラレルポート初期化関数
 */
int pp_init(void) {
    pp_dll = LoadLibrary("inpout32.dll");
    pp_out = (void(__stdcall *)(short, short))GetProcAddress(pp_dll, "Out32");
    pp_in = (short(__stdcall *)(short))GetProcAddress(pp_dll, "Inp32");
    return 0;
}

void pp_close(void) {
    FreeLibrary(pp_dll);
}