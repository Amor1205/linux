#include<iostream>
#include"./test_lib/sub.h"
#include"./test_lib/add.h"

int main()
{
	int x = 10;
	int y = 5;

	int addRes = myAdd(x, y);
	int subRes = mySub(x, y);

	std::cout << "addRes : " << addRes << std::endl;
	std::cout << "subRes : " << subRes << std::endl;
	return 0;
}
