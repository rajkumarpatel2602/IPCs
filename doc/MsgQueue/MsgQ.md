* MsgQ to communicate between two processes
Enqueue and dequeue operations
Each MsgQ has an unique identifier. which is the string
its a kernel resource so managed and resides inside kernel. its a kernel resource.
MQ creator is the owner or creator of the msgq

- As soon as data is put inside the MQ, reader process will get the notification about it.

- mqd_t is the file descriptor which points to the message queue. its an inbuild datatype.

- APIs
mqd_t mq_open(const char *mq_name, int oflag);
mqd_t mq_open(const char *mq_name, int oflag, mode_t mode, struct mq_attr *attr);

mq name always starts with the forward slash.
