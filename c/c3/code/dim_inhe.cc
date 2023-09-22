#include <iostream>
using namespace std;
#include <string>


class Person
{
	public:
		string _name;
};

class Student:virtual public Person
{
	public:
		int _num;
};
class Teacher:virtual public Person
{
	public:
		int _id;
};
class Assistant:public Student,public Teacher
{
	public:
		string _majorCourses;
};

void test()
{
	Assistant a;
	a._id = 1;
	a._num = 2;
	//a._name = "AMor";
	a.Student::_name = "AMor";
	a.Teacher::_name = "Nelo";
}
int main()
{
	test();
	return 0;
}
