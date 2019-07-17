#include <stdio.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <string.h>
#include <sys/shm.h>

int main()
{
    struct sembuf lock[2]={0,0,0,0,1,0};
    struct sembuf unlock={0,-1,0};
    struct sembuf unlock2={1,-1,0};

    char *ptr;
    int shmid;

    key_t key;
    key=ftok("server",(int)'a');

    int semid=semget(key,1,0);
    int semid2=semget(key,1,0);

    if ((shmid=shmget(key,30*sizeof(char),0))==-1)
    {
        printf("Can't find allocated memory!\n");
        return -1;
    }

    ptr=shmat(shmid,NULL,0);

    if (*ptr==-1)
    {
        printf("Can't attach shared memory!\n");
        return -2;
    }


    semop(semid,lock,2);

    printf("Receive: %s!\n",ptr);
    strncpy(ptr,"Hi!",30);
    printf("Send: Hi!\n");

    semop(semid,&unlock2,1);

    semop(semid,&unlock,1);




    return 0;
}
