inheritance:
reuse of the class level.

child -> parent : slice and no type conversion.
base member = derived member assignment compatible.

Scope in inheritance:
The principle of proximity.
Subclass and superclass have members with the same name.
For example:

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

void test1()
{
	B b;
	b.func(10);
}

---> constructors of A & B hide.   NOT overloading!
----> because the 2 func() in different scope.
-----> whether parameters same or not, always hide.

void test2()
{
	B b;
	b.func();
}

---> compile error.
----> func in A is hidden so need specified scope.

default member function in derived class:
very important 4 member function : constructor, destructor, assign, copy constructor.
1. If we do not write, translater will generate by default.
	a. for constructor & destructor: --base member: use con/de-structors to deal with.
					 --derived member: for bulit-in type, use default generator function. for custom type, do not deal with (just like normal class)
	b. for copy constructor & operator= :
					 --same as the above.

2. If we want to write, how & when to do?
When:
1. If base class has no default constructor, we need to write on our own.
2. If derived class has resources needing to release, we need to write destructor.
3. If Subclasses have shallow copy problems, we need to write copy constructor & operator=.
How:
base members use corresponding constructor & destructor, operator= & copy constructor in the base class.
subclass members are handled on demand.(just like normal class)

friendships cannot be inherited.

static member in base class:
will be inherited, but there is only one this member in the program.(if changed anywhere, the member will be changed.)


Classification of inheritance:
single inheritance: a subclass only has one direct superclass
class Person
class Student: public Person
class Graduate: public Student

multiple inheritance: a subclass has two or more direct superclasses
class Student    class Teacher
class Assistant: public Student, public Teacher

diamond inheritance: a special situation in multiple inheritance

			class Person

class Student:public Person    class Teacher:public Person

	class Assistant: public Student, public Teacher

RESULTING IN : Data Redundancy and Ambiguity


amor@amor-win:~/linux/linux/c3/code$ g++ -o dim dim_inhe.cc
dim_inhe.cc: In function ‘void test()’:
dim_inhe.cc:33:11: error: request for member ‘_name’ is ambiguous
   33 |         a._name = "AMor";
      |           ^~~~~
dim_inhe.cc:9:24: note: candidates are: ‘std::string Person::_name’
    9 |                 string _name;
      |                        ^~~~~
dim_inhe.cc:9:24: note:                 ‘std::string Person::_name’*

*we can handle the problem of ambiguity , but how to handle the data Redundancy?*
 
Use "virtual" keywords:


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

USE virtual, Person becomes a virtual base class.
In Assistant, it puts "Person" in a public location. So sometime Student or Teacher wants to modify or access "Person", 
need to calculate through the offset in the virtual base table to find "Person".
virtual base table is stored in the location where "Person" was originally stored.(no "virtual" decoration)
now this location is an address, pointing to the virtual base table.
try not to define diamond inheritance

C++:
1. No garbage collection.
2. Diamond inheritance.

Inheritance & composition:
1. Inheritance:
class A
{
	int _a;
};
class B: public A
{
	int _b;
};

2. composition:
class C
{
	int _c;
};
class D
{
	C c;
	int _d;
};

They both can reuse other classes.

DIfference:
1. public-inheritance is a "is-a" relationship. That is to say every derived class object is a base-class object.
2. Composition is a "has-a" relationship. If B composites with A, every B object has a A object.

For example: Students are a special type of people.(use inheritance)
	     Car has tires.(use composition)
If compatible with both at the same time, use composition.

Public inheritance : black-box reuse.
Compostion : white-box reuse.
inheritance destroys the package to a certain extent. (B can access protected & public members)
But D can only use the public members of C, cannot access protected members.

The relationship between classes & modules is preferably low coupling & high cohesion. -->easy maintenance.



