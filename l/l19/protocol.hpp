#pragma once

#include <cstring>
#include<iostream>
#include<string>
#include<vector>
#include<jsoncpp/json/json.h>
//1. 报文和报文之间，采用特殊字符来进行解决粘包问题。
//2. 获取一个一个独立完整的报文，序列和反序列化 --- 自定义
//

const char* SEP = "X";
const int SEP_LEN = strlen(SEP);

// class Request{

// };
// class Response{

// };
//把传入进来的缓冲区进行切分
//1. buffer 被切走的，也同时要从buffer 中移除
//2. 可能会存在多个报文， 多个报文依次放入out
// buffer : 输入输出型参数
// out : 输出型参数
void SpliteMessage(std::string& buffer, std::vector<std::string> *out){
    //可能性 1.不全 2.多点 3. 好几份
    while(true){
        auto pos = buffer.find(SEP);
        if(std::string::npos == pos) break; //没拿到完整的
        std::string message = buffer.substr(0,pos);
        buffer.erase(0, pos + SEP_LEN);
        out->push_back(message);
    }
}



////////////////////////

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
    Json::Value root; //可以承装任何对象，json 是kv式序列化方案。
    root["datax"] = req.x;
    root["datay"] = req.y;
    root["operator"] = req.op;

    //Writer 有两种，FastWriter, StyledWriter
    Json::FastWriter writer;
    std::string json_string = writer.write(root);
    return json_string;
}
//string -> request_t 
void DeserializeRequest(const std::string& json_string, request_t& out){
    Json::Reader reader;
    Json::Value root;
    reader.parse(json_string, root);
    out.x = root["datax"].asInt();
    out.y = root["datay"].asInt();
    out.op = (char)root["operator"].asInt();
}
std::string SerializeResponse(const response_t& resp){
    Json::Value root;
    root["code"] = resp.code;
    root["result"] = resp.result;

    Json::FastWriter writer;
    std::string res = writer.write(root);
    return res;
}
void DeserializeResponse(const std::string& json_string, response_t& out){
    Json::Value root;
    Json::Reader reader;
    reader.parse(json_string, root);
    out.code = root["code"].asInt();
    out.result = root["result"].asInt();
}