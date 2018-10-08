/*
 * JS -> C++, Frontend -> Backend
 * To run, do the following scripts:
 *
 * npm install --save-dev protobufjs
 * node general-client.js
 */
 
const net = require('net');
const protobuf = require('protobufjs');

const HOST = '127.0.0.1';
const PORT = 6969;
const PACKET = {
  VehicleInertialState: {
    vehicleid: 6,
    latitude: 34.05589,
    longitude: -117.819964,
  },
};

const client = net.connect(PORT, HOST);

client.on('connect', () => {
  console.log('Client connected, sending packet...');
  sendPacket(PACKET);
});

client.on('close', () => console.log('Client disconnected'));

client.on('error', () => console.log('No server detected'));

function sendPacket(packet) {
  protobuf.load('./ProtoPackets.proto')
    .then(root => {
      const Message = root.lookupType('ProtoPackets.Packet');

      const e = Message.verify(packet);
      if (e) throw Error(e);

      const message = Message.create(packet);
      const buffer = Message.encodeDelimited(message).finish();
      client.write(buffer);
    })
    .catch(console.error);
}