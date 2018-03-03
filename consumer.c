#include "library.h"

int main(int argc,char* argv[]){

    int segs_id[5];    //segs中存放了5块内存块id
    int seg_id_buf_out,seg_id_buf_in; //seg中读取指针内存块的id
    int semset_id;     //semset_id是指信号量集的id
    int semval_empty,semval_mutex,semval_full;        //信号量的值
    int buf_out,buf_in;
    char* memptr_out,*memptr_in,*segs_ptr[5];
    time_t now;

    semset_id = semget(SEMSET_KEY,3,0);
    // printf("semset_id的值%d\n",semset_id);

    segs_id[0] = shmget(MEM_KEY_0,MEM_SEG_SIZE,0777);
    segs_id[1] = shmget(MEM_KEY_1,MEM_SEG_SIZE,0777);
    segs_id[2] = shmget(MEM_KEY_2,MEM_SEG_SIZE,0777);
    segs_id[3] = shmget(MEM_KEY_3,MEM_SEG_SIZE,0777);
    segs_id[4] = shmget(MEM_KEY_4,MEM_SEG_SIZE,0777);

    seg_id_buf_in = shmget(MEM_KEY_BUF_IN,MEM_SEG_SIZE,0777);
    seg_id_buf_out = shmget(MEM_KEY_BUF_OUT,MEM_SEG_SIZE,0777);

    memptr_in = shmat(seg_id_buf_in,NULL,0);
    memptr_out = shmat(seg_id_buf_out,NULL,0);

    int i = 0;
    for(i;i<5;i++){
        segs_ptr[i] = shmat(segs_id[i],NULL,0);
    }

    while(1)
    {
        sleep(atoi(argv[1]));
        if ( semctl(semset_id, full, GETVAL, 0) == -1 ||semctl(semset_id, mutex, GETVAL, 0) == -1 )
        {
            perror("semctl error:");
            exit(1);
        }

        if (semctl(semset_id, full, GETVAL, 0) > 0 && semctl(semset_id, mutex, GETVAL, 0) > 0)
        {
            time(&now);
           // printf("full is %d, mutex is %d\n", semctl(semset_id, full, GETVAL, 0),semctl(semset_id, mutex, GETVAL, 0));
            wait_and_lock(semset_id,full);
            wait_and_lock(semset_id,mutex);
                printf("当前时间为%s",ctime(&now));
                buf_out = atoi(memptr_out);
                printf("当读取第%d个内存块,内容为：%s  当前进程id为%d\n",buf_out,segs_ptr[buf_out],getpid());
                memptr_out[0] = itoa((buf_out+1)%MEM_MAX);

                // semval_full = semctl(semset_id, full, GETVAL, 0);
                // semval_mutex = semctl(semset_id, mutex, GETVAL, 0);
                // printf("empty is %d,mutex is %d\n",semval_full,semval_mutex);
                printf("-----------------------------------------------------------\n");
            release_lock(semset_id,mutex);
            release_lock(semset_id,empty);

            //printf("empty 是 %d,mutex 是 %d,full 是 %d\n-----------------------------------------------------------\n",semctl(semset_id, empty, GETVAL, 0),semctl(semset_id, mutex, GETVAL, 0),semctl(semset_id, full, GETVAL, 0));
        }
        else
        {
            time(&now);
            printf("当前时间为%s\n进程%d没有更多资源读取\n-----------------------------------------------------------\n",time(&now),getpid());
        }
    }

    return 0;
}