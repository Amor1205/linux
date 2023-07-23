#include<iostream>
#include<vector>
#include<functional>
#include<string>

void print(const std::string& str, int num)
{
    std::cout << str << num << std::endl;
}

int main()
{
    using Task = std::function<void()>;
    std::vector<Task> array;
    array.push_back(std::bind(print, "hello", 1));
    array.push_back(std::bind(print, "hello", 2));
    array.push_back(std::bind(print, "hello", 3));
    array.push_back(std::bind(print, "hello", 4));
    array.push_back(std::bind(print, "hello", 5));
    for(auto &f : array)  f();
    // auto func = std::bind(print, "hello", std::placeholders::_1);
    return 0;
}