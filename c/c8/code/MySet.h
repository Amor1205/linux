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
		typedef typename RBTree<K,K,SetKeyOfT>::iterator iterator;

		iterator begin()
		{
			return _t.begin();
		}
		iterator end()
		{
			return _t.end();
		}

		pair<iterator,bool> insert(const K& key)
		{
			return _t.Insert(key);
		}
		iterator find(const K& key)
		{
			return _t.Find(key);
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
		set<int>::iterator it = s.begin();
		while(it != s.end())
		{
			cout << *it << " ";
			++it;
		}
		cout << endl;
	
	}

}
