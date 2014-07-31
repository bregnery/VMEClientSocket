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
	int32_t storage[5], n;
	storage[0] = 1;    // function identifier
	storage[1] = BdType;
	storage[2] = Link;
	storage[3] = BdNum;
	storage[4] = *Handle;
	n = write(sockfd, storage, sizeof(storage));
	return n; 
}

int32_t DataStorage::CAENVME_End(int32_t Handle, int32_t sockfd){
	int32_t storage[2], n;
	storage[0] = 2;
	storage[1] = Handle;
	n = write(sockfd, storage, sizeof(storage));
	return n;
}

int32_t DataStorage::CAENVME_WriteCycle(int32_t Handle, uint32_t Address, void *Data, CVAddressModifier AM, CVDataWidth DW, int32_t sockfd){
	int32_t storage[6], n;
	storage[0] = 3;
	storage[1] = Handle;
	storage[2] = Address;
	storage[3] = *((int32_t*)Data);
	storage[4] = AM;
	storage[5] = DW;
	n = write(sockfd, storage, sizeof(storage));
	//printf("Number of parameters: %d\n", (sizeof(storage)/sizeof(storage[0])));
	//printf("%d\n",storage[2]);
	//printf("Number of Bytes written: %d\n", n);
	usleep(10000);
	return n;
}

int32_t DataStorage::CAENVME_ReadCycle(int32_t Handle, uint32_t Address, void *Data, CVAddressModifier AM, CVDataWidth DW, int32_t sockfd){
	int32_t storage[6], n;
	storage[0] = 4;
	storage[1] = Handle;
	storage[2] = Address;
	storage[3] = *((int32_t*)Data);
	storage[4] = AM;
	storage[5] = DW;
	n = write(sockfd, storage, sizeof(storage));
	return n;
}
	

