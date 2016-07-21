//
//  main.c
//  threadedfs
//
//  Created by Sankarsan Seal on 18/07/16.
//  Copyright © 2016 Sankarsan Seal. All rights reserved.
//
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include "threadedfs_client/threadedfs_client/struct_msg.h"
#define MAXSIZE 1024
#define MAXLINE 80



MSGP allot_msg()
{
    MSGP temp;
    
    if((temp=(MSGP)malloc(sizeof (MSG)))!=NULL)
        return temp;
    else
        return NULL;
    
}

SMSGP allot_smsg()
{
    SMSGP temp;
    
    if((temp=(SMSGP)malloc(sizeof (SMSG)))!=NULL)
        return temp;
    else
        return NULL;
    
}


void * write_cmsg(void * temp)
{
    MSGP temp2=(MSGP) temp;
    SMSGP temp3;
    
    char path_to_client_fifo[MAXSIZE];
    unsigned int clientfifo;

    
    sprintf(path_to_client_fifo,"/tmp/client.%d",temp2->client_pid);

    clientfifo=open(path_to_client_fifo,O_WRONLY);
    
    temp3=allot_smsg();
    if(temp3!=NULL)
    {
        strcpy(temp3->msg_body,"Instruction received for user");
        temp3->more=0;
        temp3->msg_id=1;
        write(clientfifo, temp3 , sizeof(temp3));
        free(temp3);
        
    }
    else
        printf("Problem with Server Message writing\n");
    free(temp2);
    close(clientfifo);
    
    return NULL;
}




FILE * file_create(const char * filename)
{
    
    return fopen(filename,"w");
}

int main(int argc, const char * argv[]) {
    // insert code here...
    unsigned int no_of_inode;
    unsigned int size_in_bytes;
    unsigned int blocksize;
    unsigned int pid;
    unsigned int servfifo;
    unsigned int dummy;
    unsigned int userid;
    unsigned int instruct_code;
    pid_t client_pid;
    pthread_t tid;
    
    
    int errno;
    ssize_t n;
    char blanked=' ';
    char buff[MAXSIZE];
    char path[MAXSIZE];
    char path_to_server_fifo[MAXSIZE];
    
    char path_to_lock_file[MAXSIZE];
    char msg[MAXSIZE];
    
    MSGP temp,temp2;
    
//    INODE_STRUCT ind;
    
    pid=getpid();
    
    FILE * fsfile;
    FILE * fsfilelock;
    

    
    if( argc !=4)
    {
        printf("usage:%s <size in MB> <data block size> <path to file>\n",argv[0]);
        return 1;
    }
    else
    {
      /*  size_in_bytes=atoi(argv[1])<<20;
        blocksize=atoi(argv[2]);
        
        no_of_inode=(size_in_bytes)/blocksize;
        printf("Possible no of inodes %u for filesystem size %u MB with data block size %u bytes\n",
               no_of_inode,atoi(argv[1]), atoi(argv[2]));
        sprintf(path_to_lock_file,"%s.lock",argv[3]);
        
        if(!access(argv[3],F_OK))
        {
            
            fsfilelock=fopen(path_to_lock_file,"w")
            fsfile=fopen(argv[3],"r+");
            fseek(fsfile,0,SEEK_SET);
        }
        else
        {
            fsfile=file_create(argv[3]);
            fseek(fsfile,size_in_bytes,SEEK_SET);
            fwrite(&blanked,sizeof(char),1,fsfile);
            fseek(fsfile,0,SEEK_SET);

        }
        
        
        printf("Server process id:%u\n",pid);
        sprintf(path_to_server_fifo,"/tmp/server.%d",pid);
  
        
        if(access(path_to_server_fifo,F_OK))
        {
            
        }
        else
        {
            printf("Already a server fifo is running w\n");
        }
        
        
        
        fclose(fsfile);*/
        if((errno=mkfifo("/tmp/server", S_IRUSR|S_IWUSR|S_IWGRP|S_IWOTH|S_IRGRP)<0))
        {
            printf("FIFO exists\n");
            unlink("/tmp/server");
        }
        else
        {
            
            servfifo=open("/tmp/server",O_RDONLY);
            dummy=open("/tmp/server",O_WRONLY);
            
            temp=allot_msg();
            
            if(temp!=NULL)
            {
                while((n=read(servfifo,temp,sizeof(MSG))))
                {
            
                 
                    temp2=allot_msg();
                    
                    if(temp2!=NULL)
                    {
                        temp2=temp;
                        pthread_create(&tid, NULL, write_cmsg, (void *)temp2);
                        
                    }
                    
                
                }
                
            }
            else
            {
                printf("Some problem with memory allocation\n");
            }
            unlink("/tmp/server");
        }
        return 0;
    }
}
