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

mq_send is used to send the message to the message queue.
mq_send(
mqd_t msgQ,
const char *data,
size_t data_len,
unsigned int msg_prio
) 
Priority should be in the decreament order while pushing msgs in the queue. priority is the non-negative msg.
E.g. m1=2, m2=1, m3=1 --> queue msgs would be m1<-m2<-m3<- , now if m4 come with priority 5 then, m4<-m1<-m2<-m3
Size of the message should be lessthan mq_msgsize, which was set in attributes.

if msgQ is full then, mq_send will wait for some room. it can also return immediately if O_NONBLCOK is set while opening the msgQ. but it sets the errno EAGAIN before returning.

mq_recv is used to receive the message to the message queue.
mq_recv(
mqd_t msgQ, // msgQ handle
char *buffer, // buffer to hold the msg
size_t buf_len, // to hold length of the data being retrieved
int *msg_prio  // to hold msg priority
)
the oldest message of the highest priority would be retrieved.
returns n_bytes of buffere retrieved on success, else -1.

owner can destroy the msgQ using
int
mq_unlink(const char* msgQ_name); // should be called only after closing the message.
unlinking will be deffered till any process has already opened the msgQ and using it or not called mq_close() yet.
-1 on failure and 0 on success.

MsgQ is the N:1 communication.
consumer would be just 1 and producers can be many to a single msgQ. in Linux, there can't be 2 consumer process when it comes to using a msgQ. same message can not be delivered to the 2 consumers at the same time.
Consumer can read from multiple queues using the select() call. // which is the multiplexing using select()
No limitations on the server process.

compilation of message queue 
gcc -g -c rcvr.c -o rcvr.o 
gcc -g rcvr.o -o rcvr -lrt // needs linking of this library.


shared memory :
a common memory where one process put some data and other process can read the data.
there's no direct flow of data from sender to receiver.
there's flow of information(its not the data but the information as actual data is handing over to mediator) from sender to receiver.

PCB
its a diagramatic logical representation of a process and its memory layout in linux/os.
PCB helps to understand how the virtual address space works during course of process execution.

Virtual address space of the program is :
data + heap + stack 
virtual addresses are set of all the addresses which the program can access.
as program uses above sections for data, so these are called virtual address space.

shared memory is the chunk of 'kernel memory'. no process is the owner of this memory.
when a process is created, OS assigns a chunk of virtual memory of the system. this chunk of memory is called process's virtual address space.
a process can access only the memory address which lies within its VAS.
if a process tries to access the address which is beyond its VAS, then segmentation fault will occur.
so A process is never allowed to read/write into any memory which is not part of its Virtual memory region.
virtual memory may grow or shrink as process request for more memory from OS or free the memory.

Kernel has its own memory which is called kernel memory. kernel memory is used for kernel operations like scheduling, user process management, interrupt, timers, etc.
no user space process is the owner of this kernel memory.
a user process can request the kernel to create a kernel memory region. //IMPORTANT
this kernel memory region is the outside of the process' virtual address space. hence no process can access this memory directly.
through shared memory approach, processes can access this kernel memory.

process requests the kernel memory access which is starting from k-k' and which is residing in kernel process virtual address space.
process P will request the kernel to OS to expand its VAS by k-k' bytes.
OS will add k-k' addresses will be allocated to process VAS from global VAS. process borrowed new Virtual addresses from OS.
to do this process will call mmap().  this will add mmap in VAS of process. i.e. vas = stack + heap + data + mmap(few addresses which are exactly mapped to kernel memory of our interest)
this addresses in mmap are mapped using concept of paging.
Actual data continues to stay inside kernel memory region but not in virtual address space.
Pagging does mapping like A-A' to K-K'
Data is not moved to mmap region of process P
When a process accesses the B which is between A-A', then pagging will translate B to some point in K-K' (for every address between A-A' there's corresponding address K-K')
Once the mapping is done process can read/write to kernel memory as if it was part of its own VAS.
Two or more process can do this mapping at the same time with the same chunk of kernel memory segment. like this kernel memory is being shared between two or more processes and hence it is called a shared memory.
Any update to this region will be reflected to other processes.
good for sharing the large files.
Once this shared memory is used, processes should relinquish the posession and it can be done by unmap() call.

when to use shared memory?
1:N mapping should be there.
1. only one process update the shared memory. -- publisher process.
N processes reads the shared memory -- subscriber processes.
2. Publishion should not be very high from publishers. or updates should not be more from writer process.

If we need more writer processes, then it needs synchronization based on mutual exclusion.
synchronization, but, reduces the performance, as we put the threads or processes to sleep instead of natural preemption of the processes.

After updating shared memory, publisher needs to notify all the subscribers.
once these subscribers get notified, they can read the shared memory and may update their data structures if necessary.
these small notifications are generally sent using msgQ and unix sockets.
hence, shared memory needs to be backed by other IPCs.

int
shm_open(char *name_sm, file_flags, file permissions);
returns shared memory file descriptor.

by default shared memory of zero bytes would be created.

resizing of shared memory.
ftruncate(int shm_fd, size_t size);
to resize the created shared memory. hence, it should always be called after shm_open()

Mmap api.
#include<sys/mman.h>
void*
mmap(
void *addr, // mapping starting in process VAS. e.g. A from A-A' . Ideally it should be passed as NULL and let OS decide.
size_t length, // lenght of the shared memory object which needs mapping
int prot, // PROT_READ | PROT_WRITE . Flags will decide operation type in this mapped region.should match with shm_open
int flags, // MAP_SHARED this will help to make the updates to shared memory visible to all processes immediately.
int fd, // shared memory object or fd.
off_t offset // offset from the starting point of the kernel shared memory resource. we should generally pass zero. maps the shared memory object right from the beginning.
);
On success returns the pointer to the VAS of the process, where the shared memory region is mapped.
on failure MAP_FAILED, which is (void *)-1, is returned and error number is set.

Un-mapping api
int
munmap(
void *addr,    // pointer to the starting address of the process mmap VAS. return of mmap.
size_t lenght  // length of mapping which we want to unmmap. should be same as shared memory size. 
);
used to destroy the mapping between process VAS and kernel shared memoroy.
Important : it doesn't destroy the shared memory in the kernel space. it only unmap the mapping.

before munmap() vas = heap + stack + data + mmap
after mmap() vas  = heap + stack + data

unlink the shared memory // used so that we don't run out of memory.
shm_unlink(
cont char * name; // name of the shared memory.
)
de-associate the shared memory with its name.
equivalent to no shared memory exist with same name anymore. name shall be available with new instance of shared memory.
returns -1 on failure and 0 on success.

closing of the shared memory
int
close(int shm_id);
decrease the reference count of the shared memory object maintained in the kernel space
if count reaches to zero then shared memory resource will be reclaimed by the kernel. in some systems it reclaims on reboot.
returns -1 on error or success.
