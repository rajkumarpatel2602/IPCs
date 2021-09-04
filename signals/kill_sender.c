#include<stdio.h>
#include<signal.h>
#include<stdlib.h>

int
main(int args, void *argv[]){

    int ret = kill(7379, SIGUSR1);
    if(ret  == -1){
        printf("Kill command failed\n");
    }
}
