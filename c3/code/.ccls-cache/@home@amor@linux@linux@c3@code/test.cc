#include <iostream>
using namespace std;
#include<string>


class A
{
	void func()
	{
		cout << "func()" << endl;
	}
};

class B:public A
{
	public:
		void func(int i )
		{
			cout << "void func(int i)" <<endl;
		}
};

void test()
{
	B b;
	b.func(10);
}

class Person
{
public:
	Person(const char* name)
		:_name(name)
	{
		cout << "Person(const char* name)" << endl;
	}
	~Person()
	{
		cout << "~Person()" <<endl;
	}
	Person(const Person& p)
		: _name(p._name)
	{
		cout<< "Person(const Person& p)" << endl;
	}
	Person& operator=(const Person& p)
	{
		cout << "Person operator=(const Person& p)" <<endl;
		if(this != &p)
		{
			_name = p._name;
		}
		return *this;
	}

	
	string _name;
};
class Student:public Person
{
public:
	Student(const char* name = "Mary", int num = 1)
		:Person(name)
		,_num(num)
	{}

	//s2(s1)
	Student(const Student& s)
		:Person(s)//slice
		,_num(s._num)
	{}
	
	//s2=s1
	Student& operator=(const Student& s)
	{
		if(this!=&s)
		{
			Person::operator=(s);
			_num = s._num;
		}
		return *this;
	}

	~Student()
	{
		//Person::~Person();
	//do not explicit call the destructor of base class.
	//will call at the end.
	//In order to ensure that the subclass object is destructed first, and then the superclass object is destructed 
		//we need to add Person::
		//this is because the name of destructor(~Person) will be unified to "destructor()"
		//So the destructors of subclass & superclass constitute hidden.
		//Use Scope: Person::
	}
protected:
	int _num;
};


int main()
{
	//Student s1;
	//Student s2(s1);
	//Student s3;
	//s3 = s1;
	return 0;
}
