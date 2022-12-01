#include <iostream>
#include<set>
#include<map>
#include<string>
#include<queue>
#include<functional>
#include<algorithm>
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
	/*for(auto& str : arr){
		auto kv = countM.insert(make_pair(str,1));
		if(kv.second == false){
			kv.first->second++;
		}
	}*/

	for(auto& str : arr){
		countM[str]++;
	}
	for(auto& kv : countM){
		cout << kv.first << ":" << kv.second << " ";

	}
	cout << endl;
		

}

void test_map3(){
	map<string,string> dict;
	dict.insert(make_pair("sort1","paixu1"));
	dict.insert(make_pair("sort2","paixu2"));
	dict.insert(make_pair("sort3","paixu3"));
	dict.insert(make_pair("sort4","paixu4"));
	dict.insert(make_pair("sort5","paixu5"));
	dict["sort1"] = "hello";
	dict["?"];
	cout << dict["sort1"] << endl;

}

struct CountVal{
	bool operator()(const pair<string,int>& l , const pair <string,int>& r){
		return l.second > r.second;
	}
};
void GetFavoruteFruit(const vector<string>& fruits, size_t k){
	map<string,int> countMap;
	for(auto& str: fruits){
		countMap[str]++;
	}

	vector<pair<string,int>> sortV;
	for(auto& kv : countMap){
		sortV.push_back(kv);
	}
	sort(sortV.begin(),sortV.end(),CountVal());

	for(int i = 0;i < k; ++i){
		cout<< sortV[i].first << ":" << sortV[i].second << endl;
	}
}

struct CountIterVal{
	bool operator()(const map<string,int>::iterator& l, const map<string,int>::iterator& r){
		return l->second > r->second;
	}
};
void GetFavoruteFruit(const vector<string>& fruits, size_t k){
	vector<map<string,int>::iterator> sortV;
	auto it = countMap.begin();
	while(it != countMap.end()){
		sortv.push_back(it);
		++it;
	}

	sort(sortV.begin(),sortV.end(),CountIterVal());
	for(int i = 0; i<k; ++i){
		cout<< sortV[i]->first << ":" << sortV[i].second << endl;
	}
	cout << endl;
}

void GetFavoruteFruit(const vector<string>& fruits, size_t k){
	multimap<int ,string,greater> sortMap;
	for(auto& kv : countMap){
		sortMap.insert(make_pair(kv.second,kv.first));
	}
}
void GetFavoruteFruit(const vector<string>& fruits, size_t k){
	priority_queue<pair<string,int>,vector<pair<string,int>>,CountVal> pq;
	for(auto& kv : countMap){
		pq.push(kv);
	}
	while(k--){
		cout << pq.top().first << endl;
		pq.pop();
	}
	

	priority_queue<map<string,int>::iterator, vector<map<string,int>::iterator>, CountIterVal> pq;
	auto it = countMap.begin();
	while(it != countMap.end()){
		pq.push(it);
		++it;
	}
}
int main(){
	//test_set1();
	//test_multiset1();
//	test_map3();

	vector<string> v = {"apple","apple","banana","apple","cherry","cantaloupe","cantaloupe"};
	GetFavoruteFruit(v,3);
	return 0;
}
