/**
 * @file parallel.h
 * @author 戸田煉 24-326
 * @brief windowsでのパラレルポート制御
 */

#ifndef PARALLEL_H
#define PARALLEL_H

#include <windows.h>

#define PARALLEL_PORT_BASE 0x3fd8
#define PP_DATA            (PARALLEL_PORT_BASE + 0)
#define PP_STATUS          (PARALLEL_PORT_BASE + 1)
#define PP_CONTROL         (PARALLEL_PORT_BASE + 2)

static HINSTANCE pp_dll = NULL;

/**
 * @brief 指定ポートに値を出力
 * @param port ポートアドレス
 * @param value 出力値
 */
static void(__stdcall *pp_out)(short, short) = NULL;
/**
 * @brief 指定ポートから値を読み込み
 * @param port ポートアドレス
 * @return 読み込んだ値
 */
static short(__stdcall *pp_in)(short) = NULL;

/**
 * @brief パラレルポートを初期化
 * @return 成功時0、失敗時-1
 */
static inline int pp_init(void) {
    pp_dll = LoadLibrary("inpout32.dll");
    if(pp_dll == NULL) return -1;
    pp_out = (void(__stdcall *)(short, short))GetProcAddress(pp_dll, "Out32");
    pp_in = (short(__stdcall *)(short))GetProcAddress(pp_dll, "Inp32");
    if(pp_out == NULL || pp_in == NULL) return -1;
    return 0;
}

/**
 * @brief パラレルポートリソースを解放
 */
static inline void pp_close(void) {
    FreeLibrary(pp_dll);
}

#endif  // PARALLEL_H