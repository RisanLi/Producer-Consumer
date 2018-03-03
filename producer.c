/***********************************************
 * 生产者每次产生一个产品：当前时间，
 * 将当前时间写入到获取的内存当中
 * 
 * written by Risan 2018-2
***********************************************/
#include "library.h"

int main(int argc,char* argv[]){

    int segs_id[5];    //segs中存放了5块内存块id
    int seg_id_buf_in,seg_id_buf_out; //seg中的写入指针内存id
    int semset_id;     //semset_id是指信号量集的id
    int semval_empty,semval_mutex,semval_full;        //信号量的值
    int buf_in,buf_out;         //读取的内存指针
    char* memptr_in,*memptr_out,*segs_ptr[5],*temp;    //指向读取内存的指针
    time_t now;

    semset_id = semget(SEMSET_KEY,3,0);
    //printf("semset_id的值%d\n",semset_id);

    segs_id[0] = shmget(MEM_KEY_0,MEM_SEG_SIZE,0777);               //从已有的内存中获取内存id
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
        sleep(atoi(argv[1]));                                       //读取main参数
        if ( semctl(semset_id, empty, GETVAL, 0) == -1 ||semctl(semset_id, mutex, GETVAL, 0) == -1 )
        {
            perror("semctl error:");
            exit(1);
        }

        if (semctl(semset_id, empty, GETVAL, 0) > 0 && semctl(semset_id, mutex, GETVAL, 0) > 0)
        {
            //printf("empty is %d, mutex is %d\n", semctl(semset_id, empty, GETVAL, 0),semctl(semset_id, mutex, GETVAL, 0));
            wait_and_lock(semset_id,empty);
            wait_and_lock(semset_id,mutex);
                buf_in = atoi(memptr_in);
                buf_out = atoi(memptr_out);

                time(&now);
                printf("当前时间为%s\n",ctime(&now));
                //buf_in = str_to_int(memptr_in);
                // semval_empty = semctl(semset_id, empty, GETVAL, 0);
                // semval_mutex = semctl(semset_id, mutex, GETVAL, 0);
                // printf("empty is %d,mutex is %d,id is %d\n",semval_empty,semval_mutex,getpid());

                // printf("写入第%d个内存块  当前进程id为%d\n",buf_in,getpid());
                // char pid_str[25];
                // sprintf(pid_str,"%d",(int)getpid());
                // strcpy(segs_ptr[buf_in],pid_str);

                printf("当前一共占用%d块内存\n",(buf_in+-buf_out+MEM_MAX)%MEM_MAX);
                if(buf_out>buf_in){
                    int j = buf_out;
                    for(j;j<MEM_MAX;j++){
                        printf("第%d块内存被进程%s占用\n",j,segs_ptr[j]);
                    }
                    j=0;
                    for(j;j<buf_in;j++){
                         printf("第%d块内存被进程%s占用\n",j,segs_ptr[j]);
                    }
                }
                else if(buf_out == buf_in)
                    printf("没有内存被占用\n");
                else{
                    int j = buf_out;
                    for(j;j<buf_in;j++){
                        printf("第%d块内存被进程%s占用\n",j,segs_ptr[j]);
                    }
                }
                printf("写入第%d个内存块  当前进程id为%d\n",buf_in,getpid());
                char pid_str[25];
                sprintf(pid_str,"%d",(int)getpid());
                strcpy(segs_ptr[buf_in],pid_str);
                memptr_in[0] = itoa((buf_in+1)%MEM_MAX);
                // memptr_in[0] = itoa(buf_in);
               
                printf("-----------------------------------------------------------\n");
            release_lock(semset_id,mutex);
            release_lock(semset_id,full);
          //  printf("empty 是 %d,mutex 是 %d,full 是 %d\n",semctl(semset_id, empty, GETVAL, 0),semctl(semset_id, mutex, GETVAL, 0),semctl(semset_id, full, GETVAL, 0));
            
        }
        else
        {
            time(&now);
            printf("当前时间为%s\n进程%d没有更多资源写入\n-----------------------------------------------------------\n",ctime(&now),getpid());
        }
    }

    return 0;
}
