#pragma once
#include <cstdint>

using namespace std;

template <typename T>
T read_address(uint32_t address);

template <typename T>
void write_address(uint32_t address, T value);

template <typename T>
T read_address(uint32_t address)
{
    return *reinterpret_cast<T *>(address);
}

template <typename T>
void write_address(uint32_t address, T value)
{
    *reinterpret_cast<T *>(address) = value;
}