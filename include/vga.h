#pragma once
#include <cstdint>
#include <cstring>
#include <mem.h>
#include <common.h>

#include "common.h"
#include "mem.h"

struct RGB
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    RGB(const uint32_t color_data)
    {
        red = (color_data & 0x00FF0000) >> 16;
        green = (color_data & 0x0000FF00) >> 8;
        blue = color_data & 0x000000FF;
    }
    RGB(const uint8_t red, const uint8_t green, const uint8_t blue) : red(red), green(green), blue(blue) {}
    operator uint32_t() const
    {
        return red << 16 | green << 8 | blue;
    }
} __attribute__((packed));

struct Position
{
    int32_t x = 0;
    int32_t y = 0;

    Position &operator+=(const Position &rhs)
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    Position operator+(const Position &rhs)
    {
        Position result = Position(x, y);
        result.x += rhs.x;
        result.y += rhs.y;
        return result;
    }

    Position &operator-=(const Position &rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    Position operator-(const Position &rhs)
    {
        Position result = Position(x, y);
        result.x -= rhs.x;
        result.y -= rhs.y;
        return result;
    }

    bool operator==(const Position &rhs) const
    {
        if (x == rhs.x && y == rhs.y)
            return true;
        return false;
    }
};

enum VideoMode
{
    BITMAP,
    CHARACTER
};

enum CursorDirection
{
    NONE,
    UP = 128,
    DOWN,
    LEFT,
    RIGHT
};

constexpr unsigned char cursor_char = 178;

template <typename T>
class VideoControl
{
public:
    static constexpr uint32_t width_px = 320;
    static constexpr uint32_t height_px = 240;
    static constexpr uint32_t width_chr = width_px / 8;
    static constexpr uint32_t height_chr = height_px / 8;

    VideoControl(VideoMode mode, T video_memory);
    VideoControl(VideoMode mode, T video_memory, uint32_t *color_memory, uint8_t *character_memory);
    ~VideoControl();
    void set_mode(VideoMode mode);

    void set_pixel(const Position &position, const RGB &rgb);
    RGB get_pixel(const Position &position);

    char get_character(const Position &position);
    void set_character(const unsigned char character);
    void set_character(const unsigned char character, bool increment);
    // void set_character(const char character, const Position &position);
    void set_foreground(const Position &position, const RGB &rgb) const;
    void set_background(const Position &position, const RGB &rgb) const;
    void set_foreground(const RGB &rgb) const;
    void set_background(const RGB &rgb) const;

    void set_cursor(Position new_cursor);
    Position get_cursor() const;
    void increment_cursor();
    void cursor_up();
    void cursor_down();
    void cursor_left();
    void cursor_right();
    void cycle_cursor();
    void cursor_enable(bool enable);

    void clear();

private:
    uint32_t position_to_offset(const Position &position) const;
    CursorDirection check_cursor_overrun(const CursorDirection direction) const;
    void cursor_newline();
    void cursor_carriage_return();
    void restore_last_character();
    void set_last_character();
    void add_new_row();

    VideoMode mode;
    Position cursor;
    bool cursor_showing;
    bool cursor_enabled;
    char cursor_last_char;

