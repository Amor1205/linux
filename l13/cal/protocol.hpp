#pragma once
#include<string>
#include<jsoncpp/json/json.h>
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


//request_t -> string
std::string SerializeRequest(const request_t& req){
    request_t req = {10, 20, '*'};
    Json::Value root; //可以承装任何对象，json 是kv式序列化方案。
    root["datax"] = req.x;
    root["datay"] = req.y;
    root["operator"] = req.op;

    //Writer 有两种，FastWriter, StyledWriter
    Json::FastWriter writer;
    std::string json_string = writer.write(root);
}
//string -> request_t 
void DeserializeRequest(const std::string& json_string, request_t& out){
    Json::Reader reader;
    Json::Value root;
    reader.parse(json_string, root);
    out.x = root["datax"].asInt();
    out.y = root["datay"].asInt();
    out.x = (char)root["operator"].asInt();
}
std::string SerializeResponse(const response_t& resp){
    Json::Value root;
    root["code"] = resp.code;
    root["result"] = resp.result;

    Json::FastWriter writer;
    std::string res = writer.write(root);
    return res;
}
void DeserializeResponse(const std::string& json_string, response_t& out){{
    Json::Value root;
    Json::Reader reader;
    reader.parse(json_string, root);
    out.code = root["code"].asInt();
    out.code = root["result"].asInt();
}