#include <computer.h>
#include <format>
#include <map>
#include <string_view>
#include <any>
extern "C"
{
#include "ubasic.h"
}

/*
static const char program[] =
    "10 gosub 100\n\
20 for i = 1 to 78\n\
30 print i\n\
40 next i\n\
50 print \"end\"\n\
60 end\n\
100 print \"subroutine\"\n\
110 return\n";
*/

bool is_digit(char c)
{
    return std::isdigit(static_cast<unsigned char>(c));
}

// Function to extract the starting number from a string_view
std::any get_starting_number(std::string_view text)
{
    // Find the first non-digit character
    auto it = std::find_if(text.begin(), text.end(), [](char c)
                           { return !is_digit(c); });

    // If the string is empty or doesn't start with a digit, return nullopt
    if (it == text.begin() || !is_digit(*text.begin()))
    {
        return std::nullopt;
    }

    // Extract the substring of digits
    std::string_view number(text.begin(), it);

    // Try converting the substring to an integer
    try
    {
        return std::stoi(std::string(number));
    }
    catch (const std::invalid_argument &e)
    {
        // Handle potential conversion errors (e.g., non-numeric characters)
        return std::nullopt;
    }
}

std::string program_to_string(map<int, string> prg)
{
    string prg_string;
    for (const auto &[_, line] : prg)
    {
        prg_string += line + '\n';
    }
    return prg_string;
}

int main()
{
    setup_character_mode();
    print_screen(format("\n{:^40}", "*** RV32-IM TERM ***"));
    print_screen("Enter a program, type RUN to start.\n");

    while (true)
    {
        map<int, string> program;
        while (true)
        {
            string input = input_screen();
            string test_input = input;
            transform(test_input.begin(), test_input.end(), test_input.begin(), ::tolower);
            if (test_input == string("run"))
            {
                string program_string = program_to_string(program);
                ubasic_init(program_string.c_str());

                do
                {
                    ubasic_run();
                } while (!ubasic_finished());
            }
            else if (test_input == string("list"))
            {
                print_screen(program_to_string(program));
            }
            else if (test_input == string("clear"))
            {
                program.clear();
                clear_screen();
            }
            else
            {
                std::string_view str_view(input);

                auto maybe_number = get_starting_number(str_view);

                if (!maybe_number.has_value())
                {
                    print_uart("Invalid Input!\n");
                }
                else
                {
                    int number = std::any_cast<int>(maybe_number);
                    program[number] = input;
                }
            }
        }
    }
}