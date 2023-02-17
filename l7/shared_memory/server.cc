#include"com.h"

int main()
{
	key_t key = ftok(PATH_NAME, PROJ_ID);
	if(key < 0)
	{
		perror("ftok");
		return 1;
	}

	int shmId = shmget(key, SIZE, IPC_CREAT | IPC_EXCL);
	if(shmId < 0)
	{
		perror("shmget");
		return 2;
	}
	cout << "shmid: " << shmId << " " << "key : " << key << endl;
    
	shmctl(shmId, IPC_RMID, NULL);

	return 0;
}
