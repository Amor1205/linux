#include<iostream>
#include<string>
#include<jsoncpp/json/json.h>

typedef struct request{
    int x;
    int y;
    char op; // +-*/%
}request_t;
int main(){
    request_t req = {10, 20, '*'};
    Json::Value root; //可以承装任何对象，json 是kv式序列化方案。
    root["datax"] = req.x;
    root["datay"] = req.y;
    root["operator"] = req.op;

    //Writer 有两种，FastWriter, StyledWriter
    Json::StyledWriter writer;
    std::string json_string = writer.write(root);
    std::cout << json_string << std::endl;


    std::cout << std::endl;

    //反序列化
    request_t rec;
    Json::Reader reader;
    Json::Value root2;
    reader.parse(json_string, root2);
    rec.x = root2["datax"].asInt();
    rec.y = root2["datay"].asInt();
    rec.x = (char)root2["operator"].asInt();

    std::cout << req.x << " " << req.op << " " << req.y << std::endl;

    return 0;

}