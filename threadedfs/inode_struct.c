//
//  inode_struct.c
//  threadedfs
//
//  Created by Sankarsan Seal on 20/07/16.
//  Copyright © 2016 Sankarsan Seal. All rights reserved.
//

#define FILEPERM S_IRUSR|S_IWUSR|S_IWGRP|S_IWOTH|S_IRGRP


typedef struct inode_struct
{
    
    unsigned int userid;
    char  creation_dt[16];
    char  access_dt[16];
    char  modidy_dt[16];
    unsigned int type;
    unsigned int data_block_index[13];
} INODE_STRUCT;

