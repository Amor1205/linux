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
		bool insert(const pair<K,V>& kv)
		{
			return _t.Insert(kv);
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
}

