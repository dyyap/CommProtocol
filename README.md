# CommProtocol version 1.0.8


CommProtocol is an adaptable, backend protocol tool for scalable systems. It is intended for abstractly communicating over via TCP/IP, UDP, Serial, and/or wireless network layers, while also giving you the freedom to work with servers and clients seemlessly, high performance. Being a large state machine that handles communications universally, CommProtocol is a fantastic tool to work the backbone of communications with simplicity.

## Compling and run the code in Linux

	To compile the code in Linux, first go to folder the Servercodes, and then compile UNIXServer.cpp and UNIXClient.cpp for server and client respectively
	
	```
	g++ UNIXServer.cpp -o server
	g++ UNIXClient.cpp -o client
	```	

	Run the server code first, then the client code (in two separate terminals)

	```
	./server
	./client
	```	
