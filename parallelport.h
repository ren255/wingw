// parallelport.h - Header-only parallel port wrapper
// Usage: #include "parallelport.h" at the top of your program
#ifndef PARALLELPORT_H
#define PARALLELPORT_H

// ============ User Configuration ============
#ifndef PARALLEL_PORT_BASE
#define PARALLEL_PORT_BASE 0x3fd8
#endif

// ============ Register Addresses ============
#define PP_DATA    (PARALLEL_PORT_BASE + 0)
#define PP_STATUS  (PARALLEL_PORT_BASE + 1)
#define PP_CONTROL (PARALLEL_PORT_BASE + 2)

static HINSTANCE pp_dll = NULL;

/**
 * @brief パラレルポート出力関数ポインタ
 * @details 指定アドレスに値を書き込む
 * @param address レジスタアドレス (PP_DATA, PP_STATUS, PP_CONTROL)
 * @param value 書き込む値 (0x00-0xff)
 * 
 * 使用例:
 * - データ出力: pp_out(PP_DATA, 0xff);
 * - 制御設定: pp_out(PP_CONTROL, 0x00);
 */
static void(__stdcall *pp_out)(short, short) = NULL;

/**
 * @brief パラレルポート入力関数ポインタ
 * @details 指定アドレスから値を読み取る
 * @param address レジスタアドレス (PP_STATUS, PP_CONTROL)
 * @return 読み取った値 (0x00-0xff)
 * 
 * 使用例:
 * - ステータス読み取り: short s = pp_in(PP_STATUS);
 * - 制御読み取り: short c = pp_in(PP_CONTROL);
 */
static short(__stdcall *pp_in)(short) = NULL;

/**
 * @brief パラレルポート初期化関数
 * @details inpout32.dllをロードし、関数ポインタを取得する
 * @return 成功時0、失敗時-1
 * @note プログラム開始時に必ず呼び出すこと
 * 
 * 使用例:
 * @code
 * if (pp_init() != 0) {
 *     return 1;
 * }
 * @endcode
 */
static inline int pp_init(void) {
    pp_dll = LoadLibrary("inpout32.dll");
    if(!pp_dll) {
        printf("Error: Failed to load inpout32.dll\n");
        return -1;
    }
    pp_out = (void(__stdcall *)(short, short))GetProcAddress(pp_dll, "Out32");
    pp_in = (short(__stdcall *)(short))GetProcAddress(pp_dll, "Inp32");
    if(!pp_out || !pp_in) {
        printf("Error: Failed to get function addresses\n");
        FreeLibrary(pp_dll);
        pp_dll = NULL;
        return -1;
    }
    return 0;
}

/**
 * @brief パラレルポート終了処理関数
 * @details DLLを解放し、関数ポインタをNULLに設定する
 * @note プログラム終了時に必ず呼び出すこと
 * 
 * 使用例:
 * @code
 * pp_close();
 * @endcode
 */
static inline void pp_close(void) {
    if(pp_dll) {
        FreeLibrary(pp_dll);
        pp_dll = NULL;
        pp_out = NULL;
        pp_in = NULL;
    }
}

#endif  // PARALLELPORT_H