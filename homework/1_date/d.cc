#include <iostream>
using namespace std;

class Date
{
public:
	
	int get_month_day(const int year,const int month)const
	{
		static int month_day[13] = {31,31,28,31,30,31,30,31,31,30,31,30,31};
		if(2 == month)
		{
			if((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
			{
				return month_day[month] + 1;
			}
		}
		return month_day[month];
	}
	bool is_fair(const int year,const int month,const int day) const
	{
		if( month > 0 && month < 13 && day > 0 && day <= get_month_day(year,month) )
			return true;
		return false;
	}
	
	//constructor
	Date(int year= 1970,int month = 1,int day = 1)
		:_year(year),_month(month),_day(day)
	{
		if(!is_fair(year,month,day))
		{
			cout << "invalid day" << endl;
		}
	}
	//destructor
	~Date(){}
	//Copy constructor: use default
	// = 
	Date& operator=(const Date& d)
	{
		_year = d._year;
		_month = d._month;
		_day = d._day;
		return *this;
	}
	void Print()
	{
		cout << _year << ":" << _month << ":" << _day << endl;
	}

 // 日期+=天数

Date& operator+=(int day){
	_day += day;
	while(!is_fair(_year,_month,_day))
	{
		_day -= get_month_day(_year,_month);
		if(_month == 12){
			_month = 1;
			++_year;
		}
		else{
			++_month;
		}
	}
	return *this;
}

  // 日期+天数

Date operator+(int day){
	Date d(*this);
	d+=day;
	return d;
}

  // 日期-天数

Date& operator-=(int day){
	_day -= day;
	while(!is_fair(_year,_month,_day))
	{
		_day += get_month_day(_year,_month-1);
		--_month ;
		if(0 == _month)
		{
			_month = 12;
			--_year;
		}	
	}
	return *this; 
}

   // 日期-=天数

Date operator-(int day){
	Date d(*this);
	d -= day;
	return d;
}


  // 前置++

Date& operator++(){
	*this += 1;
	return *this;
}

  // 后置++

Date operator++(int){
	Date d(*this);
	++*this;
	return d;
}

  // 后置--

Date operator--(int){
	Date d(*this);
	*this -= 1;
	return d;
}

  // 前置--

Date& operator--(){
	*this -= 1;
	return *this;
}


  // >运算符重载

bool operator>(const Date& d){
	if(_year < d._year)
		return false;
	if(_year == d._year && _month < d._month)
		return false;
	if(_year == d._year && _month == d._month && _day <= d._day)
		return false;
	return true;
}


  // ==运算符重载

bool operator==(const Date& d){
	if(_year == d._year && _month == d._month && _day == d._day)
		return true;
	return false;
}

  // >=运算符重载

bool operator >= (const Date& d){
	return (*this>d || *this == d);
}
   

  // <运算符重载

bool operator < (const Date& d){
	return !(*this >= d);
}

   // <=运算符重载

bool operator <= (const Date& d){
	return !(*this > d);
}

  // !=运算符重载

bool operator != (const Date& d){
	return !(*this == d);
}

  // 日期-日期 返回天数

int operator-(const Date& d){
	int val = 0;
	Date bigD(*this);
	Date SmallD(d);
	if(bigD < SmallD)
	{
		swap(bigD,SmallD);
	}
	while(bigD > SmallD)
	{
		--bigD;
		++val;
	}
	return val;
}
private:
	int _year;
	int _month;
	int _day;

};


int main()
{
	Date d1(2000,2,29);
	Date d2(2002,1,29);
	cout << (d2-d1) << endl;
	//d1.Print();
	//
	return 0;
}
