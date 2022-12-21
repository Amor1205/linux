#pragma once 
#include<vector>
#include<iostream>
using namespace std;

enum Status
{
	EXIST,
	EMPTY,
	DELETE
};//mark

template<class K,class V>
struct HashData
{	
	pair<K,V> _kv;
	Status _status;
};

template <class K, class V>
class HashTable
{
public:
	HashData<K,V>* Find(const K& key)
	{
		size_t start = kv.first % _tables.size();
		size_t i = 0;
		size_t index = start;
		while(_tables[index]._status != EMPTY)
		{
			if(_tables[index]._kv.first == key)
			{
				return &_tables[index];
			}

			++i;
			index = start + i;

		}
		return nullptr;
	}
	bool Erase(const K& key)
	{
		HashData<K,V>* ret = Find(kv.first);
		if(ret == nullptr)
		{
			return false;
		}
		else
		{
			ret->_status = DELETE;
			return true;
		}
	}
	//linear detection
	/*bool Insert(const pair<K,V>& kv)
	{
		//size_t i = kv.first % _tables.capacity();
		size_t start = kv.first % _tables.size();
		size_t i = 0;
		size_t index = start;
		//we can only access to the _tables[size];
		while(_tables[index]._status == EXIST)
		{
			++i;
			index = start + i;
			index %= _tables.size();
		}

		_tables[index]._kv = kv;
		_tables[index]._status = EXIST;
		++ _n;
	}
	*/
	//quadratic detection
	bool Insert(const pair<K,V>& kv)
	{
		HashData<K,V>* ret = Find(kv.first);
		if(ret != nullptr)
		{
			return false;
		}
		
		//expansion
		if(_tables.size() == 0 ||_n*10 / _tables.size() >= 7)
		
			size_t new_size = _tables.size() == 0 ? 10 : _tables.size()*2;
		/*	vector<HashData<K,V>> newTables;
			newTables.resize(new_size);
			//traverse old table
			for(size_t i = 0; i < _tables.size(); ++i)
			{
				//traverse
			}{
			_talbes.swap(newTables);
		*/
			HashTable<K,V> newHashTable;
			newHashTable._tables.resize(new_size);
			for(size_t i = 0; i < _tables.size(); ++i)
			{
				if(_tables[i]._status == EXIST)
				{
					newHashTable.Insert(_tables[i]._kv);
				}
			}
			_tables.swap(newHashTable._tables);
		}
		//size_t i = kv.first % _tables.capacity();
		size_t start = kv.first % _tables.size();
		size_t i = 0;
		size_t index = start;
		//we can only access to the _tables[size];
		while(_tables[index]._status == EXIST)
		{
			++i;
			index = start + i*i;
			index %= _tables.size();
		}

		_tables[index]._kv = kv;
		_tables[index]._status = EXIST;
		++ _n;
		return true;
	}
private:
	vector<HashData<K,V>> _tables;
	size_t _n;//number of usful datas
};

void TestHashTable()
{
	HashTable<int,int> ht;
	int a[] = {2,1,72};
	for(auto e : a)
	{
		ht.Insert(make_pair(e,e));
	}
	cout << "1";
}
