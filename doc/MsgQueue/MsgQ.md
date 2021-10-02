- MsgQ to communicate between two processes
enqueue and dequeue operations
each MsgQ has an unique identifier. which is the string
its a kernel resource so managed and resides inside kernel. its a kernel resource.

MQ creator is the owner or creator of the msgq

As soon as data is put inside the MQ, reader process will get the notification about it.

mqd_t is the file descriptor which points to the message queue. its an inbuild datatype.

mqd_t mq_open(const char *mq_name, int oflag);
mqd_t mq_open(const char *mq_name, int oflag, mode_t mode, struct mq_attr *attr);
mode is set by the owner, which defines the permissions given to other processes. 0660 is the most common.
 
mq_msgsize // /proc/sys/fs/mqueue/msgsize_max -- 10 by default
mq_maxmsg // /proc/sys/fs/mqueue/msg_max -- 10 by default

mq name always starts with the forward slash.

- e.g.
mqd_t 
mq_open("/server-msg-q", O_RDONLY | O_CREAT | O_EXCL, 0660, NULL); // attributes are set to null.

struct mq_attr attr;
attr.mq_maxmsg = 10 
attr.mq_msgsize = 4096
attr.mq_flags = 0
attr.mq_curmsgs = 0;

mqd_t 
mq_open("/server-msg-q", O_RDONLY | O_CREAT | O_EXCL, 0660, &attr);// permissions and flags are important while creating or for owner.

int
mq_close(mqd_t msgQ);

if process closes msgQ it can not be used by that open before mq_open()
OS will remove or destroy the msgQ if it is not opened by any process at an instance. just like file is not opened by any process, so it destroys.
OS does reference counting on msgQ, which means it maintains count rearding how many processes has opened the msgQ.
if reference count is zero at anypoint then kernel claims the resource back. it can be socket fd, msgQ fd or any kernel resource.
