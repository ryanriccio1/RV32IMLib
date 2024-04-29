#include <computer.h>
#include <format>
#include <ranges>
#include <deque>
#include <algorithm>
using namespace std;

enum class IntersectionType
{
    NONE,
    SELF,
    FOOD,
    WALL
};

void draw_map()
{
    screen->set_background(RGB(86, 138, 52));
    for (uint32_t x{0}; x < 40; x++)
    {
        for (uint32_t y{0}; y < 30; y++)
        {
            screen->set_cursor(Position(x, y));
            if ((y == 1 || y == 28 || y == 29) ||
                ((x == 0 || x == 39 || x == 1 || x == 38) && y != 0))
            {
                screen->set_foreground(Position(x, y), RGB(74, 117, 45));
                screen->set_character(char(177), false);
            }
            else if (y == 0)
            {
                screen->set_background(Position(x, y), RGB(74, 117, 45));
                screen->set_foreground(Position(x, y), RGB(255, 255, 255));
            }
            else if (y % 2 == 0)
            {
                x % 2 == 0 ? screen->set_background(Position(x, y), RGB(162, 209, 73)) : screen->set_background(Position(x, y), RGB(170, 215, 81));
            }
            else if (y % 2 == 1)
            {
                x % 2 == 0 ? screen->set_background(Position(x, y), RGB(170, 215, 81)) : screen->set_background(Position(x, y), RGB(162, 209, 73));
            }
        }
    }
}

void update_charset()
{
    font[219 * 8] = 0x00;
    font[219 * 8 + 1] = 0x7E;
    font[219 * 8 + 2] = 0x7E;
    font[219 * 8 + 3] = 0x7E;
    font[219 * 8 + 4] = 0x7E;
    font[219 * 8 + 5] = 0x7E;
    font[219 * 8 + 6] = 0x7E;
    font[219 * 8 + 7] = 0x00;
}

void clear_board()
{
    for (uint8_t x{2}; x < 38; x++)
    {
        for (uint8_t y{2}; y < 28; y++)
        {
            screen->set_cursor(Position(x, y));
            screen->set_character(' ', false);
        }
    }
}

void draw_snake(const deque<Position> &snake_data)
{
    for (auto const &pos : snake_data | views::take(1))
    {
        screen->set_cursor(pos);

        screen->set_character(char(2), false);
        screen->set_foreground(pos, RGB(78, 124, 246));
    }
    for (auto const &pos : snake_data | views::drop(1))
    {
        screen->set_cursor(pos);

        screen->set_character(char(219), false);
        screen->set_foreground(pos, RGB(66, 111, 227));
    }
}

void draw_food(const vector<Position> &food)
{
    for (auto const &pos : food)
    {
        screen->set_cursor(pos);

        screen->set_character(char(15), false);
        screen->set_foreground(pos, RGB(232, 72, 29));
    }
}

Position get_next_position()
{
    static Position last_direction = Position(0, 1);
    switch (read_address<char>(keyboard_in))
    {
    case UP:
        last_direction = Position(0, -1);
        return last_direction;
    case DOWN:
        last_direction = Position(0, 1);
        return last_direction;
    case LEFT:
        last_direction = Position(-1, 0);
        return last_direction;
    case RIGHT:
        last_direction = Position(1, 0);
        return last_direction;
    default:
        return last_direction;
    }
}

IntersectionType calculate_intersection(const deque<Position> &snake_data, vector<Position> &food_data)
{
    auto snake_head = snake_data.front();

    // wall
    if (snake_head.x <= 1 || snake_head.x >= 38 ||
        snake_head.y <= 1 || snake_head.y >= 28)
        return IntersectionType::WALL;

    // food
    auto food_iterator = food_data.begin();
    while (food_iterator != food_data.end())
    {
        if (snake_head == *food_iterator)
        {
            food_iterator = food_data.erase(food_iterator);
            return IntersectionType::FOOD;
        }
        else
        {
            food_iterator++;
        }
    }

    // self
    for (auto const &pos : snake_data | views::drop(1))
    {
        if (pos == snake_head)
            return IntersectionType::SELF;
    }

    return IntersectionType::NONE;
}

void calculate_food(const deque<Position> &snake_data, vector<Position> &food_data)
{
    if (food_data.size() < 3)
    {
        while (true)
        {
            int food_x = rand() % (37 - 2 + 1) + 2;
            int food_y = rand() % (27 - 2 + 1) + 2;

            if (find(snake_data.begin(), snake_data.end(), Position(food_x, food_y)) == snake_data.end())
            {
                food_data.push_back(Position(food_x, food_y));
                return;
            }
        }
    }
}

void game_over(int score)
{
    for (uint8_t x{10}; x < 30; x++)
    {
        screen->set_foreground(Position(x, 8), RGB(86, 138, 52));
        screen->set_foreground(Position(x, 20), RGB(86, 138, 52));
        for (uint8_t y{9}; y < 20; y++)
        {
            screen->set_cursor(Position(x, y));
            screen->set_character('\0');
            screen->set_background(Position(x, y), RGB(86, 138, 52));
            screen->set_foreground(Position(x, y), RGB(255, 255, 255));
        }
    }
    screen->set_cursor(Position(9, 8));
    screen->set_foreground(Position(9, 8), RGB(86, 138, 52));
    screen->set_character(218);
    print_screen(string(21, 196));

    screen->set_cursor(Position(9, 20));
    screen->set_foreground(Position(9, 20), RGB(86, 138, 52));
    screen->set_character(192);
    print_screen(string(21, 196));

    screen->set_cursor(Position(30, 8));
    screen->set_foreground(Position(30, 8), RGB(86, 138, 52));
    screen->set_character(191);

    screen->set_cursor(Position(30, 20));
    screen->set_foreground(Position(30, 20), RGB(86, 138, 52));
    screen->set_character(217);

    screen->set_cursor(Position(10, 14));
    print_screen(format("{:^21}", "GAME OVER!"));

    exit(0);
}

int main()
{
    setup_character_mode();
    show_input_screen = false;
    screen->cursor_enable(false);
    update_charset();
    draw_map();

    deque<Position> snake = {Position(2, 5), Position(2, 4), Position(2, 3), Position(2, 2)};
    vector<Position> food = {Position{15, 15}};
    int speed = 2;
    int score = 0;

    timer_irq = [=]() mutable
    {
        static uint8_t counter = 0;
        counter++;
        // uint8_t timer = read_address<uint8_t>(timer_in)
        if (counter == speed)
        {
            counter = 0;
            calculate_food(snake, food);
            draw_snake(snake);
            draw_food(food);

            switch (calculate_intersection(snake, food))
            {
            default:
            case IntersectionType::NONE:
                screen->set_cursor(snake.back());
                screen->set_character('\0', false);
                snake.pop_back();
                break;
            case IntersectionType::FOOD:
                score += 25;
                break;
            case IntersectionType::SELF:
            case IntersectionType::WALL:
                game_over(score);
                break;
            }

            snake.push_front(snake.front() + get_next_position());
            screen->set_cursor(Position(0, 0));
            print_screen(format("SCORE: {:>10}", score));
        }
    };
}