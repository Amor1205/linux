#pragma once
#include<iostream>
#include<pthread.h>

namespace ns_task
{
    class Task
    {
    private:
        int _x;
        int _y;
        char _op; // + - * /
    public:
        Task()
        {

        }
        Task(int x, int y, char op)
            :_x(x), _y(y), _op(op)
        {

        }
        ~Task()
        {

        }
        std::string Show() const 
        {
            std::string message = std::to_string(_x);
            message += _op;
            message += std::to_string(_y);
            message += "= ";
            return message;
        }
        int Run()
        {
            int res = 0;
            switch(_op)
            {
                case '+':
                    res = _x + _y;
                    break;
                case '-':
                    res = _x - _y;
                    break;
                case '*':
                    res = _x * _y;
                    break;
                case '/':
                    res = _x / _y;
                    break;
                case '%':
                    res = _x % _y;
                    break;
                default:
                    std::cout << "false" << std::endl;
                    break;
            }
            std::cout << "Current task is completed by the thread [" << pthread_self()<< "] ["<< _x << _op << _y << "=" << res <<"]" << std::endl;
        }

    };
};