    T video_memory;
    uint32_t *color_memory;
    uint8_t *character_memory;
};

inline VideoControl<uint8_t *> *screen; // global char mode screen for print
inline VideoControl<uint32_t *> *bitmap_control;

template <typename T>
VideoControl<T>::VideoControl(VideoMode mode, T video_memory) : VideoControl(mode, video_memory, nullptr, nullptr) {}

template <typename T>
VideoControl<T>::VideoControl(const VideoMode mode, T video_memory, uint32_t *color_memory, uint8_t *character_memory) : mode(mode), cursor(0, 0), cursor_showing(true), cursor_enabled(true), cursor_last_char(0), video_memory(video_memory), color_memory(color_memory), character_memory(character_memory)
{
    update_offsets();
    set_mode(mode);
    write_address(vga_mem_ptr, reinterpret_cast<uint32_t>(video_memory));
    write_address(col_mem_ptr, reinterpret_cast<uint32_t>(color_memory));
    write_address(chr_mem_ptr, reinterpret_cast<uint32_t>(character_memory));
}

template <typename T>
VideoControl<T>::~VideoControl()
{
    if (video_memory)
        delete video_memory;
    if (color_memory)
        delete color_memory;
    if (character_memory)
        delete character_memory;
}

template <typename T>
void VideoControl<T>::set_mode(VideoMode mode)
{
    this->mode = mode;
    write_address<uint8_t>(vga_mode, mode);
}

template <typename T>
void VideoControl<T>::set_pixel(const Position &position, const RGB &rgb)
{
    if (mode == BITMAP)
    {
        video_memory[position_to_offset(position)] = static_cast<uint32_t>(rgb);
    }
}

template <typename T>
RGB VideoControl<T>::get_pixel(const Position &position)
{
    if (mode == BITMAP)
    {
        return video_memory[position_to_offset(position)];
    }
    return 0;
}

template <typename T>
char VideoControl<T>::get_character(const Position &position)
{
    if (mode == CHARACTER)
    {
        return video_memory[position_to_offset(position)];
    }
    return 0;
}

template <typename T>
void VideoControl<T>::set_character(const unsigned char character)
{
    set_character(character, true);
}

template <typename T>
void VideoControl<T>::set_character(const unsigned char character, bool increment)
{
    if (mode == CHARACTER)
    {
        switch (character)
        {
        case '\b':
            cursor_left();
            video_memory[position_to_offset(cursor)] = '\0';
            cursor_last_char = '\0';
            break;
        case '\n':
            cursor_newline();
            cursor_last_char = '\0';
            break;
        case '\r':
            cursor_carriage_return();
            cursor_last_char = '\0';
            break;
        default:
            video_memory[position_to_offset(cursor)] = character;
            if (character != cursor_char)
                cursor_last_char = character;
            if (increment)
                increment_cursor();
            break;
        }
    }
}

// template <typename T>
// void VideoControl<T>::set_character(const char character, const Position &position)
// {
//     if (mode == CHARACTER)
//     {
//         video_memory[position_to_offset(position)] = character;
//         increment_cursor();
//     }
// }

template <typename T>
void VideoControl<T>::set_foreground(const Position &position, const RGB &rgb) const
{
    if (mode == CHARACTER)
    {
        color_memory[position_to_offset(position) * 2 + 1] = rgb;
    }
}

template <typename T>
void VideoControl<T>::set_background(const Position &position, const RGB &rgb) const
{
    if (mode == CHARACTER)
    {
        color_memory[position_to_offset(position) * 2] = rgb;
    }
}

template <typename T>
void VideoControl<T>::set_foreground(const RGB &rgb) const
{
    for (uint32_t y = 0; y < height_chr; y++)
        for (uint32_t x = 0; x < width_chr; x++)
            set_foreground(Position(x, y), rgb);
}

template <typename T>
void VideoControl<T>::set_background(const RGB &rgb) const
{
    for (uint32_t y = 0; y < height_chr; y++)
        for (uint32_t x = 0; x < width_chr; x++)
            set_background(Position(x, y), rgb);
}

template <typename T>
void VideoControl<T>::set_cursor(const Position new_cursor)
{
    if (mode == CHARACTER && new_cursor.x < static_cast<int32_t>(width_chr) && new_cursor.y < static_cast<int32_t>(height_chr))
        cursor = new_cursor;
}

template <typename T>
Position VideoControl<T>::get_cursor() const
{
    return cursor;
}

template <typename T>
void VideoControl<T>::increment_cursor()
{
    cursor_right();
}

template <typename T>
void VideoControl<T>::cursor_up()
{
    restore_last_character();
    if (check_cursor_overrun(UP) != NONE)
        cursor.y = height_chr - 1;
    else
        cursor.y -= 1;
    set_last_character();
}

template <typename T>
void VideoControl<T>::cursor_down()
{
    restore_last_character();
    if (check_cursor_overrun(DOWN) != NONE)
        add_new_row();
    // cursor.y = 0;
    else
        cursor.y += 1;
    set_last_character();
}

template <typename T>
void VideoControl<T>::cursor_left()
{
    restore_last_character();
    if (check_cursor_overrun(LEFT) != NONE)
    {
        cursor.x = width_chr - 1;
        if (check_cursor_overrun(UP) == NONE)
            cursor.y -= 1;
    }
    else
        cursor.x -= 1;
    set_last_character();
}

template <typename T>
void VideoControl<T>::cursor_right()
{
    restore_last_character();
    if (check_cursor_overrun(RIGHT) != NONE)
    {
        cursor.x = 0;
        if (check_cursor_overrun(DOWN) == NONE)
            cursor.y += 1;
    }
    else
        cursor.x += 1;
    set_last_character();
}

template <typename T>
void VideoControl<T>::cycle_cursor()
{
    if (cursor_enabled)
    {
        if (cursor_showing)
        {
            restore_last_character();
            cursor_showing = false;
        }
        else
        {
            set_last_character();
            set_character(char(178), false);
            cursor_showing = true;
        }
    }
}

template <typename T>
void VideoControl<T>::cursor_enable(bool enable)
{
    cursor_enabled = enable;
}

template <typename T>
void VideoControl<T>::clear()
{
    if (mode == CHARACTER)
    {
        memset(video_memory, 0, width_chr * height_chr);
    }
}

template <typename T>
uint32_t VideoControl<T>::position_to_offset(const Position &position) const
{
    if (mode == BITMAP)
    {
        return position.x + (position.y * width_px);
    }
    if (mode == CHARACTER)
    {
        return position.x + (position.y * width_chr);
    }
    return 0;
}

template <typename T>
CursorDirection VideoControl<T>::check_cursor_overrun(const CursorDirection direction) const
{
    switch (direction)
    {
    case UP:
        if (cursor.y - 1 < 0)
            return UP;
        else
            return NONE;
    case DOWN:
        if (cursor.y + 1 >= static_cast<int>(height_chr))
            return DOWN;
        else
            return NONE;
    case LEFT:
        if (cursor.x - 1 < 0)
            return LEFT;
        else
            return NONE;
    case RIGHT:
        if (cursor.x + 1 >= static_cast<int>(width_chr))
            return RIGHT;
        else
            return NONE;
    default:
    case NONE:
        return NONE;
    }
}

template <typename T>
void VideoControl<T>::cursor_newline()
{
    restore_last_character();
    cursor_carriage_return();
    if (check_cursor_overrun(DOWN) != NONE)
        add_new_row();
    // cursor.y = 0;
    else
        cursor.y += 1;
}

template <typename T>
void VideoControl<T>::cursor_carriage_return()
{
    cursor.x = 0;
}

template <typename T>
void VideoControl<T>::restore_last_character()
{
    set_character(cursor_last_char, false);
}

template <typename T>
void VideoControl<T>::set_last_character()
{
    cursor_last_char = get_character(cursor);
}

template <typename T>
void VideoControl<T>::add_new_row()
{
    if (mode == CHARACTER)
    {
        for (uint32_t ptr_offset{0}; ptr_offset < height_chr * (width_chr - 1); ptr_offset += width_chr)
        {
            memcpy(video_memory + ptr_offset, video_memory + ptr_offset + width_chr, width_chr);
        }
        memset(video_memory + (width_chr * (height_chr - 1)), 0, width_chr);
    }
}
