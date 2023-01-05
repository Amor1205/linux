#pragma once
#include<vector>
#include<string>
#include<iostream>
using namespace std;

//direct insertion sort
void direct_insert_sort(vector<int>& v)
{
    for(int i = 0;i < v.size()-1; ++i)
    {
        int end = i;
        int tmp = v[end+1];
        while(end >= 0)
        {
            if(v[end] > tmp)
            {
                v[end + 1] = v[end];
                --end;
            }
            else
            {
                break;
            }
        }
        v[end+1] = tmp;
    }
}
void shell_sort(vector<int>& v)
{
    int gap = v.size()/2;
    while(gap >= 1)
    {
    for(int i = 0; i< v.size()-1; ++i)
    {
        if(gap + i > v.size()-1)
        {
            break;
        }
        if(v[i] > v[gap+i])
        {
            swap(v[i],v[gap+i]);
        }
    }
    gap /= 2;
    }
}