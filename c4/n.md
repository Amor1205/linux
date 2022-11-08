polymorphism:

1.static polymorphism: function overloading.(done at compile time)
2.dynamic polymorphism: use a reference or pointer of an base object to call the same function, when passing different objects, will call different functions.(runtime implementation);
essence:  Different people do the same thing and the outcoming is different.
 

class Person{
	public:
		virtual void BuyTicket(){cout << "full ticket" << endl;}
};
class Student: public Person{
	public:
		virtual void BuyTicket(){cout << "half ticket" << endl;}
};

Subclasses satisfy "3 indentical"(function name & parameters & return value) & virtual functions, it's called "rewrite / covering"
override needs return values to be the same, but there is a exception: the return value is required to be a pointer or reference to the parent-child relationship(base-derived relationship)
 
2 condition:
	1.must use pointer or reference of base member to call virtual functions.
	2.The called virtual function must be a virtual function and the derived class must override(/rewrite) the virtual function of the base class.
Only non-static member functions can be virtual fucntions.

if destructors are virtual functions, also constitute a rewrite.
Although names are different, but they will be modified to "destructor".
For normal object, destructor is virtual or not, is rewritten or not,  the destructor is all called correctly.

But for object of dynamic application,if given to the parent class pointer management:->
destructor need to be virtual function.

the rewrite of vitual function allows :   both funtions in base class and derived class are all vitual or function in base class is virtual.

*keywords*:
final  :  
1.use "final" to let a class cannot be inherited.  e.g., class A final{};
2.use "final" to let a function cannot be rewriten. e.g., virtual void f() final

override : e.g., virtual void f() override
check if the virtual functions have been rewriten (if not, report an error).

overloading : same scope; function name / parameters  different. No limit for return value.
rewrite : 3 identical.
redefine: does not constitute rewriting is redefining.

*abstract class:* a class containing pure virtual functions.
virutal void f() = 0;  :-> pure virtual function.   pure virtual function can be only declared, do not be realized.  Becauese no worth.
feature: cannot instantiate an object.



class Base{
public:
	virtual void Func1(){
		cout << "Func1()" << endl;
	}
private:
	int _b = 1;
	char _ch = 'a';
};

sizeof(Base)  --> 12 (Win32)   because : vfptr=4 + int=4 + char=1(fill up = 4) = 12

vfptr : virtual function table pointer. pointing an array(function pointer array).

the vfptr in derived class is different from the one in base class (it's rewriting vft);
thinking that rewriting is covering.

*the principle of polymorphism: the pointer or reference of base class, pointing someone and find and use the corresponding function in its virtual function table.*
*remember : slice cannot change the vfptr. if use passing value, the vfptr will be changed.*

*same type of objects, use same vfptr, pointing same address/ same virtual table.*

normal functions and vitual functions are stored in the same places.
*all in the code snippet*. But vitual functions will put their address in the vitual tables in order to realize polymorphism.

if not the polymorphism: determine the address of a function at compile time.
if polymorphism: determine the address of a virtual function at runtime.

the virtual function table is stored in *constant area/code snippet* 


