To run the JavaScript clients run the following commands:

```
npm install --save-dev protobufjs

# if you run the normal client run the following command
node client.js

# if you run the protobuf client run the following command
node client-with-protobuf.js
```

To compile Server.cpp do this

```
g++ Server.cpp -lboost_system -lpthread -o server
```

and run

```
./server
```
