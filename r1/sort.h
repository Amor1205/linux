#pragma once
#include<vector>
#include<string>
#include<iostream>
#include<algorithm>
#include<queue>

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
    int gap = v.size();
    while(gap > 1)
    {
        gap = gap / 3 + 1;
        cout << "gap : " << gap << endl;
        for(int i = 0; i < v.size()-gap; ++i)
        {
            int end = i;
            int tmp = v[end + gap];
            while(end >= 0)
            {
                if(v[end] > tmp)
                {
                    v[end + gap] = v[end];
                    end -= gap;
                }
                else
                {
                    break;
                }
            }
            v[end + gap] = tmp;
        }
    }

}

void direct_selection_sort(vector<int>& v)
{
    auto it = v.begin();
    for(int i = 0; i < v.size()-1; ++i)
    {
        auto pmin = min_element(it,v.end());
        if(v[i] != *pmin)
        {
            swap(v[i],*pmin);
        }
        ++it;
    }
}

void heap_sort(vector<int>& v)
{
    priority_queue<int> pq;
    for(auto e : v)
    {
        pq.push(e);
    }
    for(int i = 0; i < v.size()-1; ++i)
    {
        if(v[i] != pq.top())
        {
            v[i] = pq.top();
        }
        pq.pop();
    }
}

void Swap(int* p1, int* p2)
{
    int tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
}

void adjust_down(int* a,int n, int parent)
{
    int min_child = parent*2 + 1;
    while(min_child < n)
    {
        if(min_child+1 < n && a[min_child+1] < a[min_child])
        {
            ++min_child;
        }
        if(a[min_child] > a[parent])
        {
            Swap(&a[min_child], &a[parent]);
            parent = min_child;
            min_child = parent * 2 + 1;
        }
        else
        {
            break;
        }
    }
}

void c_heap_sort(int* a, int n)
{
    //form heap
    for(int i = (n - 1 - 1)/2; i >= 0; --i)
    {
        adjust_down(a,n,i);
    }
    //choose number
    int i = 1;
    while(i < n)
    {
        Swap(&a[0], &a[n-i]);
        adjust_down(a,n-i,0);
        ++i;
    }
}