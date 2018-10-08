Protobuf serialized server/client

Everything is the same. This time, try to use Protobuf to serialize the data before sending/receiving it to/from the server/client.

Some notes:
1. Both frontend and backend server need to use the provided ProtoPackets.proto file.
2. This ProtoPackets.proto file can change if/when we create the new backend library.

Use this website to help you out:
https://developers.google.com/protocol-buffers/

For tcp-server.js:
Run this file, try to create a backend client with C++ that is able to send messages to the frontend server.

For tcp-client.js:
Try to create a backend server with C++. Run your server, then see if the frontend client sends messages to your server.