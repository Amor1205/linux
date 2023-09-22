#include <iostream>

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



int main()
{
	
	return 0;
}
