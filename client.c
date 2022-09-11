#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/*for getting file size using stat()*/
#include<sys/stat.h>

/*for sendfile()*/
#include<sys/sendfile.h>

/*for O_RDONLY*/
#include<fcntl.h>

#define COMMAND(cmd) strcmp(command, cmd)==0

int main(int argc,char *argv[])
{
	char choice[256], arg[256]; //Variable choice for handling user choice
	FILE *fd; //File Descriptor
	struct stat obj;  // Obj variable to keep the count 
	char buf[100], command[5], filename[20], *f; //Character Arrays
	int k, size, status;
	int filehandle;

	// TCP Protocol
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == -1){
		printf("[-]socket creation failed\n");
		exit(1);
	}
	printf("[+]socket creation successfully\n");

	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;

	// set server IP address
	server_address.sin_addr.s_addr = INADDR_ANY; // Server IP Address

	// set server port number
	server_address.sin_port = htons(64000); //Server Port

/*==============================================SOCKET-CONNECTION===============================================*/
	// Connect to the server
	int e = connect(sock, (struct sockaddr*)&server_address, sizeof(server_address)); //Create Connection
	if(e == -1){
        perror("[-]Error in Connecting");
        exit(1);
    }
    printf("[+]Connected to server.\n");
	int i = 1;

	while(1)
	{
		/*==============================================DISPLAY-MENU===============================================*/
		printf("\nEnter a choice :\n1- RETR\n2- STOR\n3- APPE\n4- REST\n5- RNFR\n6- RNTO\n7- PWD\n8- MKD\n9- RMD\n10- LIST\n11- CWD\n12- USER\n13- SYST\n14- ABOR\n15- DELE\n16- QUIT\n17- CDUP\n18- REIN\n19- REST\n20- PORT\n21- STAT\n");
		scanf("%s", choice);

		if(strcmp(choice, "USER") == 0){

			// get from system
			int name = system("whoami");
			printf("[+]331 User name ok.\n");
		}

		else if(strcmp(choice, "RETR") == 0){
			printf("Enter filename to RETR: ");

			// Get input from user without white space
			scanf("%s", filename);

			// Copy the string to the buf variable
			strcpy(buf, "RETR ");

			// Concatenate the filename to buf
			strcat(buf, filename);

			// send the buf to the server
			send(sock, buf, 100, 0);

			// recieve the size of the file
			recv(sock, &size, sizeof(int), 0);
			if(!size) {
				printf("No such file on the remote directory\n\n");
				//break;
			}

		else{

			// Allocate the dynamic size
			f = (char *)malloc(size);

			recv(sock, f, size, 0);
			while(1)
			{
				// Create file
				filehandle = open(filename, O_CREAT | O_EXCL | O_WRONLY, 0666);
				if(filehandle == -1){
					sprintf(filename + strlen(filename), "%d", i);//needed only if same directory is used for both server and client
				}
				else {
					break;
				}
			}
			write(filehandle, f, size);

			// close the file pointer
			close(filehandle);
			
			strcpy(buf, "cat ");
			strcat(buf, filename);
			printf("[+]Get file successfully\n");
			system(buf);
		}
    }

		else if(strcmp(choice, "STOR") == 0){
			printf("Enter filename to STOR to server: ");

			// Get the input from user
			scanf("%s", filename);

			// Open file in read only mode
			filehandle = open(filename, O_RDONLY);

			// check file is exist or not
			if(filehandle == -1)
			{
				printf("No such file on the local directory\n\n");
				//break;
			}

		else{

			// Copy the command
			strcpy(buf, "STOR ");
			
			// Concatenate the filename
			strcat(buf, filename);

			// send buf to server
			send(sock, buf, 100, 0);

			// make the stat object
			stat(filename, &obj);

			// get the file size
			size = obj.st_size;

			//send the file size to server
			send(sock, &size, sizeof(int), 0);

			// send complete file to server
			sendfile(sock, filehandle, NULL, size);

			// Receive the sattus from server
			recv(sock, &status, sizeof(int), 0);
			if(status)
				printf("[+]File stored successfully\n"); //File successfully stored in remote host
			else
				printf("[-]File failed to be stored to remote machine\n");

			// Remove the file from client side
			remove(filename);
		}
		}

		else if(strcmp(choice, "APPE") == 0){
			printf("Enter filename to STOR to server: ");

			// Get the input from user
			scanf("%s", filename);

			// Open file in read only mode
			filehandle = open(filename, O_RDONLY);

			// Check file existence
			if(filehandle == -1){
				printf("No such file on the local directory\n\n");
				//break;
			}
		else{

			// Copy the command
			strcpy(buf, "APPE ");

			// Concatenate the filename
			strcat(buf, filename);

			// send buf to server
			send(sock, buf, 100, 0);

			// Make the stat object
			stat(filename, &obj);

			// get the file size
			size = obj.st_size;

			// send the file size to server
			send(sock, &size, sizeof(int), 0);

			// send complete file to server
			sendfile(sock, filehandle, NULL, size);

			// get the status from server
			recv(sock, &status, sizeof(int), 0);
			if(status)
				printf("File stored successfully\n");
			else
				printf("File failed to be stored to remote machine\n");

			// Remove the file from client side
			remove(filename);
		}
		}

		else if(strcmp(choice, "RNFR") == 0){

			// copy the command
			strcpy(buf, "RNFR");

			// send command to server
			send(sock, buf, 100, 0);

			// get the old file name
			printf("Enter the old directory to rename from: ");
			scanf("%s", filename);

			// send old filename to server
			send(sock, filename, 100, 0);

			// get new file name
			bzero(filename, sizeof(filename));
			printf("Enter the new directory name to rename : ");
			scanf("%s", filename);

			// send new filename to server
			send(sock, filename, 100, 0);

			bzero(buf, sizeof(buf));

			// Receive status
			recv(sock, buf, 100, 0);
			printf("The path of the remote directory is: %s\n", buf);
		}

		else if(strcmp(choice, "RNTO") == 0){
			strcpy(buf, "RNTO");
			send(sock, buf, 100, 0);

			printf("Enter the old directory to rename from: ");
			scanf("%s", filename);
			send(sock, filename, 100, 0);

			bzero(filename, sizeof(filename));
			printf("Enter the new directory name to rename : ");
			scanf("%s", filename);
			send(sock, filename, 100, 0);

			bzero(buf, sizeof(buf));
			recv(sock, buf, 100, 0);
			printf("The path of the remote directory is: %s\n", buf);
		}

		else if(strcmp(choice, "PWD") == 0){

			// Copy the command
			strcpy(buf, "PWD");

			// send command to server
			send(sock, buf, 100, 0);

			// get the working directory
			recv(sock, buf, 100, 0);
			printf("The path of the remote directory is: %s\n", buf);
		}
		else if(strcmp(choice,"CDUP") == 0){
			strcpy(buf,"CDUP");
			send(sock,buf,100,0);
			recv(sock,&status,sizeof(int),0);
			if(status)
				printf(" changed to parent directory\n");
			else
				printf(" failed to change directory\n");


		}

		else if(strcmp(choice, "MKD") == 0){

			// Copy the command
			strcpy(buf, "MKD");

			// send command to server
			send(sock, buf, 100, 0);

			printf("Enter the directory name to create SERVER side: ");

			// get input from user
			scanf("%s", filename);
			send(sock, filename, 100, 0);

			bzero(buf, sizeof(buf));

			// Receive what created
			recv(sock, buf, 100, 0);
			printf("The path of the remote directory is: %s\n", buf);
		}

		else if(strcmp(choice, "RMD") == 0){
			strcpy(buf, "RMD");
			send(sock, buf, 100, 0);

			printf("Enter the directory name to delete on SERVER side: ");
			scanf("%s", filename);
			send(sock, filename, 100, 0);

			bzero(buf, sizeof(buf));
			recv(sock, buf, 100, 0);
			printf("[+]Directory Removed Successfully\n");
		}

		else if(strcmp(choice, "DELE") == 0){
			strcpy(buf, "DELE");
			send(sock, buf, 100, 0);

			printf("Enter the file name to delete on SERVER side: ");
			scanf("%s", filename);
			send(sock, filename, 100, 0);

			bzero(buf, sizeof(buf));
			recv(sock, buf, 100, 0);
			printf("[+]File Removed Successfully\n");
		}

		else if(strcmp(choice, "LIST") == 0){
			strcpy(buf, "LIST");
			send(sock, buf, 100, 0);
			recv(sock, buf, 1024, 0);
			printf("The remote directory listing is as follows:\n %s\n\n", buf);
			//system();
		}

		else if(strcmp(choice, "CWD") == 0){
			strcpy(buf, "CD ");
			printf("Enter the path to change the remote directory: ");
			scanf("%s", buf + 3);
			send(sock, buf, 100, 0);
			recv(sock, &status, sizeof(int), 0);
			if(status)
				printf("Remote directory successfully changed\n");
			else
				printf("Remote directory failed to change\n");
		}

		else if(strcmp(choice, "ABOR") == 0){
			strcpy(buf, "ABOR");
			send(sock, buf, 100, 0);
			recv(sock, &status, 100, 0);
			if(status){
				printf("[+]Connection Aborted\nWaiting..\n");
				close(sock);
				exit(0);
			}
			printf("[-]Server failed to close connection\n");
		}

		else if(strcmp(choice, "SYST") == 0)
		{
			strcpy(buf, "SYST");
			send(sock, buf, 100, 0);
			recv(sock, buf, 100, 0);
			printf("%s", buf);
		}

		else if(strcmp(choice, "QUIT") == 0){
			strcpy(buf, "QUIT");
			send(sock, buf, 100, 0);
			recv(sock, &status, 100, 0);
			if(status){
				printf("[+]Connection closed\nQuitting..\n");
				printf("[+]Terminated Successfully...\nGood Bye...\n");
				close(sock);
				exit(0);
			}
			printf("[-]Server failed to close connection\n");
		}

		else
		{
			printf("Internal server error\n");
		}
	}	
	return 0;
}
