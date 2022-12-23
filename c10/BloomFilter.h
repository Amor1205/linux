#pragma once
#include<bitset>
#include<string>
using namespace std;

struct BKDRHash
{
	size_t operator()(const string& s)
	{
		// BKDR
		size_t value = 0;
		for (auto ch : s)
		{
			value *= 31;
			value += ch;
		}
		return value;
	}
};

struct APHash
{
	size_t operator()(const string& s)
	{
		size_t hash = 0;
		for (long i = 0; i < s.size(); i++)
		{
			if ((i & 1) == 0)
			{
				hash ^= ((hash << 7) ^ s[i] ^ (hash >> 3));
			}
			else
			{
				hash ^= (~((hash << 11) ^ s[i] ^ (hash >> 5)));
			}
		}
		return hash;
	}
};

struct DJBHash
{
	size_t operator()(const string& s)
	{
		size_t hash = 5381;
		for (auto ch : s)
		{
			hash += (hash << 5) + ch;
		}
		return hash;
	}
};

template<size_t N,
size_t X = 4, 
class HashFunc1 = BKDRHash,
class HashFunc2 = APHash,
class HashFunc3 = DJBHash,
class K = string>
class BloomFilter
{
public:
    void Set(const K& key)//标记为1
    {
        size_t len = X * N;
        size_t index1 = HashFunc1()(key) % len;
        size_t index2 = HashFunc2()(key) % len;
        size_t index3 = HashFunc3()(key) % len;

        _bs.set(index1);
        _bs.set(index2);
        _bs.set(index3);

    }
    bool Test(const K& key)
    {
        size_t len = X * N;
        size_t index1 = HashFunc1()(key) % len;
        if(_bs.test(index1) == false)
        {
            return false;
        }
        size_t index2 = HashFunc2()(key) % len;
        if(_bs.test(index2) == false)
        {
            return false;
        }
        size_t index3 = HashFunc3()(key) % len;
        if(_bs.test(index3) == false)
        {
            return false;
        }

        return true;//可能存在误判的

    }
private:
    bitset<X * N> _bs;
};

void TestBloomFilter()
{

}