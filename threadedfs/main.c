//
//  main.c
//  threadedfs
//
//  Created by Sankarsan Seal on 18/07/16.
//  Copyright © 2016 Sankarsan Seal. All rights reserved.
//

#include <stdio.h>
#include <pthread.h>

void threaded(int i)
{
    printf("I am now %d\n",i);
    
}

int main(int argc, const char * argv[]) {
    // insert code here...
    int i;
    for(i=0;i<5;i++)
        threaded(i);
    return 0;
}
