#include<stdio.h>
#include<signal.h>
#include<stdlib.h>


static void
cb_sigint_sig_handler(int sig){
    printf("SIGINT received from process itself using raise()\n");
    printf("Bye, Bye!\n");
    exit(0);
}

int
main(int args, void *argv[]){
    char input = 0;

    signal(SIGINT, cb_sigint_sig_handler);

    printf("rasie signal test\n");
    int ret = raise(SIGINT);
    if(ret  == -1){
        printf("Coudn't raise the signal\n");
    }
    printf("Exiting normally\nBye...Bye!\n");
}
