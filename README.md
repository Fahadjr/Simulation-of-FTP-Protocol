# Simulation-of-FTP-Protocol

There two programs server and client in C to simulate the FTP protocol. 
Both of your programs are in  CLI . Our goal is not to replicate a fully FTP client/server but we want to simulate the main functions of the FTP protocol:
• Your server and your client must communicate using the FTP protocol. You must implement the commands (check the link below for a description of the commands)
o USER (with no parameter, only to open the connection) o CWD
o CDUP
o REIN
o QUIT
o PORT (modify it to represent a data transfer named pipe or file instead of
port) o RETR o STOR o APPE o REST
o RNFR o RNTO o ABOR o DELE o RMD o MKD o PWD o LIST
o STAT o NOOP
• Your server must start running before the client, and wait for a connection from the client. To simplify the communication between clients and server, you can use named pipe or files of your choice instead of sockets to establish the connection. If the user press CTRL+C or CTRL+Z on the server, the server must shutdown all the connections properly before leaving. If the user press CTRL+C or CTRL+Z on the client, the client must disconnect properly from the server.
• Your server must manage several clients in the same time.
• A client must be able to navigate through the server filesystem. To make the implementation easy, you can consider that the server manages only one directory (given in parameters when you run the server – eg when you run the command ./server -d homeDirectory) instead of one home directory per user. All the clients have the same read/write permissions on the files. You can also consider that two clients won’t never try to write/read the same file simultaneously.
• When you run a client, the client can transfer only files present in the working directory (the client cannot navigate in the client filesystem – eg, you can change the current folder of the client).
• Your client must be able to download and upload files on the server (files that you transfer to the server are stored in the home directory of the server, files that you download are stored in the working directory of the client), create subdirectories, navigate in subdirectories...
• You must simulate only the stream mode.
• The list of commands for you client and server are described in the RFC 959
(especially section 4 of the RFC).
• See section 5.4 of the RFC for the sequence of commands
• Case of the STOR and RETR commands:
o In both cases, you server can open an additional file or named pipe for the transfer of the data (different of the file/named pipe used to exchange commands).
• You must implement completely your client & server in C (no call to any shell command is allowed).
Example of scenario (white are automatic processes, red are commands typed by the user)
   Server Client
     Run the server: ./server -d /home/user
• The server runs using
/home/user as current directory
• The server is ready to listen for
connections.
        Run the client: ./client
• The client runs using the current
directory as working directory
• The client tries to open a connection
to the server
  Server accepts the connection and open a specific channel (named pipe or file or socket) to discuss with the client. Server sends the name of the channel to the client.
         For the steps before, you can use the file or named pipe of your choice to send/retrieve data.
Client sends USER.
   Client opens the specific channel to communicate.
     
Server sends : 230 User logged in, proceed.
      Client sends LIST.
Client sends : PORT “unix1”
Client sends LIST.
Server sends : 425 Can't open data connection.
      Server opens named pipe “unix1”
Server sends : 200 Command okay.
      Server sends:
    150 File status okay; about to open
data connection.
125 Data connection already open;
transfer starting.
Server writes the list of files in the pipe
“unix1”.
250 Requested file action okay,
completed.
Server sends:
       Client reads the pipe “unix1”
Client displays on the screen the list of file. Client sends QUIT.
   Server sends: 221 Service closing control connection. Logged out if appropriate.
Server closes the connection.
       Client exits.
Links : RFC959 - https://www.w3.org/Protocols/rfc959/
List of parameters of the server:
• -d <homeDir> : use homeDir as a directory to store files and retrieve files; if -d is not provided, the server uses the working directory.
List of parameters of the client:
• N/A Hints:
• This project is enough comprehensive to male you use all the concepts that you learnt in this course: Memory Management, File Management, Pointer-to-Functions, Process Control, Fork, Signals, Threads and Sockets.
• We’ll pay attention to the structure of your code (don’t put everything in one single function). For example, you can use a handler or a callback for each command.
  
• Don’t forget to clean all the temporary files and pipes (if used). Don’t forget to clean correctly the memory in case of dynamic allocation. Don’t forget to close files properly.
• You can use threads instead of multiple processes if you feel enough confident to use multiple threads.
• You can test your program on alpha and bravo server.
• We would like you to use sockets in order to implement the communication between client & server (whatever if the client and the server are running on the same machine or not). However, if you’re not able to implement your client and server using sockets (because of the network rules), you can use named pipes or files instead of sockets and make your programs work at least on a local machine.
