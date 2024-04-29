#pragma once
#define EN_IRQ 1
#include <functional>
#include <cstdint>

using namespace std;

enum IRQType
{
    KEYBOARD = 1,
    UART_RX = 2,
    TIMER = 4
};

inline function<void(void)> keyboard_irq;
inline function<void(void)> uart_irq;
inline function<void(void)> timer_irq;

void irq_handler() __attribute__((section(".irqhandler")));
extern "C" void setup_irq_enabled() __attribute__((section(".irqen")));

uint8_t get_keyboard_in();
uint8_t get_uart_rx();
uint8_t get_timer_value();
