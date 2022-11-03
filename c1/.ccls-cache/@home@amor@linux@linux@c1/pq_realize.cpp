#pragma once
#include <assert.h>
#include <vector>

namespace amor
{
	//by default : maxHeap
	template<class T, class Container = vector<T>>
	class priority_queue
	{
	private:
		void adjust_up(size_t child)
		{
			size_t parent = (child - 1) / 2;
			while(child > 0)
			{
				if(_con[child] > _con[parent])
				{
					swap(_con[child], _con[parent]);
					child = parent;
					parent = (child - 1) / 2;

				}
				else
				{
					break;
				}
			}
		}
		void adjust_down(size_t parent)
		{
			size_t child  = parent * 2 + 1;
			while(child < _con.size())
			{
				if(child + 1 < _con.size() && _con[child + 1] > _con[child])
				{
					++child;
				}
				if(_con[child] > _con[parent])
				{
					swap(_con[child], _con[parent]);
					parent = child;
					child  = parent + 1;
				}
				else
				{
					break;
				}
			}
		}
	public:
		priority_queue()
		{}

		template <class InputIterator>
		priority_queue(InputIterator first, InputIterator last)
			:_con(first,last)
		{
			for(int i = (_con.size() - 1 - 1)/2;i>=0;--i)
			{
				adjust_down(i);
			}
		}

		void push(const T& x)
		{
			_con.push_back(x);
			adjust_up(_con.size()-1);
		}

		void pop()
		{
			assert(!_con.empty());
			swap(_con[0], _con[_con.size() - 1]);
			_con.pop_back();
			adjust_down(0);
		}

		const T& top()
		{
			return _con[0];
		}

		size_t size()
		{
			return _con.size();
		}

		bool empty()
		{
			return _con.empty();
		}

	private:
		Container _con;
		
	};
}
