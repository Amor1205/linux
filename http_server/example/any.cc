#include <iostream>
#include <typeinfo>
#include <cassert>
#include <string>
#include <unistd.h>
class Any{
private:
    class holder{
    public:
        virtual ~holder(){}
        virtual const std::type_info& type() = 0;
        virtual holder* clone() = 0;
    };
    template<class T>
    class placeholder : public holder{
    public:
        placeholder(const T& val) :_val(val){}

        //获取子类对象保存的数据类型
        virtual const std::type_info& type() {return typeid(T);} 

        //针对当前的对象自身，克隆出一个新的子类对象
        virtual holder* clone() {return new placeholder(_val);}
    public:
        T _val;
    };

    holder* _content;
public:
    Any() :_content(nullptr){}
    //任意类型的构造
    template<class T>
    Any(const T& val) :_content(new placeholder<T>(val)) {}

    Any(const Any& other) 
    :_content(other._content ? other._content->clone() : nullptr){}

    ~Any() {delete _content;}
    Any& swap(Any& other){
        std::swap(_content, other._content);
        return *this;
    }
    //获取/返回 子类对象所保存的数据的指针
    template<class T>
    T* get() {
        //想要获取的数据类型必须要和保存的数据类型一致
        assert(typeid(T) == _content->type());
        // if(typeid(T) != _content->type()) return nullptr;

        //content是父类指针，我们需要强转成子类指针才可以访问_val
        return &((placeholder<T>*)_content)->_val;
    }

    //重载等号运算符
    template<class T>
    Any& operator=(const T& val){
        //为val 构造一个临时的通用容器，然后与当前容器自身进行一个指针的交换。
        //临时对象释放的时候，原先保存的数据也就被释放了。
        Any(val).swap(*this);
        return *this;
    }
    //可能赋值一个其他的通用容器
    Any& operator=(const Any& other){
        Any(other).swap(*this);
        return *this;
    }
};

//-----------------------------TEST-----------------------------TEST-----------
class Test{
public:
    Test(){std::cout << " Test() " << std::endl;}
    Test(const Test& other){std::cout << " Test(const Test& other) " << std::endl;}
    ~Test(){std::cout << " ~Test() " << std::endl;}
};
int main()
{
    Any a;
    {
        Test t;
        a = t;
    }

    // Any a;
    // a = 10;
    // int* pa = a.get<int>();
    // std::cout << *pa << std::endl;
    // a = std::string("hello");
    // std::string* ps = a.get<std::string>();
    // std::cout << *ps << std::endl;
    while (1) sleep(1);
    return 0;
}