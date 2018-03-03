#include <stdio.h>
#include <sys/shm.h>
#include <time.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <zconf.h>
#include <memory.h>
#include <unistd.h>

#define MEM_MAX 5       //共享内存数量

#define MEM_KEY_0 9900  //第0块内存key值
#define MEM_KEY_1 9901
#define MEM_KEY_2 9902
#define MEM_KEY_3 9903
#define MEM_KEY_4 9904  //第4块内存key值

#define MEM_KEY_BUF_IN 9905 //读取内存指针：全局变量
#define MEM_KEY_BUF_OUT 9906//写入内存指针：全局变量

#define MEM_SEG_SIZE 100    //共享内存块大小

#define SEMSET_KEY 99   //信号量集key值

#define nsems 3         //信号量集大小
#define empty 0         //第0个信号量叫empty
#define full 1          //第1个信号量叫full
#define mutex 2         //第2个信号量叫mutex

#define err(m,x) {perror(m);exit(x);}   //报错宏定义

union semun{
    int val;    //信号量的值
    struct semid_ds *buf;
    ushort* array;
};


//设置信号量值函数
void set_sem_value(int semset_id,int semnum,int val){
    union semun initval;
    initval.val = val;

    if(semctl(semset_id,semnum,SETVAL,initval) == -1)
        err("semaphore control",4);
}


//P操作函数
void wait_and_lock(int semset_id,int semnum){
    struct sembuf actions[1];

    actions[0].sem_num = semnum;
    actions[0].sem_flg = IPC_NOWAIT;
    actions[0].sem_op = -1;

    if(semop(semset_id,actions,1) == -1){
        printf("第%d个信号量没有资源\n",semnum);
        while(1){               //手动阻塞
            if(semctl(semset_id,semnum,GETVAL,0) > 0){
                actions[0].sem_num = semnum;
                actions[0].sem_flg = IPC_NOWAIT;
                actions[0].sem_op = -1;
                semop(semset_id,actions,1);
                break;
            }
        }
      //  err("semop:locking",10);
    }

}


//V操作函数
void release_lock(int semset_id,int semnum){
    if(semctl(semset_id,semnum,GETVAL,0)>=MEM_MAX)
        return;
    else{
        struct sembuf actions[1];

        actions[0].sem_num = semnum;
        actions[0].sem_flg = IPC_NOWAIT;
        actions[0].sem_op = +1;

        if(semop(semset_id,actions,1) == -1)
            // err("semop:unlocking",10);
            printf("第%d个信号量可以使用",semnum);
    }
    
}


char itoa(int n){   //一位数字的整型到字符
    return '0'+n;
}


