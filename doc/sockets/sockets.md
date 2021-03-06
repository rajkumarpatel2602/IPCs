* What is socket?
interface available for different entities(processes) in unix/linux os.
socket interface means bunch of socket apis.

* Types of sockets
Unix domain sockets : IPCs between processes running on same machines
Network sockets : IPCs for processes running on different physical machienes.

* Socket programming steps
Remove any socket if present
Create an UNIX socket using socket()
Specify the socket name
Bind the socket using bind()
listen() on the socket
accept() new connections
Read data using recvfrom()
Send back data using sendto()
close the data socket
close the connection socket
Remove the socket
Exit

* What are the socket systecalls in socket interface or socket layer?
socket, bind, listen, accept, recvfrom, sendto, close

* socket message types
connection initiation request message (CIR)
only client process uses these type messages for dedicated connection.
serivce requeset message (SR)
once connection is established client can send SR messages to server to provide service.
Response message (RM)
Server prepares the response after SR from server and respond with RM.

* Socket design / state machine for client-server communication using socket
On boot up server creates connection socket or master socket file descriptor using socket()
M = socket()
New client request comes to Master socket FD with a CIR and OS will reach out to M and M 
will create a client handle(C1) for future communications. like that for new client C2 will be created.
and store in database.
Master socket is the mother of all client handles. e.g. C1, C2, etc.
C1, C2 are also called as data sockets.
C1 sends now SRM to server. server will divert this request to client handle corresponding to that client.
C1 will process request and respond as RM.
M only creates client handles.
Cx handles client requests and responsible for data exchange with clients.
accept() creates the client handles.
accept is used for connection establishment and creating client handles.
client handles = data sockets, communication fd
master sockets = connection socket
OS helps to divert SRM to client handles and CIR to Master socket.

* Accept system call.
Whenver server receives CIR from client, next step is to accept() to establish bidirectional communication.
Which returns client handle/data handle for data exchange.
Only used in TCP (connection oriented communication rather than connection-less communication.)

* UNIX Domain Sockets
Same machine processes uses this type of sockets.
Server and Client both are on the same machine.
STREAM based or DATAGRAM based communciation can take place.

* Stream based communication
Same as flow of water. copying or moving one file from one location to another location.
e.g. flow of water and continuous flow of bytes.

* Datagram based communication
Small units of data moved from source to destination process. data is sent as a chunk.

* Server process UNIX based socket communication.
socket()
bind()
listen()
accept() -- blocking call --> callback* 
read() --> callback*
write()
close()

* Client workflow
socket()
connect()
write()
read() --> blocking

* Pairing of APIs
read <-> write
sendto <-> recvfrom
sendmsg <-> recvmsg
connect <-> accept
Non-blocking <-> Blocking

* Multiplexing
Server design which allows to serve multiple clients at a time.
select() helps to monitor all clients activities at the same time.

* Select system call
Allows to monitor new CIR from a new client and can also monitor multiple connected clients and which clients has sent data.
Server process maintains Master socket as well as data sockets/communication FDs.
fd_set datastructure helps to maitain all these fild des.
e.g. fd_set has M, C1, C2.
select() operates on fd_set.
select() is the blocking system call.
 Unblocks when CIR received
 Unblocks when data request from existing client comes. server need to identified the client handle due to which it has unblocked.

* *fd_set* data structure
C provides data structures which is the collection(set) of file descriptors.

* Multiplexed Server workflow
server boots up
create a master socket using socket()
store master socket in fd_set data structures. i.e.fd_Set readfs
invoke select(fd_set) and gets blocked.
as soon as CIR received, select gets unblocked and as its a CIR master fd gets activated. server process hence knows that its a CIR.
accept() gets called and data socket or communication socket is returned.
server adds this fd in fd_set datastructure and gtets block on select.
if SR is arrived, then selects gets unblcoked. master fd is not activated, instead comm_fd will get activated.
Server identifies the comm_fd or data_fd or client handle and serve the client.
If server closes the client connection then remvoe entry from readfs or fd_set data structure.
Get block on select(updated fd_set)
