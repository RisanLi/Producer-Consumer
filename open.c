/********************************************************
 *                                                      *
 * 采用读取文本的方式读取配置文件                            *
 * 本进程采用fork和exec函数来在进程中创建进程                *
 *                                                      *
 * Written by Risan   2018-2                            * 
 *******************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "library.h"

int main(){
    printf("***********************************************\n");
    printf("请输入打开配置文件的路径：\n");
    char filepath[1000];//文件路径
    int nproducers, nconsumers;//生产者数量，消费者数量
    int protime,contime;//生产者时间，消费者时间
    char protime_str[25],contime_str[25];

    scanf("%s",filepath);
    FILE *configFile = fopen(filepath,"r"); //读取配置文件
    if(configFile == NULL){
        printf("filepath wrong!\n");
        exit(0);
    }
    char temp_str[100];
    int j = 4;
    for(j;j>0;j--){
        fgets(temp_str,5,configFile);
        if(j==4)    nproducers = atoi(temp_str);
        else if(j==3) nconsumers = atoi(temp_str);
        else if(j==2) protime = atoi(temp_str);
        else contime = atoi(temp_str);
    }
   sprintf(protime_str,"%d",protime);       //类似于itoa()
   sprintf(contime_str,"%d",contime);
    int i;
    for(i = 0;i<nproducers+nconsumers;i++){
        int pid = fork();                   //创建子进程
        if(pid<0){
            perror("fork");
            exit(0);
        }
        else if(pid == 0 && i < nproducers ){   //创建nproducers个 producer进程
            execl("/home/risan/Desktop/p-c/producer","producer",protime_str,NULL); //命令行执行命令并传参
        }
        else if(pid == 0 && i >= nproducers){
            execl("/home/risan/Desktop/p-c/consumer","consumer",contime_str,NULL);
        }
        else                                //父进程的作用就是为了创建子进程，没有其他功能
            continue;
        
    }


    return 0;
}