#include <iostream>
using namespace std;
#include <string>
#include <assert.h>

/*template<>
class Date< int , char >
{
	public:
		Date(){cout << "Date<int,char>" << endl;}
	private:

};


template<class T>
class Date< T, char > 
{
	public:
		Date(){cout << "Date<T, char>" << endl;}
	private:
};

tempalte<class T1*, class T2*>
class Date<T1*,T2*>
{
public:
	Date(){cout << "Date<T1*,T2*>" << endl;}
private:
};

int main()
{
	string str;
	while(cin>>str){
		cout<< str << endl;
	}
	return 0;
}

*/
struct Info
{
public:
	char _ip[32]; 
	int _port;
};
void TestC_w_bin()
{
	Info info = {"100.0.0.1", 80};
	
	FILE* fout = fopen("test.bin", "w");
	assert(fout);
	
	fwrite(&info,sizeof(info),1,fout);
	fclose(fout);
}
void TestC_r_bin()
{
	FILE* fin = fopen("test.bin","r");
	assert(fin);

	Info info;
	fread(&info,sizeof(info),1,fin);
	fclose(fin);

	cout<< info._ip <<" " << info._port <<endl;
}

void Test_r_text()
{
	FILE* fin = fopen("test.txt","r");
	assert(fin);

	Info info;
	fscanf(fin,"%s %d",info._ip,&info._port);
	fclose(fin);

	cout << info._ip << " " << info._port << endl;
}
void Test_w_text()
{
	FILE* fout = fopen("test.txt","w");
	assert(fout);

	Info info = {"100.0.0.1", 100};
	fprintf(fout,"%s %d",info._ip,info._port);
	fclose(fout);
}
int main()
{
	Test_w_text();
	Test_r_text();
	return 0;
}
