#include"com.h"

int main()
{
	key_t key = ftok(PATH_NAME, PROJ_ID);
	if(key < 0)
	{
		perror("ftok");
		return 1;
	}
	cout << "key :  " << key << endl;
    return 0;
}
