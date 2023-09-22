#include<iostream>
#include<vector>

using namespace std;

class Heap
{
public:
    vector<int> _v;
    int _size; //堆中节点的个数
public:
    Heap(int size = 10)
    :_size(size)
    {
        _v.resize(_size);
        for(int i = 0; i < _size; ++i)
        {
            _v[i] = i;
        }
        for(int i = _size / 2 - 1; i >= 0; --i)
            adjustDown(i);
    }
    void adjustDown(int n) //n是父节点的位置
    {
        int lson = 2 * n + 1;
        int rson = 2 * n + 2;
        int maxi = n;
        if(lson < _size && _v[maxi] < _v[lson]) maxi = lson;
        if(rson < _size && _v[maxi] < _v[rson]) maxi = rson;
        if(maxi != n)
        {
            swap(_v[n], _v[maxi]);
            adjustDown(maxi);
        }
    }
    void pop()
    {
        //交换堆顶和堆最低的元素，
        swap(_v[0], _v[_size - 1]);
        //删除堆最低的元素
        _size--, _v.pop_back();
        adjustDown(0);
    }
    void sort()
    {
       for(int i = _size - 1; i > 0; --i)
       {
            cout << _v[0] << " ";
            pop();
       } 
    }
    void Print()
    {
        for(auto num : _v)
        {
            cout << num << " ";
        }
        cout << endl;
    }
};