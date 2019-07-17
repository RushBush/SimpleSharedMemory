#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <string.h>

int main()
{
    key_t key;
    int shmid,semid;
    char *ptr;

    struct sembuf lock[2]={0,0,0,0,1,0};
    struct sembuf unlock={0,-1,0};
    struct sembuf lock2[2]={1,0,0,1,1,0};
    struct sembuf unlock2={1,-1,0};

    key=ftok("server",(int)'a');



    if ((shmid=shmget(key,30*sizeof(char),IPC_CREAT | 0666))==-1)
    {
        printf("Can't allocate shared memory!\n");
        return -1;
    }


    ptr=shmat(shmid,NULL,0);

    if (*ptr==-1)
    {
        printf("Can't attach shared memory!\n");
        return -2;
    }


    semid=semget(key,2,IPC_CREAT | 0666);

    semop(semid,lock2,2);
    semop(semid,lock,2);

    strncpy(ptr,"Hello!",30);
    printf("Send: Hello!\n");

    semop(semid,&unlock,1);

    semop(semid,lock2,2);

    printf("Receive: %s\n",ptr);

    semop(semid,&unlock2,1);

    semctl(semid,0,IPC_RMID);

    return 0;
}
