#include<iostream>
#include<cstring>
#include<algorithm>
#include<utility>
#include<assert.h>
namespace Amor{
    class string{
    private:
        char* str_;

    public:
    /* construction */
    string();
    string(const char* str);
    string(const string& s);
    string(string&& s);

    /* destruction */
    ~string();

    /* assignment / operator */
    string& operator=(const string& s);
    string& operator=(string&& s);
    char& operator[](size_t pos);
    const char& operator[](size_t pos) const;
    string& operator=(string s);

    };


    string::string()
        :str_(new char[1])
    {
        str_[0] = '\0';
    }
    string::string(const char* str)
    {
        str_ = new char[strlen(str) + 1];
        strcpy(str_,str);
    }
    string::string(const string& s)
        :str_(nullptr)
    {
        string tmp(s.str_);
        swap(str_,tmp.str_);
    }
    template<class T>
    void swap(T& a, T& b){
        T tmp(a);
        a = b;
        b = tmp;
    }
    /* 移动构造 */
    string::string(string&& s)
        :str_(nullptr)
    {
        swap(str_,s.str_);
    }
    /* 移动赋值 */
    string& string::operator=(string&& s)
    {
        if(this != &s)
        {
            string tmp(s.str_);
            swap(str_, tmp.str_);
        }
        return *this;
    }
    string& string::operator=(const string& s)
    {
        if(this != &s)
        {
            string tmp(s.str_);
            swap(str_, tmp.str_);
        }
        return *this;
    }
    //写法2:
    string& string::operator=(string s)
    {
        swap(str_,s.str_);
        return *this;
    }
    //[]
    char& string::operator[](size_t pos)
    {
        assert(pos >= 0);
        return str_[pos];
    }
    const char& string::operator[](size_t pos) const
    {
        assert(pos >= 0);
        return str_[pos];
    }
    string::~string()
    {
        delete[] str_;
        str_ = nullptr;
    }
};