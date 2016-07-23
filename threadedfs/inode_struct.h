//
//  inode_struct.h
//  threadedfs
//
//  Created by Sankarsan Seal on 21/07/16.
//  Copyright © 2016 Sankarsan Seal. All rights reserved.
//

#ifndef inode_struct_h
#define inode_struct_h
#define MAX_FREE_DATA_BLOCK 512

typedef struct superblock
{
    long remembered;
    long no_fdata_block;
    off_t first_avail_block;
    
} * SBP, SB;


typedef struct inode
{
    unsigned int userid;
    unsigned int grdid;
    unsigned int type;//0 -- empty inode 1 -- directory , 2 -- normal file
    time_t created;
    time_t modified;
    time_t accessed;
    off_t  datablock[10];
    
} * INODEP, INODE;

typedef struct direct_struct
{
    long inode;
    char name[24];
} * DIRSTRCP, DIRSTRC;

typedef off_t * FDBP, FDB;


#endif /* inode_struct_h */
