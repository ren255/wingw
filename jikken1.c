// jikken1.c　パラレルポートのD0～D7全てに1 (High)を出力するプログラム
// 学籍番号：xx-3xx    氏名：制御太郎
#include <stdio.h>
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
    
    pp_write_data(0xff);  // データレジスタの8 bitすべてに'1'を出力
    pp_close();
    return 0;
}