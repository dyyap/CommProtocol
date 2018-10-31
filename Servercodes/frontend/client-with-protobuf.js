/*
 * JS -> C++, Frontend -> Backend
 * To run, do the following scripts:
 *
 * node client-with-protobuf.js
 */

const net = require('net');
const readline = require('readline');
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
  sendMessage();
});

client.on('close', () => {
	if (rl) console.log();
	console.log('Client disconnected');
  if (rl) rl.close();
});

client.on('error', () => {
	if (rl) console.log();
	console.log('No server detected');
  if (rl) rl.close();
});

function sendMessage() {
  const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout,
  });

  rl.question('Type anything to send a packet (-1 to exit): ', msg => {
    rl.close();
    if (msg !== '-1') {
      sendPacket(PACKET);
      sendMessage();
    } else {
      client.end();
    }
  });
}

function sendPacket(packet) {
  protobuf.load('./protobuf/ProtoPackets.proto')
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
