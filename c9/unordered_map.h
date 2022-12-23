#pragma once
#include"hashbucket.h"

namespace Amor
{
    template<class K, class V, class Hash = LinkHash::Hash<K> >
    class unordered_map
    {
        struct MapKeyOfT
        {
            const K& operator()(const pair<K,V>& kv)
            {
                return kv.first;
            }
        };
    private:
        LinkHash::HashTable<K, pair<K,V>, MapKeyOfT, Hash> _ht;
    };
}