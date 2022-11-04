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

