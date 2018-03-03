//
// 清除key确定的共享内存和信号量集
// Created by risan on 18-2-17.
//
#include "library.h"
int segs[5],semset_id;
int seg_id_buf_in,seg_id_buf_out;

void cleanup(int n){
    for (int i = 0; i <MEM_MAX ; ++i) {
        shmctl(segs[i],IPC_RMID,NULL); //删除共享内存id所对应的内存
    }
    shmctl(seg_id_buf_in,IPC_RMID,NULL);
    shmctl(seg_id_buf_out,IPC_RMID,NULL);
    semctl(semset_id,empty,IPC_RMID,NULL);
    semctl(semset_id,full,IPC_RMID,NULL);
    semctl(semset_id,mutex,IPC_RMID,NULL);

}

int main(){
    semset_id = semget(SEMSET_KEY,nsems,0);

    segs[0] = shmget(MEM_KEY_0,MEM_SEG_SIZE,0777);
    segs[1] = shmget(MEM_KEY_1,MEM_SEG_SIZE,0777);
    segs[2] = shmget(MEM_KEY_2,MEM_SEG_SIZE,0777);
    segs[3] = shmget(MEM_KEY_3,MEM_SEG_SIZE,0777);
    segs[4] = shmget(MEM_KEY_4,MEM_SEG_SIZE,0777);
    
    seg_id_buf_in = shmget(MEM_KEY_BUF_IN,MEM_SEG_SIZE,0777);
    seg_id_buf_out = shmget(MEM_KEY_BUF_OUT,MEM_SEG_SIZE,0777);

    cleanup(0);
}