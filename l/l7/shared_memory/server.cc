#include"com.h"

int main()
{
	key_t key = ftok(PATH_NAME, PROJ_ID);
	if(key < 0)
	{
		perror("ftok");
		return 1;
	}

	int shmId = shmget(key, SIZE, IPC_CREAT | IPC_EXCL | 0666);
	if(shmId < 0)
	{
		perror("shmget");
		return 2;
	}
	cout << "shmid: " << shmId << " " << "key : " << key << endl;
	
	char * mem =(char*) shmat(shmId, NULL, 0);
	cout << "shmat is successful" << endl;
	
	while(1)
	{
		sleep(1);
		cout << mem << endl;

	}

	int ret = shmdt(mem);
	cout << "shmdt is successful" << endl;    
	shmctl(shmId, IPC_RMID, NULL);

	return 0;
}
