#include<stdio.h>
#include<signal.h>
#include<stdlib.h>


static void
cb_sigint_sig_handler(int sig){
    printf("SIGINT received from user\n");
    printf("Bye, Bye!\n");
    exit(0);
}

static void
cb_sigabort_sig_handler(int sig){
    printf("abort() called by process itself\n");
    printf("Bye, Bye!\n");
    exit(0);
}


int
main(int args, void *argv[]){
    char input = 0;

    signal(SIGINT, cb_sigint_sig_handler);
    signal(SIGABRT, cb_sigabort_sig_handler);

    printf("signal test\n");
    printf("NOTE: to interrupt anytime press Ctrl+C\n");
    printf("Press 'y' to abort the process =\n");
    scanf("%c", &input);
    if(input == 'y')
        abort();
    printf("Exiting normally\nBye...Bye!\n");
}
