#include <iostream>
#include<set>
#include<map>
#include<string>
using namespace std;
void test_set1(){
	set<int> s;
	s.insert(3);
	s.insert(2);
	s.insert(5);
	s.insert(8);
	s.insert(1);
	s.insert(7);
	s.insert(4);
	
	set<int>::iterator pos = s.find(3);
	if(pos != s.end()){
		s.erase(pos);
	}
	set<int>::iterator it = s.begin();
	while(it!= s.end()){
		cout<< *it << ' ';
		++it;
	}
}
void test_multiset1(){
	multiset<int> ms;
	ms.insert(1);	
	ms.insert(7);
	ms.insert(5);
	ms.insert(1);
	ms.insert(3);
	ms.insert(1);
	ms.insert(2);
	
	multiset<int>::iterator pos = ms.find(1);
/*	if(pos!=ms.end()){
		ms.erase(pos);
	}
	pos = ms.find(1);
	while(pos!=ms.end() && *pos == 1){
		auto next = pos;
		++ next;

		ms.erase(pos);
		
		pos = next;
	}*/
	ms.erase(1);
	multiset<int>::iterator it = ms.begin();
	while(it!=ms.end()){
		cout << *it << ' ';
		++it;
	}

}

void test_map1(){
	map<string,string> dict;
	pair<string,string> kv1("sort","paixu");
	dict.insert(kv1);
	dict.insert(pair<string,string> ("hello","nihao"));
	dict.insert(make_pair("test","ceshi"));

	//for(auto e : dict){
	//	cout << e << endl;
	//}
	map<string,string>::iterator it = dict.begin();
	while(it != dict.end()){
//		cout << (*it).first << ":" << (*it).second << " ";
		cout << it->first << ":" << it->second << " ";
		++it;
	}
	cout << endl;

	for(auto& kv : dict){
		cout << kv.first << ":" << kv.second << " ";
	}
}
void test_map2(){
	string arr[] = {"1","2","1","3"};
	map<string,int> countM;
/*	for(auto& str : arr){
		auto ret = countM.find(str);
		if(ret != countM.end()){
			ret->second++;
		}
		else{
			countM.insert(make_pair(str,1));
		}
	}
*/
	for(auto& str : arr){
		auto kv = countM.insert(make_pair(str,1));
		if(kv.second == false){
			kv.first->second++;
		}
	}
	for(auto& kv : countM){
		cout << kv.first << ":" << kv.second << " ";

	}
	cout << endl;
		

}

int main(){
	//test_set1();
	//test_multiset1();
	test_map2();
	return 0;
}
