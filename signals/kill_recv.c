#include<stdio.h>
#include<signal.h>
#include<stdlib.h>


static void
cb_sigusr_sig_handler(int sig){
    printf("signal %d received from process kill_sender\n", sig);
    printf("Bye, Bye!\n");
    exit(0);
}

int
main(int args, void *argv[]){
    int input;
    signal(SIGUSR1, cb_sigusr_sig_handler);
    printf("kill() signal test\n");

    //make process wait..
    scanf("%d", &input);
}
