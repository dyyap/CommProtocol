#include <iostream>
#include <WS2tcpip.h>
#include <string.h>
#include <string>



// instructions need PUTTY 
// ssh into 127.0.0.1 with port 6969
// Created for NGCP by Daniel Yap 2018 
// Treasurer for ./SCRIPT Daniel Yap 2019

#pragma comment (lib, "ws2_32.lib")

using namespace std;

void InitializeWinsock() {

	// Initialize winsock
	WSAData wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0) { // simple network error 
		cerr << "Can't Initialize winsock! Quitting" << endl;

	}

}

void main() {

	InitializeWinsock();

	// Create a socket
	// socket is an endpoint; number ; in unix it is a file descriptor

	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0); // socket (ip address family, TCP socket, flag for off)
	if (listening == INVALID_SOCKET) { // if socket doesn't work quit
		cerr << "Can't creat a socket! Quitting" << endl;
		return;
	}

	// Bind the socket to an ip adress and port or vice versa
	sockaddr_in hint; // hint data structure https://patents.google.com/patent/WO2017078732A1/en
	hint.sin_family = AF_INET; // IP4
	hint.sin_port = htons(6969);  // host to network short
	hint.sin_addr.S_un.S_addr = INADDR_ANY; // could also use inet_pton

	//bind to socket
	bind(listening, (sockaddr*)&hint, sizeof(hint));



	// tell winsock the socket is for listening
	listen(listening, SOMAXCONN); //marks it for listening


	// wait for connection
	sockaddr_in client; // make a client
	int clientsieze = sizeof(client); // get the size of client

	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientsieze);
	if (clientSocket == INVALID_SOCKET){ // if socket doesn't work quit
		cerr << "Can't create a socket! Quitting" << endl;
		return;
	}
	
	char host[NI_MAXHOST]; // client's remote name , could use DNS lookup, we'll find out
	char service[NI_MAXHOST]; // Service( i.e port) the client is connect on
	
	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXHOST);
	//memset(host, 0, NI_MAXHOST); //portable

	//lookup the host name of the client we are working with
	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
		//DNS lookup
		cout << host << " connected on port " << service << endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << "connected on port " <<
			ntohs(client.sin_port) << endl;
	}
	// close listening socket
	closesocket(listening);

	// while loop: accept and echo message back  to client
	char buf[4096];

	while (true) {
		memset(buf, 0, 4096);

		// wait for client to send data
		
		int bytesReceived = recv(clientSocket, buf, 4096, 0);
		if (bytesReceived == SOCKET_ERROR)
		{
			cerr << "Error in recv(). Quitting" << endl;
			break;
		
		}
		
		if (bytesReceived == 0) {
			cout << "Client disconnected " << endl;
			break;
			
		}

		cout << "Received: " << string(buf ,0, bytesReceived)<< endl;

		send(clientSocket, buf, bytesReceived + 1, 0); // never get the terminating zero

		
		// Echo message back to client
	}
	//close the socket
	closesocket(clientSocket);
	//Shutdown winsock
	WSACleanup();

}


