#pragma once
#ifdef __cplusplus
extern "C"
{
#endif
#include <stdint.h>

    void c_print_uart(const char *str);
    void c_clear_uart();

    void c_print_screen(const char *str);
    void c_clear_screen();
#ifdef __cplusplus
}
#endif