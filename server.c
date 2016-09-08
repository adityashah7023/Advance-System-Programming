/* Server
Project
Name 	:	Aditya Shah
ID	:	104512003
Section	:	2

Name 	:	Palak Desai
ID	:	104512014
Section	:	2
*/
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <wait.h>

void childProcess(int sd){
        char message[255],temp[255][255];

        while(1){
                int n,pid,i=0,j=0,k=0,token_count=0;
                char *token[255]={NULL};
                if((n=read(sd,message,254))){
                        message[n]='\0';
                }
                else if(n==0){
                        printf("Terminating child.\n");
                        exit(5);
                }
                while(k<strlen(message)+1){
                        if(message[k]==' '){
                                temp[i][j]='\0';
                                i++;
                                j=0;
                                token_count++;
                        }

                        else if(message[k]=='\n'||message[k]=='\0'){
                                break;
                        }
                        else{
                                temp[i][j]=message[k];
                                j++;
                        }
                        k++;
                }
                i=0;

                while(i<=token_count){
                        token[i]=temp[i];
                        i++;
                }
                if(pid=fork()){
                        int s;
                        wait(&s);
                }
                else{
                        dup2(sd,1);
                        dup2(sd,2);
                        if(execvp(token[0],token)<0){
                                write(2,"Error: Command not found\n\n",26);
                                exit(20);
                        }
                }
        }
}

int main(int argc, char *argv[]){
        int sd, portNumber, client, status;
        struct sockaddr_in servAdd;

        if(argc != 2){
                printf("Call model: %s <Port Number>\n", argv[0]);
                exit(0);
        }
	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
                printf( "Cannot create socket\n");
                exit(1);
        }

        servAdd.sin_family = AF_INET;
        servAdd.sin_addr.s_addr = htonl(INADDR_ANY);
        sscanf(argv[1], "%d", &portNumber);
        servAdd.sin_port = htons((uint16_t)portNumber);

        bind(sd, (struct sockaddr *)&servAdd, sizeof(servAdd));

        listen(sd, 5);

        while(1){
                client=accept(sd, (struct sockaddr*)NULL, NULL);
                printf("Got client\n");
                if(!fork())
                        childProcess(client);

                close(client);
                waitpid(0,&status,WNOHANG);
        }
}

