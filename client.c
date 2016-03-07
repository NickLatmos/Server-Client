/************CLIENT*************/
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

void readMessage(char *message);
void sendMessage(int pipe,char *message);
void closeConnection(int pipe);

void main(int argc, char **argv){
	
	int pipe;
	char *message;

	/*opening pipe*/
	pipe = open("server_client_pipeline",O_WRONLY); //only for writing
	if(pipe >= 0)
		fprintf(stderr, "CLIENT SUCCESFULLY OPENED THE PIPELINE\n");
	else{
		fprintf(stderr, "CLIENT COULD NOT OPEN THE PIPELINE\nABORT EXECUTION\n");
		exit(1);
	}

	printf("Please write your message to send to the server\n");
	while(1){
		message = (char *)malloc(MESSAGE_SIZE*sizeof(char));
		readMessage(message);
		sendMessage(pipe,message);
		if(message == "logout\n"){
			closeConnection(pipe); //closes the pipe's end from the client's side.
			break;
		}
	}
}

/*This function reads a message from the console*/
void readMessage(char *message)
{
	scanf ("%[^\n]%*c", message);
}

/*This funcion send a message to the server*/
void sendMessage(int pipe,char *message)
{
	int message_size;
	
	//Firstly we have to calculate and send the message size to the server and then the message itself 
	message_size = strlen(message);
	write(pipe, &message_size, sizeof(int));
	write(pipe,message,message_size);

	free(message);
}

void closeConnection(int pipe)
{
	if(close(pipe) == 0) 			
		fprintf(stderr, "--CLIENT-- PIPE WAS SUCCESSFULLY CLOSED\n" );
	else{
		fprintf(stderr, "--CLIENT-- ERROR, PIPE COULD NOT BE CLOSED\n" );
		fprintf(stderr, "ABORT EXECUTION\n");
		exit(1);
	}
}