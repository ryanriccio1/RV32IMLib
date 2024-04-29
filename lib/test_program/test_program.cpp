#include <string>
#include <format>
#include <memory>
#include <vector>
#include <computer.h>

using namespace std;

template <typename T>
class Person
{
public:
    Person(const string &name, const T age) : name(name), age(age){};
    void set_age(T new_age)
    {
        age = new_age;
    }
    void set_name(string &new_name)
    {
        name = new_name;
    }
    void print_age()
    {
        print_screen(format("Your age is: {}\n", age));
    }
    void print_name()
    {
        print_screen(format("\nYour name is: {}\n", name));
    }

public:
    string name;
    T age;
};

int main()
{
    setup_character_mode();
    print_screen(format("\n{:^40}\n\n", "*** RV32-IM TERM ***"));

    print_screen("Enter your name: ");
    string name_in = input_screen();
    print_uart("Enter your age: ");
    uint8_t age_in = stoi(input_uart());
    clear_uart();

    const auto person = make_shared<Person<uint8_t>>(name_in, age_in);

    person->print_name();
    person->print_age();

    print_uart("Hello World!\rGoodbyeWorld!\b\nI am text that is on a new line!");

    input_screen();
    clear_screen();

    // multiplication
    print_screen("\nEnter an int: ");
    int ia = stoi(input_screen());

    print_screen("\nEnter another int: ");
    int ib = stoi(input_screen());

    print_screen(format("{} + {} = {}\n", ia, ib, ia + ib));
    print_screen(format("{} - {} = {}\n", ia, ib, ia - ib));
    print_screen(format("{} * {} = {}\n", ia, ib, ia * ib));
    print_screen(format("{} / {} = {}\n", ia, ib, ia / ib));

    // float arithmatic
    input_screen();
    clear_screen();
    print_screen("\nEnter a float: ");
    float fa = stof(input_screen());

    print_screen("\nEnter another float: ");
    float fb = stof(input_screen());

    print_screen(format("{} + {} = {}\n", fa, fb, fa + fb));
    print_screen(format("{} - {} = {}\n", fa, fb, fa - fb));
    print_screen(format("{} * {} = {}\n", fa, fb, fa * fb));
    print_screen(format("{} / {} = {}\n", fa, fb, fa / fb));

    // vector (and iterator/allocator/foreach(ranged-for))
    input_screen();
    clear_screen();
    std::vector<int> nums;
    for (int i = 0; i < 5; i++)
    {
        nums.push_back(i * 5);
    }
    for (auto num : nums)
    {
        nums.push_back(num * 10);
    }
    for (size_t i = 0; i < nums.size(); i++)
    {
        print_screen(format("[{}]: {}\n", i, nums[i]));
    }

    return 0;
}
