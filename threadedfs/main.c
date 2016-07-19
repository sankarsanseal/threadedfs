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
#include<unistd.h>

pthread_t test1;

void * threaded(void *arg)
{
    printf("I am now with PID: %u\n",getpid());
    printf("I am inside the thread: %u\n", (unsigned int)pthread_self());
    
    return( (void *)0);
    
    
}

int main(int argc, const char * argv[]) {
    // insert code here...
    int i;
    
    i=pthread_create(&test1,NULL,threaded,NULL);
    if(i!=0)
        return i;
    else
    {
        pthread_join(test1,NULL);
        printf("I am now in main with PID: %u\n",getpid());
        printf("I am inside the thread: %u\n", (unsigned int)pthread_self());
        return 0;
    }

}
