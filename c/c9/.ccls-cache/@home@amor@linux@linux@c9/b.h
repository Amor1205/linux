#pragma once 
#include<vector>
#include<iostream>
using namespace std;

namespace CloseHash
{

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
	Status _status = EMPTY;
};

template<class K>
struct Hash
{
	size_t operator()(const K& key)
	{
		return key;
	}
};

struct HashStr
{
	size_t operator()(const string& s)
	{
		size_t value = 0;
		for(auto ch : s)
		{
			value += ch;
		}
		return value;
	}
};

template <class K, class V, class HashFunc = Hash<K>>
class HashTable
{
public:
	HashData<K,V>* Find(const K& key)
	{
		if(_tables.size() == 0)
		{
			return nullptr;
		}
		HashFunc hf;
		size_t start = hf(key) % _tables.size();
		size_t i = 0;
		size_t index = start;
		while(_tables[index]._status == EXIST)
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
		HashData<K,V>* ret = Find(key);
		if(ret == nullptr)
		{
			return false;
		}
		else
		{
			-- _n;
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
		{
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
			HashTable<K,V,HashFunc> newHashTable;
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
		HashFunc hf;
		size_t start = hf(kv.first) % _tables.size();
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
/*
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
*/
void TestHashTable2()
{
	HashTable<string,string,HashStr> ht;
	ht.Insert(make_pair("1","2"));
	ht.Insert(make_pair("hello","world"));

}

}//CloseHash
 //
 
namespace LinkHash
{
	template<class K,class V>
	struct HashNode
	{
		pair<K,V> _kv;
		HashNode<K,V>* _next = nullptr;

		HashNode(const pair<K,V>& kv)
			:_kv(kv),_next(nullptr)
		{}
	};
		
	template<class K>
	struct Hash
	{
		size_t operator()(const K& key)
		{
			return key;
		}
	};

	template<class K,class V,class HashFunc = Hash<K>>
	class HashTable
	{
	public:
		typedef HashNode<K,V> Node;

		Node* Find(const K& key)
		{
			if(_tables.empty())
			{
				return nullptr;
			}
			HashFunc hf;
			size_t index = hf(key) % _tables.size();
			Node* cur = _tables[index];
			while(cur)
			{
				if(cur->_kv.first == key)
				{
					return cur;
				}
				else
				{
					cur = cur->_next;
				}
			}		
			return nullptr;
		}
		bool Insert(const pair<K,V>& kv)
		{
			Node* ret = Find(kv.first);
			HashFunc hf;
			if(ret)
			{
				return false;
			}
			if(_n == _tables.size())
			{
				size_t newSize = _tables.size() == 0 ? 10 : 2*_tables.size();
				
				vector<Node*> newTables;
				newTables.resize(newSize);
				//traverse 
				for(size_t i = 0; i < _tables.size(); ++i)
				{
					Node* cur = _tables[i];
					while(cur)
					{
						Node* next = cur->_next;
						size_t index = hf(cur->_kv.first) % newTables.size();

						cur->_next = newTables[index];
						newTables[index] = cur;
					}
					_tables[i] = nullptr;
				}
				_tables.swap(newTables);
			}
			size_t index = hf(kv.first) % _tables.size();
			//toucha ]
			Node* newnode = new Node(kv);
			newnode->_next = _tables[index];
			_tables[index] = newnode;

			++_n;
			return true;
		}
	private:
		vector<Node*> _tables;
		size_t _n = 0;
	};

	void TestHashTable()
	{
		int a[] = {4, 24, 14 ,7, 37};
		HashTable<int,int> ht;
		for(auto e : a)
		{
			ht.Insert(make_pair(e,e));
		}
		cout << ht.Find(4);
	}
}

