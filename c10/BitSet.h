#pragma once
#include<vector>
#include<iostream>

namespace Amor
{
    template<size_t N>
    class bit_set
    {
    public:
        bit_set()
        {
            _bits.resize(N/8 + 1 , 0);
        }
        void set(size_t x)// turn into 1
        {
            size_t i = x / 8;
            size_t j = x % 8;

            _bits[i] |= (1 << j);

        }

        void reset(size_t x)// turn into 0
        {            
            size_t i = x / 8;
            size_t j = x % 8;
            _bits[i] &= (~(1 << j));
        }

        bool test(size_t x)// check 1 or 0
        {
            size_t i = x / 8;
            size_t j = x % 8;

            return (bool) (_bits[i] & (1 << j));            
        }
    private:
        std::vector<char> _bits;
    };

    void test_bit_set()
    {
        bit_set<100> bs;
        bs.set(5);
        bs.set(15);
        bs.set(25);

        bs.reset(25);
        std::cout << bs.test(5) << std::endl;
        std::cout << bs.test(6) << std::endl;
        std::cout << bs.test(15) << std::endl;
        std::cout << bs.test(25) << std::endl;
    }
    void test_bit_set2()
    {
        bit_set<0xffffffff> bs;
    }
    template<size_t N>
    class TwoBitSet
    {
    public:
        void Set(size_t x)
        {
            if(! (_bs1.test(x) || _bs2.test(x))) // 00 -> 01
            {
                _bs2.set(x);
            }
            else if( !(_bs1.test(x)) && _bs2.test(x)) // 01 -> 11
            {
                _bs1.set(x);
            }
        }   
        void PrintOnce()
        {
            for(size_t x = 0; x < N; ++x)
            {
                if(!(_bs1.test(x)) && _bs2.test(x))//_bs1 --> 0  _bs2 --> 1
                {
                        std::cout << x << " ";
                }
            }
        }
    private:
        bit_set<N> _bs1;
        bit_set<N> _bs2;
    };

    void TestTwoBitSet()
    {
        int a[] = {99,0,99,0,50};
        TwoBitSet<100> bs;
        for (auto e : a)
        {
            bs.Set(e);
        }
        bs.PrintOnce();
    }

}// Amor



