#include ".\client.h"
#include <iostream>

//debug mary maid
#include "..\Objects\MaryMaid.h"
#include "..\Objects\SpaceObject.h"
#include "..\Objects\World.h"
#include "..\Objects\PlayerShip.h"

using namespace std;
#define MAX_NETWORK_BUFFER_SIZE 12000
//how many sends you can do per x miliseconds

const int waitTime = 10;


client::client(void* sharedmem, int sharedSize)
{
	//newData = malloc(MAX_NETWORK_BUFFER_SIZE);
	//memset(newData, 0 ,MAX_NETWORK_BUFFER_SIZE);

	newData = malloc(sharedSize+10);
	memset(newData, 0 ,sharedSize+10);
	
	timeSinceLastSend = 0;
	sharedMemorySize = sharedSize;
	sharedMemory = sharedmem;



	printStack = 0;

	playerID = 0;
	debug = 0;
	clientState = 0;

	//turn the client on to recive messages from the server
	isOn = 1;
}

client::~client(void)
{
	free(newData);
	if(printStack)
		cerr<<"client destruct"<<endl;
	//free(sharedMemory);
}

//recives data from server and stores on the heap
//caller is responcible for freeing the data stored
//on the heap
//returns 0 if connection was closed
//returns -1 if recv failed
//size is the size of the data returned
void* client::reciveDataFromServer(int* size)
{
	

	//static char * recivedData = (char*)(malloc(MAX_NETWORK_BUFFER_SIZE));
	static char * recivedData = (char*)(malloc(sharedMemorySize+10));
		//if(printStack)
		//	cerr<<"client::reciveDataFromServer"<<endl;
		

		//get the size of the data
		int dataSize = 0;

        iResult = recv(ConnectSocket, (char*)(&dataSize), sizeof(int), 0);

		//message recived
        if ( iResult > 0 )
		{

			//recive 2000 bytes at a time
			recvALotOfData(recivedData,	dataSize );
						
			if(debug)
			{
				printf("reciveDataFromServer::Bytes received: %d\n", iResult);
				printf("reciveDataFromServer::BUFFER SIZE: %d\n", dataSize);
				printf("reciveDataFromServer::DATA: %s\n", recivedData);
			}


			*size = dataSize;

			return ((void*)recivedData);
		}
        else if ( iResult == 0 )
		{
			if(debug)
				printf("recivedDataFromServer::Connection closed\n");
			return ((void*)0);
		}
        else
		{
			//if(debug)
	        //    printf("recivedDataFromServer::recv failed: %d\n", WSAGetLastError());

				(*size) = 0;
			return ((void*)-1);
		}
}

void client::recvALotOfData(	char* dataToRecv,	int numBytesToRecv )
{
	while(numBytesToRecv >0)
	{
			//recive Data
			iResult = recv( ConnectSocket, (char*)dataToRecv, numBytesToRecv, 0 );
			numBytesToRecv -= iResult;
			dataToRecv = dataToRecv+ iResult;

	}
}

//updates the memory of the flag object manager to the real object manager
void client::updateMemory()
{

}

//closes the connection
//returns 1 if failed
//returns 0 if not failed
int client::closeConnection()
{
	if(printStack)
		cerr<<"client::closeConnection"<<endl;

	// shutdown the connection since no more data will be sent
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();

	return 0;

}


