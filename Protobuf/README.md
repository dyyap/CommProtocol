```
g++ UNIXServer.cpp ProtoPacketsV2.pb.cc  -std=c++11 `pkg-config --cflags --libs protobuf` -o Server

g++ UNIXClient.cpp ProtoPacketsV2.pb.cc  -std=c++11 `pkg-config --cflags --libs protobuf` -o Client
```
Then run Server first and enter all the numbers before running Client (in different terminal

```
./Server
./Client
```
