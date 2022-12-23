#pragma once
#include"hashbucket.h"

namespace Amor
{
    template<class K, class Hash = LinkHash::Hash<K> >
    class unordered_set
    {
    public:
        struct SetKeyOfT;

        typedef typename LinkHash::HashTable<K,K,SetKeyOfT,Hash>::iterator iterator;

        iterator begin()
        {
            return _ht.begin();
        }
        iterator end()
        {
            return _ht.end();
        }
        struct SetKeyOfT
        {
            const K& operator()(const K& key)
            {
                return key;
            }
        };
        bool insert(const K& key)
        {
            return _ht.Insert(key);
        }
    private:
        LinkHash::HashTable<K,K, SetKeyOfT, Hash> _ht;
    };

    void Test_unordered_set()
    {
        unordered_set<int> us;
        us.insert(1);
        us.insert(5);
        us.insert(3);
        us.insert(2);
        us.insert(9);

        unordered_set<int>::iterator it = us.begin();
        while(it != us.end())
        {
            cout << *it << " ";
            ++it;
        }
        cout << endl;
        
    }
}