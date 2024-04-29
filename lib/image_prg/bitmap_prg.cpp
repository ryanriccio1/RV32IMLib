#include <computer.h>
#include <bitmap_prg.h>
using namespace std;

int main()
{
    setup_bitmap_mode();
    uart_irq = [&]() mutable
    {
        uart_buffer.add(get_uart_rx());
        if (show_input_uart)
            print_uart(string(1, get_uart_rx()));
    };

    write_address(vga_mem_ptr, niceland);
    input_uart();

    const uint32_t *frames[] = {frame0, frame1, frame2, frame3, frame4, frame5, frame6, frame7};

    timer_irq = [=]()
    {
        static int counter = 0;
        write_address(vga_mem_ptr, frames[counter]);
        if (counter == 7)
            counter = 0;
        else
            counter++;
    };
}