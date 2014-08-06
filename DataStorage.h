#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "CAENVMElib.h" 
#include "CAENVMEtypes.h"


/*
 * This is a class that writes data recieved from the client to the server
 *
 * sockfd is the socket number
 */

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

class DataStorage{
	private:
	public:
		DataStorage();
		int32_t CAENVME_Init(CVBoardTypes BdType, short Link, short BdNum, int32_t *Handle, int32_t sockfd);
		int32_t CAENVME_End(int32_t Handle, int32_t sockfd);
		int32_t CAENVME_WriteCycle(int32_t Handle, uint32_t Address, void *Data, CVAddressModifier AM, CVDataWidth DW, int32_t sockfd);
		int32_t CAENVME_ReadCycle(int32_t Handle, uint32_t Address, void *Data, CVAddressModifier AM, CVDataWidth DW, int32_t sockfd);
};

DataStorage::DataStorage(){
};

int32_t DataStorage::CAENVME_Init(CVBoardTypes BdType, short Link, short BdNum, int32_t *Handle, int32_t sockfd){
	int32_t storage[6], info[2], n;
	storage[0] = 1;    // function identifier
	storage[1] = BdType;
	storage[2] = Link;
	storage[3] = BdNum;
	storage[4] = *Handle;
	storage[5] = 0;
	n = write(sockfd, storage, sizeof(storage));
    	if (n < 0) error("ERROR writing to socket");
    	n = read(sockfd,info,sizeof(info));
    	if (n < 0) error("ERROR reading from socket");
    	unsigned i;
    	if (n != 8) printf("return value is wrong. %d\n",n);
    	printf("Status and Data Values: \n");
    	for(i=0; i<2; i++){
		printf("%d\n",info[i]);
    	}
	*Handle = info[1];
	//if(info[1] != storage[4]) error("function returned invalid value!\n");
	return info[0]; 
}

int32_t DataStorage::CAENVME_End(int32_t Handle, int32_t sockfd){
	int32_t storage[6], info[2], n;
	storage[0] = 2;
	storage[1] = Handle;
	storage[2] = 0;
	storage[3] = 0;
	storage[4] = 0;
	storage[5] = 0;
	n = write(sockfd, storage, sizeof(storage));
    	if (n < 0) error("ERROR writing to socket");
    	n = read(sockfd,info,sizeof(info));
    	if (n < 0) error("ERROR reading from socket");
    	unsigned i;
    	if (n != 8) printf("return value is wrong. %d\n",n);
    	printf("Status and Data Values: \n");
    	for(i=0; i<2; i++){
		printf("%d\n",info[i]);
    	}
	//if (info[1] != storage[1]) error("function returned invalid value!\n");
	return info[0];
}

int32_t DataStorage::CAENVME_WriteCycle(int32_t Handle, uint32_t Address, void *Data, CVAddressModifier AM, CVDataWidth DW, int32_t sockfd){
	int32_t storage[6], info[2], n;
	storage[0] = 3;
	storage[1] = Handle;
	storage[2] = Address;
	storage[3] = *((int32_t*)Data);
	storage[4] = AM;
	storage[5] = DW;
	n = write(sockfd, storage, sizeof(storage));
	if (n < 0) error("ERROR writing to socket");
    	n = read(sockfd,info,sizeof(info));
    	if (n < 0) error("ERROR reading from socket");
    	unsigned i;
    	if (n != 8) printf("return value is wrong. %d\n",n);
    	printf("Status and Data Values: \n");
    	for(i=0; i<2; i++){
		printf("%d\n",info[i]);
    	}
	//printf("Number of Bytes written: %d\n", n);
	//usleep(10000);
	//if(info[1] != storage[3]) error("function returned invalid value!\n");
	return info[0];
}

int32_t DataStorage::CAENVME_ReadCycle(int32_t Handle, uint32_t Address, void *Data, CVAddressModifier AM, CVDataWidth DW, int32_t sockfd){
	int32_t storage[6], info[2], n;
	storage[0] = 4;
	storage[1] = Handle;
	storage[2] = Address;
	storage[3] = *((int32_t*)Data);
	storage[4] = AM;
	storage[5] = DW;
	n = write(sockfd, storage, sizeof(storage));
    	if (n < 0) error("ERROR writing to socket");
    	n = read(sockfd,info,sizeof(info));
    	if (n < 0) error("ERROR reading from socket");
    	unsigned i;
    	if (n != 8) printf("return value is wrong. %d\n",n);
    	printf("Status and Data Values: \n");
    	for(i=0; i<2; i++){
		printf("%d\n",info[i]);
    	}
	//if(info[1] != storage[3]) error("function returned invalid value!\n");
	*((int32_t*)Data) = info[1];
	return info[0];
}
	

