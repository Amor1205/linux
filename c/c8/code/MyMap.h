#pragma once
#include "RBTREE.h"

namespace AMor
{
	template<class K, class V>
	class map
	{
	public:
		struct MapKeyOfT
		{
			const K& operator()(const pair<K,V>& kv)
			{
				return kv.first;
			}
		};

		typedef typename RBTree<K,pair<K,V>,MapKeyOfT>::iterator iterator;

		iterator begin()
		{
			return _t.begin();
		}
		iterator end()
		{
			return _t.end();
		}

		V& operator[](const K& key)
		{
			auto ret = _t.Insert(make_pair(key,V()));
			return ret.first->second;
		}

		pair<iterator,bool> insert(const pair<K,V>& kv)
		{
			return _t.Insert(kv);
		}
		iterator find(const K& key)
		{
			return _t.Find();
		}

	private:
		RBTree<K,pair<K,V>,MapKeyOfT> _t;		
	};

	void test_map()
	{
		map<int,int> m;
		m.insert(make_pair(3,3));
		m.insert(make_pair(2,5));
		m.insert(make_pair(9,7));
		m.insert(make_pair(4,6));
		m.insert(make_pair(16,16));
	}
	
	void test_map2()
	{
		map<string,string> dict;
		dict.insert(make_pair("sort","paixu"));
		dict.insert(make_pair("string","zifuchuan"));
		dict.insert(make_pair("map","ditu"));
		
		dict["left"];
		dict["right"];
		dict["map"]="ditu\\yingshe";
		auto it = dict.begin();
		while(it != dict.end())
		{
			cout << it->first << ":" << it->second << endl;
			++it;
		}
		
	}
}

