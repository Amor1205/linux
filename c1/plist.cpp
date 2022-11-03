#include <iostream>
//#include <queue>
#include <vector>
#include <functional>
#include "pq_realize.cpp"
#include<utility>
using namespace amor;

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
template <class T>
struct Less
{
	bool operator()(const T& x, const T& y) const
	{
		return x < y;
	}
};
template<class T>
struct Greater
{
	bool operator()(const T& x, const T& y) const
	{
		return x > y;
	}
};



void test_priority_queue()
{
	priority_queue<int,std::vector<int>,std::less<int> > pq;
	pq.push(3);	
	pq.push(4);
	pq.push(5);
	pq.push(2);
	pq.push(1);
	pq.push(7);
	while(!pq.empty())
	{
		std::cout<< pq.top()<<std::endl;
		pq.pop();
	}
}

template <class T,size_t N>  //N -> non-type template parameter --> must be integer constant
class Mystack{
	public:
		void Push(const T& x)
		{}
	private:
		T _a[N];
		size_t _top;

};


template <class T>
bool ObjLess(const T& p1, const T& p2)
{
	return left < right;
}
int main()
{
	cout << ObjLess(1,2) << endl;
	Date* p1 = new Date(2000,1,1);
	Date* p2 = new Date(2010,1,1);

	cout << ObjLess(p1,p2) << endl;
}

int main()
{
	Mystack<int,100> st;
	st.Push(10);
}

//int main()
//{
	//test_priority_queue();
	/*Less<int> lessi;
	std::cout << lessi(1,2) << std::endl;//
	std::cout << Less<int>()(1,2) << std::endl;//anonymous object
	//Greater gt;
	//std::cout << gt(1,2) << std::endl;
	*/
	
//	return 0;
//}
