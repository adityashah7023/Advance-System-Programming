/* Client
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
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[]){
        char message[255];
        int server, portNumber, n;
        struct sockaddr_in servAdd;

        if(argc != 3){
                printf("Call model: %s <IP Address> <Port Number>\n", argv[0]);
                exit(0);
        }
        if ((server = socket(AF_INET, SOCK_STREAM, 0)) < 0){
                fprintf(stderr, "Cannot create socket\n");
                exit(1);
        }
        servAdd.sin_family = AF_INET;
        sscanf(argv[2], "%d", &portNumber);
        servAdd.sin_port = htons((uint16_t)portNumber);

        if(inet_pton(AF_INET, argv[1], &servAdd.sin_addr)<0){
                fprintf(stderr, "inet_pton() has failed\n");
                exit(2);
        }

        if(connect(server,(struct sockaddr *) &servAdd, sizeof(servAdd))<0){
                fprintf(stderr, "connect() has failed \n");
                exit(3);
        }

        while(1){

                if((n=read(0,message,254))){
                                message[n]='\0';
                                write(server,message,strlen(message)+1);
                }
                else if(n==0){
                        close(server);
                        exit(4);
                }
                else{
                        printf("Error in reading.");
                }

                if((n=read(server,message,254))){
                        message[n]='\0';
                        fprintf(stderr,"%s\n",message);
                }
        }
}
                                          

