#pragma once
#include <cstdint>

class InputBuffer
{
public:
    void add(char character);
    void start_input();
    char *get_input();
    bool is_reading();

private:
    bool read_input = false;
    uint32_t current_idx = 0;
    char input[1024];
};

inline void InputBuffer::add(char character)
{
    if (read_input)
    {
        switch (character)
        {
        case '\b':
            current_idx--;
            break;
        case '\n':
        case '\r':
            read_input = false;
            break;
        default:
            if (isprint(character) || isblank(character))
            {
                input[current_idx] = character;
                current_idx++;
            }
        }
        if (current_idx >= 1024)
            current_idx = 0;
        input[current_idx] = '\0';
    }
}

inline void InputBuffer::start_input()
{
    read_input = true;
    current_idx = 0;
    input[0] = '\0';
}

inline char *InputBuffer::get_input()
{
    return input;
}

inline bool InputBuffer::is_reading()
{
    return read_input;
}