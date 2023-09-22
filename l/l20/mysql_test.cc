#include<iostream>
#include<cstdio>
#include<mysql/mysql.h>
const std::string host = "127.0.0.1";
const std::string user = "root";
std::string password = "password";
std::string db = "test"; // 
const int port = 3306;
int main(){
    // std::cout << "client version : " <<  mysql_get_client_info() << std::endl;
    //0. 创建mysql 句柄
    MYSQL* my = mysql_init(nullptr);
    //1. 连接数据库
    if(mysql_real_connect(my, host.c_str(), user.c_str(), password.c_str(), db.c_str(), port, nullptr, 0) == nullptr)
    {
        std::cout << "connection failed." << std::endl;
        return 1;
    }
    //1.1 设置连接的编码格式
    mysql_set_character_set(my, "utf8");
    std::cout << "connection is successful" << std::endl;
    //2. 访问数据库
    std::string sql = "insert into test values(1, \'name\')";
    std::cout << "please input what you wanna do using mysql:> ";
    std::cin >> sql;
    //会出现乱码， 默认是使用拉丁而非utf8
    int res = mysql_query(my, sql.c_str());
    //2.1 select 需要处理，因为要解析数据。
    //使用mysql_store_result来接受。注意，接收的时候自动malloc 一块内存，需要手动free
    MYSQL_RES* result = mysql_store_result(my);
    int rows = mysql_num_rows(result); //获取行数
    int cols = mysql_num_fields(result); //获取列数
    std::cout << "rows : " << rows << "cols : " << cols << std::endl;
    
    //2.3 解析列名，但是一般不用。
    MYSQL_FIELD* fields = mysql_fetch_fields(result);
    for(int i = 0; i < cols; i++)
    {
        std::cout << fields[i].name << "\t";
    }
    std::cout << std::endl;
    //2.4 解析表中数据->重要
    for(int i = 0; i < rows; ++i)
    {
        MYSQL_ROW line = mysql_fetch_row(result); //获取完整的一行记录，可能包含多列
        
        for(int j = 0; j < cols; ++j)
        {
            std::cout << line[j] << "\t";
        }
        std::cout << std::endl;
    }

    if(res != 0)
    {
        std::cout << "executing " << sql << "failed." << std::endl;
        return 2;
    }
    std::cout << "executing " << sql << "successfully." << std::endl;

    //3. 关闭数据库
    free(result);
    mysql_close(my);
    return 0;
}