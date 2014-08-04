#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "CAENVMElib.h" 
#include "CAENVMEtypes.h"
#include "DataStorage.h"

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    //int32_t info[2];

    //char buffer[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    printf("Connection Successful! ");
    //printf("Please enter the message: ");
    //bzero(buffer,256);
    //fgets(buffer,255,stdin);
    //n = write(sockfd,buffer,strlen(buffer));
    CVBoardTypes BT = cvV1718;
    CVAddressModifier AM = cvA16_S;
    CVDataWidth DW = cvD8;
    DataStorage dataStore;
    int l;
    for(l=0;l<100;l++){
	    int32_t Data = l;
	    printf("Sending Data! \n");
	    n = dataStore.CAENVME_WriteCycle(5,4,&Data,AM,DW,sockfd);
	    n = dataStore.CAENVME_Init(BT,2,3,&Data,sockfd);
	    n = dataStore.CAENVME_End(Data,sockfd);
	    n = dataStore.CAENVME_ReadCycle(5,4,&Data,AM,DW,sockfd);
	    if (n < 0) 
	         error("ERROR writing to socket");
	    //bzero(buffer,256);
	    //n = read(sockfd,info,sizeof(info));
	    if (n < 0) 
	         error("ERROR reading from socket");
    }
    usleep(10000);
    close(sockfd);
    return 0;
}
