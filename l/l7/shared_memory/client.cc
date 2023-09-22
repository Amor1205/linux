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
	int shmId = shmget(key ,SIZE, IPC_CREAT);
	if(shmId < 0)
	{
		perror("shmget");
		return 1;
	}
	char* mem =(char*) shmat(shmId, NULL,0);
	char ch = 'A';

	while(ch < 'Z')
	{
		mem[ch - 'A'] = ch;
		ch ++;
		mem[ch - 'A'] = 0;
		sleep(1);
	}

	shmdt(mem);
	sleep(5);
	return 0;
}
