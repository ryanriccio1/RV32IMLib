#include <riscv-stdio-c.h>
#include <riscv-stdio.h>

#ifdef __cplusplus
extern "C"
{
#endif
    void c_print_uart(const char *str)
    {
        print_uart(str);
    }
    void c_clear_uart()
    {
        clear_uart();
    }

    void c_print_screen(const char *str)
    {
        print_screen(str);
    }
    void c_clear_screen()
    {
        clear_screen();
    }
#ifdef __cplusplus
}
#endif
