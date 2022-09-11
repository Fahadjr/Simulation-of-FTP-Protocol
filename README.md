# FTP
Simulation of File Transfer Protocol

# Description
The aim of this project is to implement FTP application between a server and a client. The application is a console based application and is implemented using Unix Sockets. The project is developed in C language.

File Transfer Protocol (FTP) is an application layer protocol which moves files between local and remote file systems. 
When a FTP session is started between a client and a server, the client initiates a TCP connection with the server side. 
The client has to do FTP login, FTP login uses normal username and password scheme for granting access. The username is 
sent to the server using the USER command, and the password is sent using the PASS command. If the information provided 
by the client is accepted by the server, the server will send a greeting to the client and the session will commence.

Note: Fork() is used to create multiclient environment.

**Implemented commands are**
	
	1.  USER – This command sends the user identification to the server.
	2.  CWD  – This command allows the user to change the current working directory to the path specified with the  command.PASS – This command sends the user password to the server.
	3.  CDUP - The CWD command is issued to change the client’s current working directory to the path specified with the command. FTP Voyager and other GUI-based FTP clients will      automatically issue these commands as the user browses the remote file system from within the program.
	4. REIN - This command Reinitialize Session between Systems  
	5. QUIT – This command terminates a USER and if file transfer is not in progress, the server closes the connection. 
	6. PORT - The PORT command is issued by the client to initiate a data connection required to transfer data (such as directory listings or files) between the client and server. The PORT command is used during "active" mode transfers.  
	7. RETR – This command send the requested file over the data connection.
	8. STOR – This command causes to store a file into the current directory of the remote host.
	9. APPE - A client issue the APPE command after successfully establishing a data connection when it wishes to upload data to the server. The client provides the file name it wishes to use for the upload. If the file already exists on the server, the data is appended to the existing file. If the file does not exist, it is created.
	10.REST - The REST command is used to specify a marker to the server for the purposes of resuming a file transfer . Issuing the REST command does not actually initiate the transfer. After issuing a REST command, the client must send the appropriate FTP command to transfer the file. The server will use the marker specified in the REST command to resume file transfer at the specified point.
	11.RNFR - The RNFR command is issued when an FTP client wants to rename a file on the server. The client specifies the name of the file to be renamed along with the command. After issuing an RNFR command, an RNTO command must immediately follow.
	12.RNTO - The RNTO command is used to specify the new name of a file specified in a preceding RNFR (Rename From) command.
	13.ABOR -  This command tells the server to abort the previous FTP service command and any associated transfer of data.
	14.DELE - The DELE command is used to delete the specified file from the server. To delete a directory, use the RMD command.
	15.RMD - This command causes the directory specified in the path-name to be removed as a directory.
	16.MKD - This command causes the directory specified in the path name to be created as a directory.
	17.PWD - This command causes the name of the current working directory to be returned in the reply.
	18.LIST - Sends a request to display the list of all the files present in the directory.
	19.STAT - If the STAT command is issued during a file transfer, information about the current file transfer is sent to the client. If a path name is provided with the command, it is analogous to the LIST command except for the file information for the specified pathname is sent over the command connection instead of a data connection. A partial pathname can be provided instead, in which case the server will respond with a list of file names or attributes associated with that specification. If no parameter is provided and a file transfer is not in progress, the server will respond with general status information about the FTP server and the current connection.
	20.NOOP - The NOOP command does not cause the server to perform any action beyond acknowledging the receipt of the command. This command can be issued to the server to prevent the client from being automatically disconnected for being idle. It can also prevent modern routers/firewalls from closing a connection that it perceives as being idle as well.
	

**Some of FTP replies used are**

	1.  200	-  Command okay.
	2.  530 -  Not logged in.
	3.  331 -  User name okay, need password.
	4.  225 -  Data connection open; no transfer in progress.
	5.  551 -  Requested action aborted: page type unknown.
	6.  502 -  Command not implemented.
	7.  503 -  Bad sequence of commands.
	8.  501 - Syntax error in parameters or argument.
	9.  500 - Syntax error, command unrecognized.
	10. 504 - Command not implemented for that parameter.

# Download and run
    $ git clone https://github.com/Fahadjr/Simulation-of-FTP-Protocol.git
	$ cd FTP
	
#### Terminal-1
	$ cd FTP-Server
	$ make
	$ ./ftpserver	

#### Terminal-2(Can Open Multiple Terminals-as the simulation supports multi-client environment)
	$ cd FTP-Client
	$ make
	$ ./ftpclient 127.0.0.1
