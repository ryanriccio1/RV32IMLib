#include <computer.h>
#include <image_prg.h>
#include <vga.h>
#include <cstdint>
#include <vector>
#include <random>

using namespace std;

vector<int> random_fill_vector(int max_value)
{
    vector<int> vec;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, max_value - 1);

    vec.resize(max_value);

    // Fill the vector with random numbers using the distribution
    for (int &value : vec)
    {
        value = dis(gen);
    }
    return vec;
}

int main()
{
    setup_character_mode();
    screen->cursor_enable(false);
    screen->set_foreground(RGB(0, 0, 0));
    show_input_screen = false;
    auto random_vec = random_fill_vector(40 * 30);
    auto en_timer = new bool;
    *en_timer = true;

    timer_irq = [=]() mutable
    {
        if (*en_timer)
        {
            static int counter = 0;
            screen->set_cursor(Position(random_vec[counter] % 40, random_vec[counter] / 30));
            screen->set_character(177);
            counter++;
        }
    };

    for (uint32_t x{0}; x < 40; x++)
    {
        for (uint32_t y{0}; y < 30; y++)
        {
            screen->set_background(Position(x, y), RGB(background_color[x + y * 40]));
        }
    }
    input_screen();
    *en_timer = false;
}