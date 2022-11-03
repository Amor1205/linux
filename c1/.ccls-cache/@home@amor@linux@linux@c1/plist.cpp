#include <iostream>
//#include <queue>
#include <vector>
#include <functional>
#include "pq_realize.cpp"

using namespace std;

//void test_topk1(vector<int>& nums, int k)
//{
//	sort(nums.begin(),nums.end(),greater<int>());
//	return nums[k-1];
//}
//O(n*logn)
//void test_topk2(vector<int>& nums, int k)
//{
//	priority_queue<int> manHeap(nums.begin(),nums.end());
//	while(--k)//pop k-1 times
//	{
//		maxHeap.pop();
//	}
//	return manHeap.top();
//}
//O(N+K*logN)
//void test_topk3(vector<int>& nums, int k)
//{
//	priority_queue<int, vector<int>, greater<int>> minHeap(nums.begin(),nums.begin()+k);
//	for(size_t i = k;i<nums.size();++i)
//	{
//		if(nums[i]>minHeap.top())
//		{
//			minHeap.pop();
//			minHeap.push(nums[i]);
//		}
//	}
//	return minHeap.top();
//}
//O(K+(N-K)*logK) --> O(N*logK)
//the best : 3


void test_priority_queue()
{
	amor::priority_queue<int,vector<int>> pq;
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
