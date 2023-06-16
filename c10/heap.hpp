#include<iostream>
#include<vector>

class Heap{
private: 
    vector<int> _v;
public:
    Heap(){}
    ~Heap(){
        _v.clear();
    }
public: 
    // 从序号为n开始向下调整
    void AdjustDown(int n) 
    {
        int size = _v.size();
        if(2n+1 < size) AdjustDown(2n+1);
        
    }
};