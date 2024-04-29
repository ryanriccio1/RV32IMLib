#include <irq.h>
#include <common.h>
#include <mem.h>

#if defined(EN_IRQ)
#define IRQ_EN 1

// volatile void port_a_irq()
// {
//     char str[2];
//     char in_char = *port_a;
//     while (in_char != 0)
//     {
//         in_char &= 0b01111111;
//         str[0] = in_char;
//         print_uart(str);
//         in_char = *port_a;
//     }
// }

void irq_handler()
{
    asm volatile(
        "add    sp, sp, -24\n"
        "sw     a0, 0(sp)\n"
        "sw     a1, 4(sp)\n"
        "sw     a2, 8(sp)\n"
        "sw     a3, 12(sp)\n"
        "sw     a4, 16(sp)\n"
        "sw     a5, 20(sp)");
    switch (read_address<uint8_t>(irq_vector))
    {
    case KEYBOARD:
        if (keyboard_irq)
            keyboard_irq();
        break;
    case UART_RX:
        if (uart_irq)
            uart_irq();
        break;
    case TIMER:
        if (timer_irq)
            timer_irq();
        break;
    default:
        break;
    }
    write_address<uint8_t>(irq_handle, true);
    asm volatile(
        "lw     a0, 0(sp)\n"
        "lw     a1, 4(sp)\n"
        "lw     a2, 8(sp)\n"
        "lw     a3, 12(sp)\n"
        "lw     a4, 16(sp)\n"
        "lw     a5, 20(sp)\n"
        "add    sp, sp, 24\n"
        "lw     ra, 12(sp)\n"
        "add    sp, sp, 16\n"
        "jr     tp");
}
#else
#define IRQ_EN 0
void irq_handler(){};
#endif

extern "C" void setup_irq_enabled()
{
    update_irq_en();
    write_address<uint8_t>(irq_en, IRQ_EN);
}

uint8_t get_keyboard_in()
{
    return read_address<uint8_t>(keyboard_in);
}

uint8_t get_uart_rx()
{
    return read_address<uint8_t>(uart_rx);
}

uint8_t get_timer_value()
{
    return read_address<uint8_t>(timer_in);
}
