template & Io stream
----------------------------

*full specialization:*

template<>
class Date< int , char >
{
	public:
		Date(){cout << "Date<int,char>" << endl;}
	private:

};

*partial specialization:* just limitation for the parameters.

example 1:

template<class T>
class Date< T, char > 
{
	public:
		Date(){cout << "Date<T, char>" << endl;}
	private:
};

example 2:

tempalte<class T1*, class T2*>
class Date<T1*,T2*>
{
public:
	Date(){cout << "Date<T1*,T2*>" << endl;}
private:
;

ps: non-type template parameters can be specialized.

-------------------------------
declaration and defination seperation:  --> easy maintenance
read xx.h file can understand the framework and basic fuctioons.
read xx.cpp file can know how to realize it.

see the code below:

tempalte<class T>
void F(const T& n)
{
	cout << "void F(const T& n)" << endl;
}
int main()
{
	F(0);
	return 0;
}

ERROR!!!
*template is not instantiated*, it cannot be compiled.
In the process2 (compiling/translator-editor), it can call the uncertain address of the function.(Because it has declaration) not specific address, just address of declaration.
However,in process4 (link), the address still cannot be found. So there is an error.

Solution:
1. explicit instantiation:
*template
void F<int>(const int& n);*
shortcoming: When using many types, we need to instantiate each type.
2. declaration and defination are not separated: 
when using, there are template defination and instantation after header file expansion. can fill in the address directly in process2.

----
1.preprocessing: header file expension\ conditional compilation\ macro substitution\ uncomment
.h -> .i
2.translator-editor (compile): check the syntax problems, if there is no problem, generate assembly code.
.i -> .s (assembly code)   (++n  mov eax[n] ; add eax 1 ; mov[n] eax ;)
3.compilation: convert assembly code to binary machine code.
.s -> .o
4.link.
Before linking, the declaration and defination will not interact. When linking, they interact and generat

*template summary:*
1.template reuses code, saves resources, and enables faster iterative development.
2.strengthen the flexibility of code.
shortcoming:
1.result in code bloat and compile time becomes longer.(instantise different types --> executable program bigger)
2.When a template compilation error occurs, the error message is very messy and it is not easy to locate the error.

----

iostream
C: scanf & printf(only can use bulit-in types)
C++: istream & ostream
for bulit-in types, we can use cout<< & cin>>
but for custom types, use operator overloading.
continuous: cout << i << d;  because: 2 operator function << transfer(use return value to realize).

Remember:
using cin & scanf, 'space' & '\n' are spacer.
so you cannot cin>>"hello world";
it is just "hello" string, you need to use getline().

multiple input:
c: while(scanf(...) != EOF), use ctrl+z to end
c++: while(cin >> str), use ctrl+z to end the program. cin will make implicit type conversion, and become bool type.
If this is a custom type, we need to define operator bool
e.g.,
class A
{
public:
	operator bool()
	{
		return _a != o;
	}


	int _a;
}

and we can use:

int main()
{
	A a;
	while(a)
	{
		cin >> a._a;
	}
	return 0;
}


about file: binary read and write:
fwrite / fread

void TestC_w_bin()
{
	Info info = {"100.0.0.1", 80};
	
	FILE* fout = fopen("test.bin", "wb");
	assert(fout);
	
	fwrite(&info,sizeof(info),1,fout);
	fclose(fout);
}
void TestC_r_bin()
{
	FILE* fin = fopen("test.bin","rb");
	assert(fin);

	Info info;
	fread(&info,sizeof(info),1,fin);
	fclose(fin);

	cout<< info._ip <<" " << info._port <<endl;
}
text read and write:
fprintf / fscanf

void TestC_r_text()
{
	FILE* fin = fopen("test.txt","r");
	assert(fin);

	Info info;
	fscanf(fin,"%s %d",info._ip,&info._port);
	fclose(fin);

	cout << info._ip << " " << info._port << endl;
}
void TestC_w_text()
{
	FILE* fout = fopen("test.txt","w");
	assert(fout);

	Info info = {"100.0.0.1", 100};
	fprintf(fout,"%s %d",info._ip,info._port);
	fclose(fout);
}

C++ write & read in Binary & Text:

class ConfigManager
{
	public:
		ConfigManager(const char* filename)
			:_filename(filename)
		{}
	void WriteBin(const Info& info)
	{
		ofstream ofs(_filename.c_str(), ios_base::out | ios_base::binary);
		ofs.write((const char*)&info,sizeof(Info));
	}
	void ReadBin(Info& info)
	{
		ifstream ifs(_filename.c_str(),ios_base::in | ios_base::binary);
		ifs.read((char*)&info,sizeof(Info));
	}
	void WriteText(const Info& info)
	{
		ofstream ofs(_filename.c_str());

		//support << stream insert
		ofs << info._ip <<" " <<info._port<<" "<<info._d;

	}
	void ReadText(Info& info)
	{
		ifstream ifs(_filename.c_str());
		//support >> stream getting
		ifs >> info._ip >>info._port;
		cout << info._ip <<"|"<< info._port <<endl;
	}
	private:
		string _filename;
};



inheritance:
1. what is inheritance ?
*inheritance is the most important method to make the code reuse in object oriented programming.*
It allow programmer to extend on the basis of ensuring the characteristics of the original class, and can add features and functions.
This classes are called derived classes.

For example
define a class for student : its member variables:
string name;
string id;
int age;
string address;
string contact;
etc,
define a class for teacher : its member variables:
etc.
You will find there are many same variables between them. ---> produce redundancy

-----> inheritance;
parent / base class   &  child / derived class 
*inheritance is the reuse of class design level*

grammar:
the way of defination :
class + [derived class]: [the way of inheritance][base class]

			 -> public inheritance
the way of inheritance : -> protected inheritance
			 -> private inheritance 

So there are 9 ways to inherit:

base member      |        public inheritance 	     | protected inheritance | private inheritance |
public member    |  public member in derived class   | protected member      | 	 private member    |
protected member |  protected member in derived class| protected member      |   private member    |
private member   |  	invisible                    | 	invisible            | 	invisible          |

"invisible" means we cannot assess it in (or out of) class
in base class : 
we want derived class to assess while other cannot assess :-> protected.
we want all can assess :-> public.

Conclusion:
always public inheritance. almost rarely protected|private inheritance;
So :
 base members use public & protected.
 inheritance way : public. 
By default : private inheritance(base is public), public inheritance(base is struct)

AND : 
for derived members, they can pass their value & address & reference to their base member.
like:      child -> parent
{
	Person p;
	Student s;
	
	p = s;
	Person* ptr = &s;
	Person& ref = s;
} 
It is called "assignment compatible"(or slice) (only public inheritance).
there is no type conversion. (type conversion will generate temporary variable, must add const before s like this: const Person& ref = s)

But : parent -------xxxxxxx---> child . can't do it.
exception:
{
	//pointer:
	Student* pptr = (Student*)&p;
	//reference:
	Student& rref = (Student&)p;
	
	//need coercion.
	// risk of crossing the border.  ----very dangerous!
}


