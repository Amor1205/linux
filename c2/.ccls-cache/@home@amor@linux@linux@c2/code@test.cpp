#include <iostream>
using namespace std;
#include <string>
#include <assert.h>
#include <fstream>
#include <sstream>

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

struct Info
{
public:
	char _ip[32]; 
	int _port;
	Date _d;
};

class Date
{

	friend ofstream& operator<< (ofstream& ofs, const Date& d)
	friend ifstream& operator>> (ifstream& ofs, Date& d)
	public:
		Date(int year=1970,int month=1,int date=1)
			:_year(year),_month(month),_date(date)
		{}
	private:
		int _year;
		int _month;
		int _date;
};

ofstream& operator<< (ofstream& ofs, const Date& d)
{
	ofs<<d._year << " " << d._month << " " << d._date << endl;
	return ofs;
}
ifstream& operator>> (ifstream& ifs, Date& d)
{
	ifs>>
}
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

int main()
{
	//TestC_w_text();
	//TestC_r_text();
	//Info info = {"100.0.0.1",100};
	Info info;
	//ConfigManager cm("config.bin");
	ConfigManager cm("config.txt");
	//cm.WriteBin(info);
	//cm.ReadBin(info);
	//cm.WriteText(info);
	cm.ReadText(info);
	//cout<< info._ip << " " << info._port << endl;

	return 0;
}

*/
struct PersonInfo
{
	string _name;
	int _age;

};

int main()
{
	PersonInfo info = {"AMor",20};
	ostringstream oss;
	oss << info._name <<" "<< info._age;
	string str = oss.str();
	//cout << str;
	
	istringstream iss(str);
	string name;
	int age;
	iss >> name >> age;
	cout << name << " "<< age << endl;
	
	return 0;
}
