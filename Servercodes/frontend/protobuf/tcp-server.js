/*
 * JS <- C++, Frontend <- Backend
 * To run, do the following scripts:
 *
 * npm install --save-dev protobufjs
 * node tcp-server.js
 */

const net = require('net');
const protobuf = require('protobufjs');

const HOST = '127.0.0.1';
const PORT = 6969;

const Packet = protobuf.loadSync('./ProtoPackets.proto').lookup('ProtoPackets.Packet');

const server = net.createServer();
server.listen(PORT, HOST, console.log('Receiving incoming messages...'));

server.on('connection', socket => {
  socket.on('data', printPacket);
});

function printPacket(data) {
  const packet = getPacket(data);
  if (!data) {
    console.log('Received invalid packet, did you serialize it with Protobuf?');
  } else {
    console.log('Received packet:', packet);
  }
}

function getPacket(buffer) {
  const message = Packet.decodeDelimited(buffer);
  const data = Packet.toObject(message);

  for (const packetType in data) {
    return {
      type: packetType,
      data: data[packetType],
    };
  }
  return null;
}

