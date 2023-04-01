#pragma once
#include<string>
#include<iostream>
using namespace std;

//请求格式
typedef struct request{
    int x;
    int y;
    char op; // +-*/%
}request_t;

//响应格式
typedef struct response{
    int result; //计算结果，不能区分是正常计算结果，还是异常退出。
    int code; //server运算完毕的计算状态，0为正常，-1为无意义，如除零错误

}response_t;
