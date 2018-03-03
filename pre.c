/********************************************************
 *                                                      
 * 采用shmget semget shmat方法对内存进行操作               
 * 
 * 假设一共有5块共享内存                                   
 * 每块内存100B                                          
 * key值从9900~9904                                      
 *                                                      
 * 创建信号量集
 * 第0个信号量定义为empty
 * 第1个信号量定义为full
 * 第2个信号量定义为mutex                                  
 *                                                     
 * Written by Risan   2018-2                            
 *******************************************************/

#include "library.h"

int main(){
    int segs[MEM_MAX],semset_id;
    int seg_id_buf_in,seg_id_buf_out;
    //创建5块共享内存
    segs[0] = shmget(MEM_KEY_0,MEM_SEG_SIZE,IPC_CREAT|0777);
    segs[1] = shmget(MEM_KEY_1,MEM_SEG_SIZE,IPC_CREAT|0777);
    segs[2] = shmget(MEM_KEY_2,MEM_SEG_SIZE,IPC_CREAT|0777);
    segs[3] = shmget(MEM_KEY_3,MEM_SEG_SIZE,IPC_CREAT|0777);
    segs[4] = shmget(MEM_KEY_4,MEM_SEG_SIZE,IPC_CREAT|0777);
    
    //创建全局变量内存，存放指向内存的指针，和读取内存的指针
    seg_id_buf_in = shmget(MEM_KEY_BUF_IN,MEM_SEG_SIZE,IPC_CREAT|0777);
    seg_id_buf_out = shmget(MEM_KEY_BUF_OUT,MEM_SEG_SIZE,IPC_CREAT|0777);

    int i;
    for ( i= 0;  i<MEM_MAX ; i++) {
        if(segs[i] == -1){
            err("shmget",1);
        }
    }

    //创建3个信号量
    semset_id = semget(SEMSET_KEY,nsems,0666|IPC_CREAT|IPC_EXCL);
    if (semset_id == -1)
        err("semget",3);
    set_sem_value(semset_id,empty,5);
    set_sem_value(semset_id,full,0);
    set_sem_value(semset_id,mutex,1);

    //声明内存内存指针
    char* memptr_in;
    char* memptr_out;

    memptr_in = shmat(seg_id_buf_in,NULL,0);        //获取内存指针
    memptr_out = shmat(seg_id_buf_out,NULL,0);

    strcpy(memptr_in,"0");                          //写入“0”到内存当中  
    strcpy(memptr_out,"0");

    printf("完成缓冲池和信号量的配置\n");
    //检查信号量的值
    printf("empty 是 %d,full 是 %d,mutex 是 %d,in 是 %s,out 是 %s\n",semctl(semset_id,0,GETVAL,0),semctl(semset_id,1,GETVAL,0),semctl(semset_id,2,GETVAL,0),memptr_in,memptr_out);
    return 0;
}
