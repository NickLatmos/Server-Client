/************SERVER*************/
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stddef.h>
#include <fcntl.h>
#include <string.h>

#define MESSAGE_SIZE 60

char *readClient(int pipe,char *message);
void serverWrite(char *message);
void closeConnection(int pipe);

void main(int argc, char **argv){
	
	char *message;
	int pipe;

	message = (char *)malloc(MESSAGE_SIZE*sizeof(char));
	pipe = mkfifo("server_client_pipeline",0666);   //creating fifo (or named pipe). Variable pipe is just an integer, the fifo pipe is the server_client_pipeline
	if(pipe == -1){
		fprintf(stderr, "ERROR, PIPE COULD NOT BE CREATED\n");
		exit(1);
	}
	else
		fprintf(stderr, "PIPE CREATED SUCCESSFULLY\n");

	/*Now that we have created the fifo pipe, we have also to open it from the server's end*/
    fprintf(stderr, "OPENING FIFO PIPE\nWaiting for client to connect...\n");
    pipe = open("server_client_pipeline", O_RDONLY);     //Open for reading only. 
    if(pipe >= 0)						//Upon successful completion, open function return a non-negative integer
    	fprintf(stderr, "SERVER READY\n");
    else{
    	fprintf(stderr, "ERROR, CANNOT OPEN FIFO PIPE\nABORT EXECUTION\n");  
    	exit(1);
    }

    while(1){
	    message = readClient(pipe,message);
	    if(strcmp(message,"logout") == 0){
	    	closeConnection(pipe);
	    	break;
	    }
	    serverWrite(message);
	}

}

/*This function reads the message which the client sends*/
char *readClient(int pipe,char *message)
{
	int message_size;

	/*Firstly we have to read the message size the client sends and after that the message itself*/
	read(pipe, &message_size, sizeof(int));
	read(pipe,message,message_size);
	message[message_size] = '\0';
	return message;
}

/*This function writes in the screen the received message from the client*/
void serverWrite(char *message)
{
	printf("Received message : %s\n" ,message);
}

/*This functions closes the connection between the server and the client by closing the pipe and unlinking the pipe*/
void closeConnection(int pipe)
{
	if(close(pipe) == 0) 			
		fprintf(stderr, "PIPE WAS SUCCESSFULLY CLOSED\n" );
	else{
		fprintf(stderr, "ERROR, PIPE COULD NOT BE CLOSED\n" );
		fprintf(stderr, "ABORT EXECUTION\n");
		exit(1);
	}

	if(unlink("server_client_pipeline") == 0 )   //Upon successful completion, 0 shall be returned
		fprintf(stderr, "SERVER IS TURNED OFF\n");
	else{
		fprintf(stderr, "ERROR, SERVER COULD NOT BE TURNED OFF\n");
		fprintf(stderr, "ABORT EXECUTION");
		exit(1);
	}
}