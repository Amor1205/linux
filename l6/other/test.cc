#include"./lib/add.h"
#include"./lib/sub.h"

int main()
{
	int x = 10;
	int y = 5;
	int r1 = myAdd(x,y);
	int r2 = mySub(x,y);

	printf("r1 : %d\n", r1);
	printf("r2 : %d\n", r2);

	return 0;
}
