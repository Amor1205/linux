#include <iostream>
#include <queue>
#include <vector>
#include <functional>

using namespace std;

void test_topk()
{
	sort(nums.begin(),nums.end(),greater<int>());
	return nums[k-1];
}
void test_topk2()
{
	priority_queue<int> manHeap(nums.begin(),nums.end());
	while(--k)//pop k-1 times
	{
		maxHeap.pop();
	}
	return manHeap.top();
}



void test_priority_queue()
{
	priority_queue<int,vector<int>,greater<int>> pq;
	pq.push(3);	
	pq.push(4);
	pq.push(5);
	pq.push(2);
	pq.push(1);
	pq.push(7);
	while(!pq.empty())
	{
	cout<< pq.top()<<endl;
	pq.pop();
	}
}

int main()
{
	test_priority_queue();
	return 0;
}
