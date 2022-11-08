#include <iostream>
using namespace std;

class Person{
	public:
		virtual void BuyTicket(){cout << "full ticket" << endl;}
	protected:
		int _a;
};
class Student: public Person{
	public:
		virtual void BuyTicket(){cout << "half ticket" << endl;}
	protected:
		int _b;
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

/*class Base{
public:
	virtual void Func1(){
		cout << "Func1()" << endl;
	}
private:
	int _b = 1;
	char _ch = 'a';
};*/
class Base{
	public: 
		virtual void func1(){cout << "Base::func1()"<< endl;}
		virtual void func2(){cout << "Base::func2()"<< endl;}
	private:
		int a = -1;
};
class Derive: public Base{
	public: 
		virtual void func1(){cout << "Derive::func1()"<< endl;}
		virtual void func3(){cout << "Derive::func3()"<< endl;}
		void func4(){cout << "Derive::func4()" << endl;}
	private:
		int b = -2;
};

/*
int main()
{
//	Person ps;
//	Student st;  
//	Func(ps);
//	Func(st);	
//	A aa = A::create_obj(10);	

//	cout << sizeof(Base)  << endl;

	Person Mike;
	Func(Mike);

	Student Johnson;
	Func(Johnson);

	return 0;
}

*/

typedef void(*VF_PTR)();
void PrintVFtable(VF_PTR* table)
{
	for(int i = 0; i<= 4; ++i)
	{
		printf("vft[%d]:%p",i,table[i]);
		//VF_PTR f = table[i];
		//f();
	}
}


int main(){
	Base b;
	PrintVFtable((VF_PTR*)(*(int*)&b));

	Derive d;
	return 0;
}
