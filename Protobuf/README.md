```
g++ UNIXServer.cpp ProtoPacketsV2.pb.cc  -std=c++11 `pkg-config --cflags --libs protobuf` -o Server

g++ UNIXClient.cpp ProtoPacketsV2.pb.cc  -std=c++11 `pkg-config --cflags --libs protobuf` -o Client
``` 
Then run Server first and enter all the numbers before running Client (in different terminal)

For every changes to UNIXServer.cpp or UNIClient.cpp need to run those above two commands again.

this push gonna make the server open and waiting at first untill the client connect. 

after the client connects can start sending messages.

./Server
./Client


