#pragma once
#ifndef _CLIENT_
#define _CLIENT_


#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

#include <iostream>

//Nick's graphics
#include "../Objects/ObjectManager.h"

using namespace std;



// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib










class client
{
public:
	int temp;

	void* newData;
	int printStack;
	DWORD timeSinceLastSend;

	//0 is off, 1 is on
	int isOn;
	//create thread info for server listen thread
	HANDLE aThread[1];
    DWORD ThreadID;

	WSADATA wsaData;
    SOCKET ConnectSocket;
    struct addrinfo *result;

    int iResult;
	char playerID;
	int debug;

//	void* sharedMemoryBuffer;
	void* sharedMemory;
	int sharedMemorySize;

	client(void* sharedmem, int sharedSize);
	~client(void);

	//0 is prejoined
	//1 is joined but not enough clients
	//2 is playing game
	//3 is game ended
	int clientState;

	
	//Graphics/Game logic
	ObjectManager * objMan;
//	ObjectManager * globalObjMan;


	/**************************************************************************
	*					connectToServer
	*
	*	Description: connects to the server, using TCP/IP.  This 
	*				method also calls doCommand whenever the server
	*				sends a message
	*
	*	address: the ascii form of the adress you want to connect to
	*			example: "127.0.0.1"
	*
	*	port:	The ascii form of the port you want to connect to
	*			example: "8080"
	*
	*	returns: 0 for sucess and 1 for failure
	*
	*
	**************************************************************************/
	int connectToServer(char* address, char* port);


	/**************************************************************************
	*					sendDataToServer
	*
	*	Description: Sends data to the server we're connect to
	*
	*	data:		The data we want to send
	*
	*	dataSize:	The size of the data we want to send
	*
	*	returns:	returns the size of the data we sent or less than 0 if
	*				failed.
	*
	*
	**************************************************************************/
	int sendDataToServer(void* data, int dataSize);


	/**************************************************************************
	*					sendDataToServer
	*
	*	Description: This sends data to the server we're connect to
	*	
	*	data:		The data we want to send
	*
	*	command:	The command we want to send over
	*
	*	dataSize:	The size of the data excluding the size of the command
	*
	*
	**************************************************************************/
	int sendDataToServer(void* data, int dataSize, char command);


	/**************************************************************************
	*					closeConnection
	*
	*	Description: Closes the connection
	*
	*	returns:	1 if failed and 0 if not failed
	*
	*
	**************************************************************************/
	int closeConnection();




	/**************************************************************************
	*					reciveDataFromServer
	*
	*	Description: recives data from the server and stores it on the heap
	*				the caller is responcible for freeing the data stored
	*				on the heap
	*
	*	returns: 0 if connection was closed, -1 if failed or the address
	*			of the data allocated on the heap if sucessfull
	*
	*
	*   size:	 is the size of the data returned
	**************************************************************************/
	void* reciveDataFromServer(int* size);

	//only one thread will be creating threads with this function
	//<TO DO> if we can run multiple clients then add locks.
	static DWORD client::spawnServerThread(LPVOID lpParam)
	{

		client *curClient= (client *)lpParam;
		
		if(curClient->printStack)
			cerr<<"client::spawnServerThread"<<endl;	
		
		while(curClient->isOn)
		{
			int dataSize = 0;
			//get data from server or wait until server gives us data
			void* dataFromServer = 0;
			
			do			
			{
				
				if(curClient->printStack)
					cerr<<"before recive data from server"<<endl;
					dataFromServer = curClient->reciveDataFromServer((&dataSize));
				if(curClient->printStack)
					cerr<<"cafter recive data from server"<<endl;			
			}while(dataSize == 0 );

					
			//printf("reciveDataFromServer::Bytes received: %d\n", dataSize);

			//get command out of dataFromServer (1st byte)
			char command = *((char*)dataFromServer);
	
			//get data from Server
			void* data = ((char*)dataFromServer) +1;
			
			//call doCommand on that data
			void* dataToServer = curClient->doCommand(data, (&dataSize), (&command) );

			//send back the info unless the size is 0
			if(dataSize > 0)
			{
				//send data to server
				curClient->sendDataToServer(dataToServer, dataSize, command);
			}
		
		}

		if(curClient->debug)
			cerr<<"client ended listening to server"<<endl;

		return 0;
	}

	/**************************************************************************
	*					acceptServerCommands
	*
	*	Description: continually recives messages from the server
	*	
	*	port: the port you want your server to run on	
	*   func: the function you want to run every time a client sends
	*		information.  Every time a client sends info, func is run on 
	*       the information that the client gives us.  It takes in a void*
	*		which is the information that the client gives the server and
	*		it returns a void* which is what the host or server gives to
	*		the client.
	*
	*		
	*		openAndAcceptsNewClients will delete the data. 
	*
	**************************************************************************/
	int openAndAcceptNewClients(char* port);


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
	void* doCommand(void* data, int* dataSize, char* command);


	//recives more than 2000 bytes of data
	void recvALotOfData(	char* dataToRecv,	int numBytesToRecv );

	void updateMemory();
	

};

#endif /* _CLIENT_ */
