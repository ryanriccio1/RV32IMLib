#pragma once

#include <riscv-stdio.h>
#include <irq.h>
#include <vga.h>
#include <font.h>
#include <clocale>

inline void setup_character_mode()
{
    setlocale(LC_ALL, "en_US.UTF-8");
    auto vga_mem = new uint8_t[30 * 40];
    auto color_mem = new uint32_t[30 * 40 * 2];
    screen = new VideoControl(CHARACTER, vga_mem, color_mem, font);

    screen->set_background(RGB(0, 0, 0));
    screen->set_foreground(RGB(255, 255, 255));

    keyboard_irq = [=]() mutable
    {
        switch (const char keypress = get_keyboard_in())
        {
        case UP:
            screen->cursor_up();
            break;
        case DOWN:
            screen->cursor_down();
            break;
        case LEFT:
            screen->cursor_left();
            break;
        case RIGHT:
            screen->cursor_right();
            break;
        default:
            screen_buffer.add(keypress);
            if (show_input_screen)
                print_screen(string(1, keypress));
            break;
        }
    };

    uart_irq = [&]() mutable
    {
        uart_buffer.add(get_uart_rx());
        if (show_input_uart)
            print_uart(string(1, get_uart_rx()));
    };

    timer_irq = [=]() mutable
    {
        if (read_address<uint8_t>(timer_in) % 64 == 0)
            screen->cycle_cursor();
    };
}

inline void disable_character_mode()
{
    timer_irq = nullptr;
    keyboard_irq = nullptr;
    if (screen)
        delete screen;
}

inline void setup_bitmap_mode()
{
    setlocale(LC_ALL, "en_US.UTF-8");
    auto vga_mem = new uint32_t[320 * 240];
    bitmap_control = new VideoControl(BITMAP, vga_mem);

    uart_irq = [&]() mutable
    {
        uart_buffer.add(get_uart_rx());
        if (show_input_uart)
            print_uart(string(1, get_uart_rx()));
    };
}

inline void disable_bitmap_mode()
{
    if (bitmap_control)
        delete bitmap_control;
}