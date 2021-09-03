* What is socket?
- interface available for different entities(processes) in unix/linux os.
- socket interface means bunch of socket apis.

* Types of sockets
- Unix domain sockets : IPCs between processes running on same machines
- Network sockets : IPCs for processes running on different physical machienes.

* Socket programming steps
- Remove any socket if present
- Create an UNIX socket using socket()
- Specify the socket name
- Bind the socket using bind()
- listen() on the socket
- accept() new connections
- Read data using recvfrom()
- Send back data using sendto()
- close the data socket
- close the connection socket
- Remove the socket
- Exit

* What are the socket systecalls in socket interface or socket layer?
- socket, bind, listen, accept, recvfrom, sendto, close

* socket message types
- connection initiation request message (CIR)
-- only client process uses these type messages for dedicated connection.
- serivce requeset message (SR)
-- once connection is established client can send SR messages to server to provide service.
- Response message (RM)
-- Server prepares the response after SR from server and respond with RM.

* Socket design / state machine for client-server communication using socket
- On boot up server creates connection socket or master socket file descriptor using socket()
-- M = socket()
- New client request comes to Master socket FD with a CIR and OS will reach out to M and M 
will create a client handle(C1) for future communications. like that for new client C2 will be created.
and store in database.
- Master socket is the mother of all client handles. e.g. C1, C2, etc.
- C1, C2 are also called as data sockets.
- C1 sends now SRM to server. server will divert this request to client handle corresponding to that client.
- C1 will process request and respond as RM.
- M only creates client handles.
- Cx handles client requests and responsible for data exchange with clients.
- accept() creates the client handles.
- accept is used for connection establishment and creating client handles.
- client handles = data sockets, communication fd
- master sockets = connection socket
- OS helps to divert SRM to client handles and CIR to Master socket.

* Accept system call.
- 
