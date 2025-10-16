// parallelport.h - Header-only parallel port wrapper
// Usage: #include "parallelport.h" at the top of your program
#ifndef PARALLELPORT_H
#define PARALLELPORT_H

#include <windows.h>
#include <stdio.h>

// ============ User Configuration ============
#ifndef PARALLEL_PORT_BASE
#define PARALLEL_PORT_BASE 0x3fd8
#endif

// ============ Register Addresses ============
#define PP_DATA    (PARALLEL_PORT_BASE + 0)
#define PP_STATUS  (PARALLEL_PORT_BASE + 1)
#define PP_CONTROL (PARALLEL_PORT_BASE + 2)

// ============ Internal Variables ============
static HINSTANCE pp_dll = NULL;
static void  (__stdcall *pp_out)(short, short) = NULL;
static short (__stdcall *pp_in)(short) = NULL;

// ============ Initialization ============
static inline int pp_init(void) {
    pp_dll = LoadLibrary("inpout32.dll");
    if (!pp_dll) {
        printf("Error: Failed to load inpout32.dll\n");
        return -1;
    }
    pp_out = (void (__stdcall *)(short, short))GetProcAddress(pp_dll, "Out32");
    pp_in = (short (__stdcall *)(short))GetProcAddress(pp_dll, "Inp32");
    if (!pp_out || !pp_in) {
        printf("Error: Failed to get function addresses\n");
        FreeLibrary(pp_dll);
        pp_dll = NULL;
        return -1;
    }
    pp_out(PP_CONTROL, 0x00); // Set to output mode
    return 0;
}

// ============ Cleanup ============
static inline void pp_close(void) {
    if (pp_dll) {
        FreeLibrary(pp_dll);
        pp_dll = NULL;
        pp_out = NULL;
        pp_in = NULL;
    }
}

// ============ Data Register (D0-D7: Pin 2-9) ============
static inline void pp_write_data(short value) {
    if (!pp_out) {
        printf("Error: pp_init() not called\n");
        return;
    }
    pp_out(PP_DATA, value);
}

// ============ Status Register (S3-S7: Pin 15,13,12,10,11) ============
static inline short pp_read_status(void) {
    if (!pp_in) {
        printf("Error: pp_init() not called\n");
        return 0;
    }
    return pp_in(PP_STATUS);
}

// ============ Control Register (C0-C3: Pin 1,14,16,17) ============
static inline void pp_write_control(short value) {
    if (!pp_out) {
        printf("Error: pp_init() not called\n");
        return;
    }
    pp_out(PP_CONTROL, value);
}

static inline short pp_read_control(void) {
    if (!pp_in) {
        printf("Error: pp_init() not called\n");
        return 0;
    }
    return pp_in(PP_CONTROL);
}

#endif // PARALLELPORT_H