#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/*for getting file size using stat()*/
#include<sys/stat.h>

/*for sendfile()*/
#include<sys/sendfile.h>

/*for O_RDONLY*/
#include<fcntl.h>
struct stat st = {0};

int main(int argc,char *argv[])
{
	struct sockaddr_in server, client; //Variable used Server & Client under <sys/socket.h> for server socket address Structure
	struct stat obj; // Obj variable to keep the count 
	socklen_t sock2, len; //For Socket Length
	char buf[100], command[5], filename[20]; //Character Arrays for handling user inputs
	int k, i, size, c;
	int filehandle, sock1; //To handle files and Sock1 for socket commands
	/*================================================SOCKET-CREATION================================================*/
	// TCP Protocol 
	sock1 = socket(AF_INET, SOCK_STREAM, 0); // For UDP you can use SOCK_DGRAM  
	if(sock1 == -1)
	{
		printf("[-]Socket creation failed");
		exit(1);
	}


	struct sockaddr_in server_address;

	server_address.sin_family = AF_INET;

	// Setting port number
	server_address.sin_port = htons(64000); //In case of Binding Error change port number ranging from 1024 to 64000

	// setting IP address
	server_address.sin_addr.s_addr = INADDR_ANY;

	// Binding the socket
	k = bind(sock1,(struct sockaddr*)&server_address, sizeof(server_address));
	if(k == -1) {
		printf("[-]Binding error"); //[-] Shows Negative Approach 
		exit(1);
	}

	// Listen the connections
	k = listen(sock1,10); //At a time socket are open for 10 Connection 
	if(k == -1){
		printf("[-]Listen failed");
		exit(1);
	}

	while(1){
		printf("[+]Waiting for incoming connections\n");
		len = sizeof(client);

		// Accept the incoming connections
		sock2 = accept(sock1,(struct sockaddr*)&client, &len);
		i = 1;
		while(1){
			recv(sock2, buf, 100, 0);
			sscanf(buf, "%s", command);

			if(!strcmp(command,"RETR"))
			{
				// seperate the command and filename
				sscanf(buf, "%s%s", command, filename);
				stat(filename, &obj);

				// open the file in Read Only Mode
				filehandle = open(filename, O_RDONLY);

				// get the file size using st_size function
				size = obj.st_size; 

				// check if file is exist or NOT
				if(filehandle == -1)

					// set the size of file
					size = 0;

				// send the size of file to client
				send(sock2, &size, sizeof(int), 0);

				if(size)
					// send complete file to the client
					sendfile(sock2, filehandle, NULL, size);

				// Remove that file from server side
				remove(filename);
				printf("[+]213 File status REMOVED.\n");
				printf("[+]200 Command okay.\n");
			}

			else if(!strcmp(command, "STOR"))
			{
				int c = 0, len;

				// seperate the command and filename
				sscanf(buf+strlen(command), "%s", filename);

				// receive the file size
				recv(sock2, &size, sizeof(int), 0);
				i = 1;
				while(1){

					// Create the file
					filehandle = open(filename, O_CREAT | O_EXCL | O_WRONLY, 0666);
					if(filehandle == -1){
						sprintf(filename + strlen(filename), "%d", i);
					}
					else{
						break;
					}
				}

				// Dynamically Allocate the size
				char *f = (char*)malloc(size);

				// Receive the complete file from client
				recv(sock2, f, size, 0);

				// Write to file
				c = write(filehandle, f, size);

				// Close the file
				close(filehandle);

				// send status to client
				send(sock2, &c, sizeof(int), 0);
				printf("[+]213 File status OK.\n");
				printf("[+]200 Command okay.\n");
			}

			else if(!strcmp(command, "APPE"))
			{
				int c = 0, len;

				// seperate the command and filename
				sscanf(buf+strlen(command), "%s", filename);

				// receive the file size
				recv(sock2, &size, sizeof(int), 0);
				i = 1;
				while(1)
				{
					// Create file in append mode
					filehandle = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0666);
					if(filehandle == -1) {
						sprintf(filename + strlen(filename), "%d", i);
					}
					else {
						break;
					}
				}

				// Dynamically allocate the size
				char *f = (char*)malloc(size*2);

				// Receive the file from client
				recv(sock2, f, size, 0);

				// Write data to file
				c = write(filehandle, f, size);

				// Close the file
				close(filehandle);

				// send status to client
				send(sock2, &c, sizeof(int), 0);
				printf("[+]213 File status OK.\n");
				printf("[+]200 Command okay.\n");
			}

			else if(!strcmp(command, "RNFR"))
			{
				char old_name[100], new_name[100];

				// get the old file name
				recv(sock2, old_name, 100, 0);

				// get new file name
				recv(sock2, new_name, 100, 0);
	
				// check if directory is created or not
				if (stat(buf, &st) == 1) {
					printf("[-]Directory Does Not EXIST\n");
				}
				else {

					// Rename
					rename(old_name, new_name);
					printf("[+]Successfully rename\n");
				}

				// send status
				send(sock2, new_name, 100, 0);
				printf("[+]200 Command okay.\n");
			}

			else if(!strcmp(command, "RNTO"))
			{
				char old_name[100], new_name[100];
				recv(sock2, old_name, 100, 0);

				recv(sock2, new_name, 100, 0);
	
				// check if directory is created or not
				if (stat(buf, &st) == 1) {
					
					printf("[-]Directory Does Not EXIST\n");
				}
				else {
					rename(old_name, new_name);
					printf("[+]Successfully rename\n");
				}
				send(sock2, buf, 100, 0);
				printf("[+]200 Command okay.\n");
			}
			
			else if(!(strcmp(command,"CDUP")))
			{
				if(chdir("..") == 0)
					c = 1;
				else
					c = 0;
				send(sock2,&c,sizeof(int),0);
				printf("200 Command okay");
			}

			else if(!strcmp(command, "PWD")){
				system("pwd > PWD.txt");
				i = 0;

				// Open file in reading mode
				FILE*f = fopen("PWD.txt", "r");

				// Read until end of file
				while(!feof(f)){

					// copy the PWD
					buf[i++] = fgetc(f);
				}

				// add the null chracter at the end
				buf[i-1] = '\0';

				// close file pointer
				fclose(f);

				// send the PWD result
				send(sock2, buf, 100, 0);
				printf("[+]200 Command okay.\n");
			}

			else if(!strcmp(command, "MKD"))
			{
				recv(sock2, buf, 100, 0);
	
				// check if directory is created or not
				if (stat(buf, &st) == -1) {

					// make new directory
					mkdir(buf, 0777);
					
					printf("257 %s created.\n", buf);
				}
				else {
					printf("[-]Unable to create directory\n");
					exit(1);
				}
				send(sock2, buf, 100, 0);
				printf("[+]200 Command okay.\n");
			}

			else if(!strcmp(command, "RMD"))
			{
				recv(sock2, buf, 100, 0);
	
				// check if directory is created or not
				if (stat(buf, &st) == 1) {
					printf("[-]Directory Does Not EXIST\n");
				}
				else {
					remove(buf);
					printf("[+]Successfully remove directory\n");
				}
				send(sock2, buf, 100, 0);
				printf("[+]200 Command okay.\n");
			}

			else if(!strcmp(command, "DELE"))
			{
				recv(sock2, buf, 100, 0);
	
				// check if directory is created or not
				if (stat(buf, &st) == 1) {
					printf("[-]Directory Does Not EXIST\n");
				}
				else {
					remove(buf);
					printf("[+]Successfully remove file\n");
				}
				send(sock2, buf, 100, 0);
				printf("[+]200 Command okay.\n");
			}

			else if(!strcmp(command, "LIST"))
			{
				system("ls > LIST.txt");
				i = 0;
				FILE*f = fopen("LIST.txt","r");
				while(!feof(f)){
					buf[i++] = fgetc(f);
					//buf[i++] = "\n";
				}
				buf[i-1] = '\0';
				fclose(f);
				send(sock2, buf, 1024, 0);
				printf("[+]200 Command okay.\n");
			}

			else if(!strcmp(command, "CD"))
			{
				if(chdir(buf+3) == 0)
					c = 1;
				else
					c = 0;
				send(sock2, &c, sizeof(int), 0);
				printf("[+]200 Command okay.\n");
			}

			else if(!strcmp(command, "SYST")){
				char syst[100];
				#ifdef _WIN32
					strcpy(syst, "It seems that you are working on a Windows OS.\n\n\n");
				#elif __APPLE__
					strcpy(syst, "It seems that you are working on a Mac OS.\n\n\n")
				#elif __linux__
					strcpy(syst, "It seems that you are working on a Linux OS.\n\n\n");
				#endif

				send(sock2, syst, 100, 0);
				printf("[+]200 Command okay.\n\n");
			}
			
			else if(!strcmp(command, "BYE") || !strcmp(command, "QUIT"))
			{
				printf("[+]226 Closing client data connection.\n");
				i = 1;
				send(sock2, &i, sizeof(int), 0);
				break;
			}
			else{
				printf("[-]502 Command not implemented.\n");
			}
		}
	}

	return 0;
}
