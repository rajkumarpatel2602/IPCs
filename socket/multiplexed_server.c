#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_NAME "/tmp/DemoSocket" // must be unique
#define BUFFER_SIZE 128 // server will use this much buffer to receive data

#define MAX_CLIENT_CONN 32 // 31+1 (Master socket)

int monitored_fd_set[MAX_CLIENT_CONN] = {0};

void
init_monitored_fd_set(){
    int i=0;
    for(;i<MAX_CLIENT_CONN;i++){
        monitored_fd_set[i] = -1;
    }
}

int
add_to_monitored_fd_set(int skt_fd){
    int i=0;
    for(;i<MAX_CLIENT_CONN;i++){
        if(monitored_fd_set[i] != -1){
            continue;
        }else{
            monitored_fd_set[i] = skt_fd;
            break;
        }
    }
}

int
remove_monitored_fd_set(int skt_fd){
    int i=0;
    for(;i<MAX_CLIENT_CONN;i++){
        if(monitored_fd_set[i] == skt_fd){
            monitored_fd_set[i] = -1;
        }else{
            continue;
        }
    }
}

int
main(int argc, char *argv[])
{
    struct sockaddr_un name;
#if 0
    struct sockaddr_un {
        sa_family_t sun_family;               /* AF_UNIX */
        char        sun_path[108];            /* pathname */
    };
#endif

    int ret;
    int connection_socket;
    int data_socket;
    int result;
    int data;
    char buffer[BUFFER_SIZE];

    /*In case the program exited inadvertently on the last run,
     *remove the socket.
     **/

    unlink(SOCKET_NAME);

    /* Create Master socket. */

    /*SOCK_DGRAM for Datagram based communication*/ //XXX
    connection_socket = socket(AF_UNIX, SOCK_STREAM, 0);

    if (connection_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    printf("Master socket created\n");

    /*initialize*/
    memset(&name, 0, sizeof(struct sockaddr_un));

    /*Specify the socket Cridentials*/
    name.sun_family = AF_UNIX;
    strncpy(name.sun_path, SOCKET_NAME, sizeof(name.sun_path) - 1);

    /* Bind socket to socket name.*/
    /* Purpose of bind() system call is that application() dictate the underlying
     * operating system the criteria of recieving the data. Here, bind() system call
     * is telling the OS that if sender process sends the data destined to socket "/tmp/DemoSocket",
     * then such data needs to be delivered to this server process (the server process)*/
    ret = bind(connection_socket, (const struct sockaddr_un *) &name,
            sizeof(struct sockaddr_un));

    if (ret == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    printf("bind() call succeed\n");
    /*
     * Prepare for accepting connections. The backlog size is set
     * to 20. So while one request is being processed other requests
     * can be waiting.
     * */

    ret = listen(connection_socket, 20); //XXX backlogsize = 20, Can queue 20 requests.
    if (ret == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    /* This is the main loop for handling connections. */
    /*All Server process usually runs 24 x 7. Good Servers should always up
     * and running and shold never go down. Have you ever seen Facebook Or Google
     * page failed to load ??*/
    for (;;) {

        /* Wait for incoming connection. */
        printf("Waiting on accept() sys call\n");

        data_socket = accept(connection_socket, NULL, NULL);

        if (data_socket == -1) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        printf("Connection accepted from client\n");

        result = 0;
        for(;;) {

            /*Prepare the buffer to recv the data*/
            memset(buffer, 0, BUFFER_SIZE);

            /* Wait for next data packet. */
            /*Server is blocked here. Waiting for the data to arrive from client
             * 'read' is a blocking system call*/
            printf("Waiting for data from the client\n");
            ret = read(data_socket, buffer, BUFFER_SIZE);

            if (ret == -1) {
                perror("read");
                exit(EXIT_FAILURE);
            }

            /* Add received summand. */
            memcpy(&data, buffer, sizeof(int));
            printf("received data : %d\n", data);
            if(data == 0) break;
            result += data;
        }

        /* Send result. */
        memset(buffer, 0, BUFFER_SIZE);
        sprintf(buffer, "Result = %d", result);

        printf("sending final result back to client\n");
        ret = write(data_socket, buffer, BUFFER_SIZE);
        if (ret == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }

        /* Close socket. */
        close(data_socket);
    }

    /*close the master socket*/
    close(connection_socket);
    printf("connection closed..\n");

    /* Server should release resources before getting terminated.
     * Unlink the socket. */

    unlink(SOCKET_NAME);
    exit(EXIT_SUCCESS);
}