//This sends data to the server we're connected to
//data is the data we want to send, dataSize is the size of the data we want to send
int client::sendDataToServer(void* data, int dataSize)
{


	//if the current time - old time < 10 miliseconds, wait for the difference
	if( waitTime > (timeGetTime() - timeSinceLastSend) )
		Sleep( (waitTime-(timeGetTime() - timeSinceLastSend)) );
	timeSinceLastSend = timeGetTime();
	
	if(printStack)
		cerr<<"client::sendDataToServer"<<endl;

	//send size
    iResult = send( ConnectSocket, (char*)(&dataSize), sizeof(int), 0 );
	
	//send playerID
    iResult = send( ConnectSocket, (char*)(&playerID), sizeof(char), 0 );
	
    iResult = send(ConnectSocket, (char*)data, dataSize, 0 );


    if (iResult == SOCKET_ERROR) {
        printf("send failed: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();

        return 1;
    }
	else if( iResult < 0)
	{
		printf("Error sending\n");
	}

	if(debug)
		printf("sendDataToServer(void*, int):: Bytes Sent: %ld\n", iResult);

	return 0;
}

//This sends data to the server we're connected to
//data is the data we want to send, dataSize is the size of the data we want to send
//Command is the command that we want to send over
//the datasize is excluding the command
int client::sendDataToServer(void* data, int dataSize, char command)
{
	

	if(printStack)
		cerr<<"client::sendDataToServer"<<endl;

	//set new size of data
	int newDataSize = dataSize+sizeof(char);
	//allocate space for new data
//	newData = malloc(newDataSize);

	//copy command to front of new data
	memcpy ( newData, (const void *)(&command), sizeof (char) );

	//copy rest of the data we want to send to the back of command
	//cast newData to a char* because we need to add 1 byte to it
	memcpy ( ( ((char*)newData) +1), (const void *)(data), dataSize );

	//pass it to the senDataToServer code because we reformated it
	sendDataToServer(newData, newDataSize);
	
	//clean up stuff we sent
	//free (newData);

	if(debug)
		printf("sendDataToServer(void*, int, char):: Bytes Sent: %ld\n", iResult);

	return 0;
}

	/**************************************************************************
	*					doCommand
	*
	*	Description: Does a specified command whenever a message is 
	*				recived by the server.
	*
	*	data:the data recived from the server
	*
	*	dataSize: address to the size of the data that is going to be sent to the 
	*			server
	*
	*	
	*	returns: returns the address to the information that is needed to 
	*			be sent to the client.
	*
	**************************************************************************/

	void* client::doCommand(void* data, int* dataSize, char* command)
	{
	//	cerr<<"Command::"<< (char)((*(command))+'0')<<endl;
	if(printStack)
		cerr<<"client::doCommand"<<endl;

		//keep alive pre-join
		if((*command) == 0)
		{
		
		}
		//keep alive waiting for more players
		else if((*command) == 1)
		{
//			this->playerID = ( * ( (int*)data ) );
		}
		//game just started
		else if((*command) == 2)
		{
		
		}
		//update memory
		else if((*command) == 3)
		{
			//if there is somthing to copy over
			if(dataSize)
			{
				//increment counter to see number of updates to memory per draw
				temp++;
				memcpy (sharedMemory,data, sharedMemorySize);

				//Update shared info to private memory
				///////////////////////////////////////////////////////////////////////////////////////
				
				//player ship
				for(int i =0; i<objMan->numPlayerShip; i++)
				{
					PlayerShip * currShip = &objMan->privStartPlayerShip[i];					 
					currShip->shipFlag |= ((SharedInfoPlayerShip*)currShip->sharedInfo)->playerFlag;
					((SharedInfoPlayerShip*)currShip->sharedInfo)->playerFlag = 0;
				}

				//projectiles
				for(int i =0; i<objMan->numProjectile; i++)
				{
					Projectile * currProj = &objMan->privStartProjectile[i];					 
					currProj->projFlag |= ((SharedInfoProjectile*)currProj->sharedInfo)->projFlag;
					((SharedInfoProjectile*)currProj->sharedInfo)->projFlag = 0;
				}

				//bases
				for(int i=0; i<objMan->numBase; i++)
				{
					Base * curBase = &objMan->privStartBase[i];					 
					curBase->baseFlag |= ((SharedInfoBase*)curBase->sharedInfo)->baseFlag;
					curBase->ownerID  = (int)(((SharedInfoBase*)curBase->sharedInfo)->playerID);

					//if(((SharedInfoBase*)curBase->sharedInfo)->baseFlag)
					//	printf("FOUND FLAG FOR BASE:%d\n",(int)((SharedInfoBase*)curBase->sharedInfo)->baseFlag);
					((SharedInfoBase*)curBase->sharedInfo)->baseFlag = 0;
				}

				//check for number of updates per second
				static int updatesPer	= 0;
				static int lastTime		= timeGetTime();
				int curTime				= timeGetTime();
				if( (curTime - lastTime) > 1000)
				{					
					fprintf(stderr, "Updates per second %d\n", updatesPer);
					updatesPer=0;
					lastTime=curTime;
				}
				updatesPer++;				
				
				//set draw falg
				objMan->draw |= 1;
			}
		}
		else if((*command) == 4)
		{
			playerID = (char) ( * ( (int*)data ) );
		}


		//do not send anything as default
		(*dataSize) = 0;
		return (void*)0;
	}


	//spawns a new thread to lissen for the server continually and 
	//calls the doCommand

//returns 0 for sucess and 1 for failure, connect to a server
//serverAddr is the address of the server you want to connect to
//I.E "127.0.0.1"
//This function connects to the server so you can send/recv
//data via TCP/IP

int client::connectToServer(char * ServerAddr, char * port)
{
	if(printStack)
		cerr<<"client::connectToServer"<<endl;
/////////////////////////////////

    ConnectSocket = INVALID_SOCKET;
    addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;
    iResult;


	////////////////////////////



   // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
//    iResult = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
    iResult = getaddrinfo(ServerAddr, port, &hints, &result);

	
	if ( iResult != 0 ) {
        printf("getaddrinfo failed: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("Error at socket(): %ld\n", WSAGetLastError());
            freeaddrinfo(result);
            WSACleanup();
            return 1;
        }

        // Connect to server.
        iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }

	//spawn new thread to lissen to server
	aThread[0] = CreateThread( 
		NULL,       // default security attributes
		0,          // default stack size
		(LPTHREAD_START_ROUTINE) spawnServerThread, 
		this,       // pass it the current client so the new thread can access instance vars
		0,          // default creation flags
		&ThreadID); // receive thread identifier


	return 0;
}

