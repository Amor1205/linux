#include <iostream>
using namespace std;

class Person{
	public:
		virtual void BuyTicket(){cout << "full ticket" << endl;}
};
class Student: public Person{
	public:
		virtual void BuyTicket(){cout << "half ticket" << endl;}
};
void Func(Person& p)
{
	p.BuyTicket();
}

class A 
{
private:
	A(int a = 0)
		:_a(a){}
public:
	static A create_obj(int a = 0)
	{
		return A(a);
	}
protected:
	int _a;
};

class B: public A
{

};

class Car{
	public:
		virtual void Drive() = 0;

};

class Base{
public:
	virtual void Func1(){
		cout << "Func1()" << endl;
	}
private:
	int _b = 1;
	char _ch = 'a';
};

int main()
{
//	Person ps;
//	Student st;
//	Func(ps);
//	Func(st);	
//	A aa = A::create_obj(10);	

	cout << sizeof(Base)  << endl;

	return 0;
}
