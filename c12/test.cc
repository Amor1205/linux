#include<iostream>
#include<string>
using namespace std;
class Exception
{
public:
    Exception(const string& errmsg, int id)
        :_errmsg(errmsg)
        ,_id(id)
        {}
    const string& what()
    {
        return _errmsg;
    }
protected:
    string _errmsg;
    int _id;
};
void ServerStart()
{
    try{
    }
    catch()
    {

    }
    catch(...)
    {
        
    }
}
int main()
{
    ServerStart();

}
