#pragma once
#include "RBTREE.h"
namespace AMor
{
	template< class K>
	class set
	{
	public:
		struct SetKeyOfT
		{
			const K& operator()(const K& k)
			{
				return k;
			}
		};
		bool insert(const K& key)
		{
			return _t.Insert(key);
		}
	private:
		RBTree<K,K,SetKeyOfT> _t;

	};

	void test_set()
	{
		set<int> s;
		s.insert(1);
		s.insert(15);
		s.insert(13);
		s.insert(7);
		s.insert(3);

	}
}
