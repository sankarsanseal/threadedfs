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
#include "threadedfs_client/threadedfs_client/struct_msg.h"
#include "inode_struct.h"
#define MAXSIZE 1024
#define MAXLINE 80

unsigned int servfifo;
unsigned int dummy;

char path_to_server_fifo[MAXSIZE];
char path_to_lock_file[MAXSIZE];
char path_to_file[MAXSIZE];
char msg[MAXSIZE];

FILE * fsfile;
FILE * fsfilelock;

unsigned int no_of_inode;
unsigned int size_in_bytes;
unsigned int size_in_MB;
unsigned int no_of_datablock;
unsigned int blocksize;
unsigned int pid;

SB filesuperblock;

INODE empty_inode;
DIRSTRC empty_datablock;




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

void cleanup(int signaltype)
{
    
    close(servfifo);
    fclose(fsfile);
    fclose(fsfilelock);
    unlink(path_to_lock_file);
    unlink("/tmp/server");
    printf("\nClean up complete.\n");
    exit(0);
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
        printf("%s",temp3->msg_body);
        temp3->more=0;
        temp3->msg_id=temp2->instruct_code;
        write(clientfifo, temp3 , sizeof(temp3));
        free(temp3);
        
    }
    else
        printf("Problem with Server Message writing\n");
    free(temp2);
    close(clientfifo);
    
    return NULL;
}

void initialize_empty_inode()
{
    int i;
    empty_inode.accessed=0;
    empty_inode.created=0;
    empty_inode.grdid=0;
    empty_inode.modified=0;
    empty_inode.userid=0;
    
    for(i=0;i<sizeof(empty_inode.datablock)/sizeof(empty_inode.datablock[0]);i++)
        empty_inode.datablock[i]=0;
}


void file_create()
{
    int i;
    int no_of_datablock_add;
    int no_of_user;
    
    FDBP free_data_block;
    
    fsfilelock = fopen(path_to_lock_file,"w");
    fsfile=fopen(path_to_file,"w");
    
    initialize_empty_inode();
    
    fseek(fsfile,sizeof(SB),SEEK_SET);
    
    for(i=0;i<no_of_inode;i++)
    fwrite(&empty_inode,sizeof(INODE),1,fsfile);
    
    printf("Inode blocks are written upto :%lld\n",ftello(fsfile));
    

    no_of_datablock=(size_in_bytes-sizeof(SB)-(sizeof(INODE)*no_of_inode))/blocksize;
    
    

    
    empty_datablock.inode=0;
    empty_datablock.name[0]='\0';
    
    no_of_datablock_add=blocksize/sizeof(off_t);
    printf("no_of_datablock_add per datablock %d\n",no_of_datablock_add);
    
    free_data_block=(FDBP)malloc(sizeof(FDB)*no_of_datablock_add);
    
    while(ftello(fsfile)<size_in_bytes)
    {
        
        
        for(i=0;i<no_of_datablock_add && ftello(fsfile)<size_in_bytes;i++)
        {
            *(free_data_block+i)=ftello(fsfile);
           // printf("Size of %ld %d\n", sizeof(off_t),i);
            fseek(fsfile,sizeof(FDB),SEEK_CUR);
        
        }
        
        fwrite(free_data_block,sizeof(FDB)*i,1,fsfile);
        
    }
    
    //Return to first inode location and create home directories for given users
    fseek(fsfile,sizeof(SB),SEEK_SET);
    
    printf("Enter the number of users in filesystem:");
    scanf("%d",&no_of_user);
    
    for(i=0;i<no_of_user;i++)
    {
        
        
    }
    
    
}

void point_user_dir()
{
    
    
}

void print_present_user_dir()
{
    
}

void change_user_dir()
{
    
}

void make_user_dir()
{
    
}

void remove_user_dir()
{
    
}

void changeowner()
{
    
}

void create_user_file()
{
    
}

void rename_copy_user_file_dir()
{
    
}

void delete_user_file()
{
    
}



int main(int argc, const char * argv[]) {
    // insert code here...


    pthread_t tid;
    
    
    int errno;
    ssize_t n;
    
    MSGP temp,temp2;
    
//    INODE_STRUCT ind;
    
    pid=getpid();
    
   
    signal(SIGINT,cleanup);
    signal(SIGKILL,cleanup);
    
    if( argc !=4)
    {
        printf("usage:%s <size in MB> <path to file>\n",argv[0]);
        return 1;
    }
    else
    {
        size_in_MB=atoi(argv[1]);
        size_in_bytes=size_in_MB <<20;
        blocksize=atoi(argv[2]);
        
        strcpy(path_to_file,argv[3]);
        
        no_of_inode=(size_in_bytes)/blocksize;
        printf("Possible no of inodes %u for filesystem size %u MB with data block size %u bytes\n",
               no_of_inode,size_in_MB, blocksize);
        
        sprintf(path_to_lock_file,"%s.lock",argv[3]);
        
        if((!access(path_to_file,F_OK))&&(access(path_to_lock_file,F_OK)))
        {
            
            fsfilelock=fopen(path_to_lock_file,"w");
            fsfile=fopen(path_to_file,"r+");
            fseek(fsfile,0,SEEK_SET);
        }
        else
        {
            file_create();
        }
        
        
        printf("Server process id:%u\n",pid);
        printf("Server is running.\n To stop the server gracefully please press Ctrl+c\n");
        //sprintf(path_to_server_fifo,"/tmp/server.%d",pid);
  
        
       /* if(access(path_to_server_fifo,F_OK))
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
            cleanup(SIGINT);
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
            
                 switch(temp->instruct_code)
                    {
                        case 1:
                            point_user_dir();
                            break;
                        case 2:
                            print_present_user_dir();
                            break;
                        case 3:
                            change_user_dir();
                            break;
                        case 4:
                            make_user_dir();
                            break;
                        case 5:
                            remove_user_dir();
                            break;
                        case 6:
                            changeowner();
                            break;
                        case 7:
                            create_user_file();
                            break;
                        case 8:
                            rename_copy_user_file_dir();
                            break;
                        case 9:
                            delete_user_file();
                            break;
                    }
                    
                
                }
                
            }
            else
            {
                printf("Some problem with memory allocation\n");
            }
            cleanup(SIGINT);
        }
        return 0;
    }
}
