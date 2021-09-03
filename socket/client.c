#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>


#define SOCKET_NAME "/tmp/DemoSocket"
#define BUFFER_SIZE 128

int
main( int argc, void *argv[]){

    struct sockaddr_un addr;
    int i;
    int ret;
    int data_socket;
    char buffer[BUFFER_SIZE];

    /* communication file descriptoe */
    data_socket = socket(AF_UNIX, SOCK_STREAM, 0);

    if (data_socket == -1){
        perror("Failed to create socket!");
        exit(EXIT_FAILURE);
    }

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    memcpy(addr.sun_path, SOCKET_NAME, sizeof(addr.sun_path)-1);

    ret = connect(data_socket, (const struct sockaddr *) &addr, sizeof(struct sockaddr_un));

    if (ret == -1){
        perror("Connect failed");
        exit(EXIT_FAILURE);
    }

    int input = 0;
    do{
        printf ("Enter data to send for total computation.\n");
        printf ("Enter zero to quit and get final result\n") ;
        scanf("%d", &input);

        ret = send(data_socket, &input, sizeof(int), 0);

        if(ret == -1){
            perror("send");
            exit(EXIT_FAILURE);
        }

    }while(input);

    memset(buffer, 0, sizeof(buffer));
    recv(data_socket, &buffer, sizeof(buffer), 0);
    printf ("Final result is : %s\n", buffer);

    close(data_socket);

    exit(EXIT_FAILURE);
}
