#pragma once
#include <cstdint>

constexpr uint32_t ram_size = 0x400000;

inline uint32_t vga_mode = 0xFFF00;
inline uint32_t vga_mem_ptr = 0xFFF04;
inline uint32_t col_mem_ptr = 0xFFF08;
inline uint32_t chr_mem_ptr = 0xFFF0C;

inline uint32_t irq_mask = 0xFFFF0;
inline uint32_t timer_in = 0xFFFF1;
inline uint32_t keyboard_in = 0xFFFF2;
inline uint32_t uart_tx = 0xFFFF3;
inline uint32_t uart_rx = 0xFFFF4;
inline uint32_t irq_en = 0xFFFF5;
inline uint32_t irq_handle = 0xFFFF6;
inline uint32_t irq_vector = 0xFFFF7;
inline uint32_t data_ready = 0xFFFF8;

static constexpr uint32_t offset_vga_mode = 0x34;
static constexpr uint32_t offset_vga_mem_ptr = 0x30;
static constexpr uint32_t offset_col_mem_ptr = 0x2C;
static constexpr uint32_t offset_chr_mem_ptr = 0x28;

static constexpr uint32_t offset_irq_mask = 0x24;
static constexpr uint32_t offset_timer_in = 0x20;
static constexpr uint32_t offset_keyboard_in = 0x1C;
static constexpr uint32_t offset_uart_tx = 0x18;
static constexpr uint32_t offset_uart_rx = 0x14;
static constexpr uint32_t offset_irq_en = 0x10;
static constexpr uint32_t offset_irq_handle = 0x0C;
static constexpr uint32_t offset_irq_vector = 0x08;
static constexpr uint32_t offset_data_ready = 0x04;

inline void update_offsets()
{
    vga_mode = ram_size - offset_vga_mode;
    vga_mem_ptr = ram_size - offset_vga_mem_ptr;
    col_mem_ptr = ram_size - offset_col_mem_ptr;
    chr_mem_ptr = ram_size - offset_chr_mem_ptr;

    irq_mask = ram_size - offset_irq_mask;
    timer_in = ram_size - offset_timer_in;
    keyboard_in = ram_size - offset_keyboard_in;
    uart_tx = ram_size - offset_uart_tx;
    uart_rx = ram_size - offset_uart_rx;
    irq_en = ram_size - offset_irq_en;
    irq_handle = ram_size - offset_irq_handle;
    irq_vector = ram_size - offset_irq_vector;
    data_ready = ram_size - offset_data_ready;
}

inline void update_irq_en()
{
    irq_en = ram_size - offset_irq_en;
}