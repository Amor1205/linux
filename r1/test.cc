#include"sort.h"


int main()
{
    vector<int> v;
    int a[] = {5,3,2,7,9,11,1};
    for(auto e : a)
    {
        v.push_back(e);
    }
    //direct_insert_sort(v);
    shell_sort(v);
    for(auto e : v)
    {
        cout << e << " ";
    }
}