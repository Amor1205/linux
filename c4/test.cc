#include<iostream>
using namespace std;
#include <string>

int main(){
	string a = "hello world";
	string b = a;
	cout << (a.c_str() == b.c_str()) << endl;
	cout << a.c_str() << endl;
	cout << b.c_str();
	

	cout << "for_update" << endl;
	return 0;
}
