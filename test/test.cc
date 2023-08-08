#include <iostream>
#include <regex>
#include <string>
#include <cstring>
#include <bitset>
#include <arpa/inet.h>
using namespace std;

bool isValid(const string& ip) {
    regex re("(([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\\.){4}");
    return regex_match(ip, re);
}

char isAorBorCorDorE(const string& ip) {
    regex ra("([0-9]|[1-9][0-9]|1[01][0-9]|12[0-6])\\.(([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\\.){3}");
    if (regex_match(ip, ra)) return 'a';
    regex rb("(12[8-9]|1[3-8][0-9]|19[01])\\.(([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\\.){3}");
    if (regex_match(ip, rb)) return 'b';
    regex rc("(19[2-9]|2[01][0-9]|22[0-3])\\.(([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\\.){3}");
    if (regex_match(ip, rc)) return 'c';
    regex rd("(22[4-9]|23[0-9])\\.(([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\\.){3}");
    if (regex_match(ip, rd)) return 'd';
    regex re("(24[0-9]|25[0-5])\\.(([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\\.){3}");
    if (regex_match(ip, re)) return 'e';
    return 'f';
}
bool isValidmask(const string& mask) {
        if (!isValid(string(mask+'.'))) {return false;cout << "errorip ";}
        //不是合法ip直接返回
        vector<unsigned int> n(4);
        unsigned int b;
        //写入数组n中
        sscanf(mask.c_str(), "%u.%u.%u.%u", &n[3], &n[2], &n[1], &n[0]);
        //写入b中
        //将掩码转化成32无符号整型，取反为000...00111...1，然后再加1为00...01000...0，此时为2^n，如果满足就为合法掩码。
        for (int i = 0; i < 4; ++i) {
            b += (n[i] << (i * 8));
        }
        b = ~b + 1;
        if ((b & (b - 1)) == 0) {
            return true;
        }
        return false;
}

bool isValidMask(std::string mask) {
    int iRet = -1;
    // 将IP地址由“点分十进制”转换成 “二进制整数”
    struct in_addr s;
    iRet = inet_pton(AF_INET, mask.c_str(), &s); 
    // 转换成功返回1，说明是有效的IP地址
     if (iRet == 1) {
         // 从网络字节顺序转换为主机字节顺序
         unsigned int addr = ntohl(s.s_addr);
         // 转换为二进制字符串
         std::bitset<32> b((int)addr);
        std::string strMask = b.to_string();
        // 查找二进制字符串中的"01"，如果不存在，说明是有效的子网掩码
        return (strMask.find("01") == std::string::npos);
    }
    return false;
}
bool is_mask(string ip){
    istringstream iss(ip);
    string seg;
    unsigned b = 0;
    while(getline(iss,seg,'.')) b = (b << 8) + stoi(seg);
    if(!b) return false;
    b = ~b + 1;
    if(b == 1) return false;
    if((b & (b-1)) == 0) return true;
    return false;
}
int main() {
    vector<string> vs{{"255.110.255.255"},{"255.255.58.255"},{"255.255.154.0"},{"255.255.255.100"},{"255.255.111.255"},{"255.86.0.0"},{"255.255.255.139"},{"255.255.90.255"},{"255.153.0.0"},{"255.255.255.255"},{"255.0.15.0"},{"255.255.255.255"},{"255.0.0.226"},{"255.255.74.255"}};
    for(int i = 0; i < vs.size(); ++i) {
        if (is_mask(vs[i])) {
            cout << vs[i] << endl;
        }
    }
    return 0;
}