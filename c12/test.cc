#include"singleton.h"

class Exception
{
public:
    Exception(const string& errmsg, int id)
        :_errmsg(errmsg)
        ,_id(id)
        {}
    const string& what() const
    {
        return _errmsg;
    }
protected:
    string _errmsg;
    int _id;
};

class func1_exception : public Exception
{

};
class func2_exception : public Exception
{

};
class func3_exception : public Exception
{
public:
    func3_exception(const string& errmsg, int id, const string& type)
        :Exception(errmsg,id),_type(type)
    {    
    }

private:
    const string& _type;
};
void Func1()
{
    
}
void Func2()
{
    Func1();
}
void Func3()
{
    if(rand()%2 == 0)
    {
        throw func3_exception("connection error", 100, "get");
    }
    else if (rand()%7 == 0)
    {
        throw func3_exception("permission error", 200, "post");
    }
    Func2();
}
void ServerStart()
{
    srand(time(0));
    int n = 100;
    while(n--)
    {
        try{
            Func3();
        }
        catch(const Exception& e)
        {
            cout << e.what() << endl;
        }
        catch(...)
        {
            cout << "unkown Exception" << endl;
        }
    }

}
// int main()
// {
//     ServerStart();

// }


// int main()
// {
//     vector<int> v(10,5);
//     // v.reserve(1000000000000);
//     v.at(10) = 0;
//     return 0;
// }

// int main()
// {
//     const int a = 2;
//     int* p = const_cast<int*>(&a);
//     *p = 3;
//     cout << a << endl;
//     cout << *p; 
// }


class A
{
public: 
    virtual void func(){}
};
class B: public A
{
public:
    int _b = 0;
};

void func(A* pa){
        B* pb1 = dynamic_cast<B*>(pa);
        if (pb1 == nullptr)
        {
            cout << "FATHER"<< endl;
        }
        else
        {
            cout << "DERIVED" << endl;
        }
}

int main()
{
    A aa;
    B bb;
    //func(&aa);
    func(&bb);
    return 0;
}