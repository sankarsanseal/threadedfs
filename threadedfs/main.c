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

#include "inode_struct.c"

FILE * file_create(const char * filename)
{
    
    return fopen(filename,"w");
}

int main(int argc, const char * argv[]) {
    // insert code here...
    unsigned int no_of_inode;
    unsigned int size_in_bytes;
    unsigned int blocksize;
    char blanked=' ';
//    INODE_STRUCT ind;
    

    
    FILE * fsfile;
    

    
    if( argc !=4)
    {
        printf("usage:%s <size in MB> <data block size> <path to file>\n",argv[0]);
        return 1;
    }
    else
    {
        size_in_bytes=atoi(argv[1])<<20;
        blocksize=atoi(argv[2]);
        
        no_of_inode=(size_in_bytes)/blocksize;
        printf("Possible no of inodes %u for filesystem size %u MB with data block size %u bytes\n",
               no_of_inode,atoi(argv[1]), atoi(argv[2]));
        if(!access(argv[3],F_OK))
        {
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
        
        printf("Server process id:%u\n",getpid());
        
        
        
        fclose(fsfile);
        return 0;
    }
}
