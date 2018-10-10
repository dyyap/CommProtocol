/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdlib.h>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#define PORT 59000
using namespace std;

int main(int argc, char *argv[])
{
    //Create a listening socket
    int listening = socket(AF_INET, SOCK_STREAM, 0);
    if(listening == -1){
        cerr << "Can't create socket!";
        return -1;
    }

    //Bind socket
    sockaddr_in hint;
    hint.sin_family = AF_INET;
        //Convert host port to network address
    hint.sin_port = htons(PORT);
        //Run any IP
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);
        //Check
    if(bind(listening, (sockaddr*)&hint, sizeof(hint)) == -1){
        cerr << "Bind fail";
        return -2;
    }
    //Mark socket for listening
    if(listen(listening, SOMAXCONN) == -1){
        cerr << "Can't listen";
        return -3;
    }
    //Accept a call
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);
    char host[NI_MAXHOST];
    char srv[NI_MAXSERV];

    int clientSocket = accept(listening, (sockaddr*)&client,&clientSize);
    if(clientSocket == -1){
        cerr << "Problem in connecting to client";
        return -4;
    }

    //Close the listening socket
    close(listening);
    memset(host, 0, NI_MAXHOST);
    memset(srv, 0, NI_MAXSERV);

    int result = getnameinfo((sockaddr*)&client, clientSize, host,
                                NI_MAXHOST, srv, NI_MAXSERV,0);

    if(result){
        cout << host << " connected on " << srv << endl;
    } else{
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        cout << host << " connected on " << ntohs(client.sin_port) << endl;
    }
    //Receive and display/echo
    char buffer[4096];
    while(true){
        //Clear the buffer
        memset(buffer, 0, 4096);
        //Wait for message
        int bytesRecv = recv(clientSocket, buffer, 4096, 0);
        if(bytesRecv == -1){
            cerr << "Receive error" << endl;
            break;
        }
        if(bytesRecv == 0){
            cout << "The client disconnected" << endl;
            break;
        }

        //Display message
        cout << "Received: " << string(buffer, 0, bytesRecv) << endl;
        //Resend message
        send(clientSocket, buffer, bytesRecv + 1, 0);
    }
    //Close
    close(clientSocket);
    return EXIT_SUCCESS;
}
