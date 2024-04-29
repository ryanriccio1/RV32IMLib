#pragma once
#include <cstdint>
#include <string>
#include <common.h>
#include <vga.h>
#include <input_buffer.h>

using namespace std;

inline InputBuffer uart_buffer;
inline InputBuffer screen_buffer;

inline bool show_input_screen = true;
inline bool show_input_uart = false;

string input_uart();
string input_screen();

void print_uart(const char *str);
void print_uart(const char *str, uint32_t len);
void print_uart(const string &str);
void clear_uart();

void print_screen(const char *str);
void print_screen(const char *str, uint32_t len);
void print_screen(const string &str);
void clear_screen();

inline string input_uart()
{
    show_input_uart = true;
    uart_buffer.start_input();
    while (uart_buffer.is_reading())
    {
        asm volatile("nop");
    }
    show_input_uart = false;
    return string(uart_buffer.get_input());
}

inline string input_screen()
{
    screen_buffer.start_input();
    while (screen_buffer.is_reading())
    {
        asm volatile("nop");
    }
    return string(screen_buffer.get_input());
}

inline void print_uart(const char *str)
{
    uint32_t idx = 0;
    while (str[idx] != '\0')
    {
        write_address<uint8_t>(uart_tx, str[idx]);
        if (read_address<uint8_t>(data_ready) == 0)
        {
            write_address<uint8_t>(data_ready, 1);
            idx++;
        }
    }
}

inline void print_uart(const char *str, uint32_t len)
{
    uint32_t idx = 0;
    while (idx < len)
    {
        write_address<uint8_t>(uart_tx, str[idx]);
        if (read_address<uint8_t>(data_ready) == 0)
        {
            write_address<uint8_t>(data_ready, 1);
            idx++;
        }
    }
}
inline void print_uart(const string &str)
{
    for (const auto character : str)
    {
        write_address<uint8_t>(uart_tx, character);
        while (read_address<uint8_t>(data_ready) != 0)
        {
        } // wait for data ready to be open
        write_address<uint8_t>(data_ready, 1);
    }
}

inline void clear_uart()
{
    print_uart(string(1, char(2)));
}

inline void print_screen(const char *str)
{
    uint32_t idx = 0;
    while (str[idx] != '\0')
    {
        screen->set_character(str[idx]);
        idx++;
    }
}

inline void print_screen(const char *str, uint32_t len)
{
    uint32_t idx = 0;
    while (idx < len)
    {
        screen->set_character(str[idx]);
        idx++;
    }
}

inline void print_screen(const string &str)
{
    for (const auto character : str)
    {
        screen->set_character(character);
    }
}

inline void clear_screen()
{
    screen->set_cursor(Position(0, 0));
    screen->clear();
}
