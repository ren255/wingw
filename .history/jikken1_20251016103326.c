// jikken1.c　パラレルポートのD0～D7全てに1 (High)を出力するプログラム
// 学籍番号：xx-3xx    氏名：制御太郎
#include <stdio.h>   // 標準入出力用ヘッダファイル
#include <windows.h> // パラレルポートを使うためのヘッダファイル
typedef  void  (__stdcall *p_out)(short, short);
typedef  short  (__stdcall *p_inp)(short);

int main(void)
{
    HINSTANCE hd = LoadLibrary("inpout32.dll");  // パラレルポートの使用準備
    p_out pout = (p_out)GetProcAddress(hd, "Out32");
    p_inp pinp = (p_inp)GetProcAddress(hd, "Inp32");
    pout( 0x3fda, 0x00 );  // 0x3fda番地にあるコントロールレジスタを出力モードに設定
    pout( 0x3fd8, 0xff );  // 0x3fd8番地にあるデータレジスタの8 bitすべてに’1’を出力

    FreeLibrary( hd );     // パラレルポートへのアクセスを終了
    return 0;
}
