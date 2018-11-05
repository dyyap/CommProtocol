/*
  THIS WILL NOT BE USED IN THE FINAL PROJECT,
  
  I AM ONLY INCLUDING THIS FOR TEST PURPOSES FOR YOU
  WHO WANT TO SEE HOW JS CLIENT INTERACTS WITH THEORETICAL
  SERVER (CURRENTLY WRITTEN ALSO IN JS)
  
  THIS IS HOW THE C++ SERVER SHOULD BE WRITTEN. THE SERVER SHOULD
  CREATE THE HOST:PORT CONNECTION OPEN TO ANY CLIENT RIGHT AFTER
  THE SERVER IS OPENED, THEN TAKE IN ANY INCOMING MESSAGES AND SEND
  MESSAGES AS NEEDED. YOU CAN SEND MESSAGES MANUALLY (in intervals, user input, etc.)
 */

const net = require('net');
const readline = require('readline');
const protobuf = require('protobufjs');

const HOST = '127.0.0.1';
const PORT = 59000;

const PACKET = {
  VehicleInertialState: {
    vehicleid: 6,
    latitude: 34.05589,
    longitude: -117.819964,
  },
};

const Packet = protobuf.loadSync('./ProtoPacketsV2.proto').lookup('ProtoPackets.Packet');

const server = net.createServer();
server.listen(PORT, HOST, console.log('Ready'));

let client, rl;

server.on('connection', socket => {
	client = socket;
  client.on('data', receiveMessage);
  sendMessage();
});

server.on('close', () => {
	if (rl) console.log();
	console.log('Server shut down');
  if (rl) rl.close();
});

server.on('error', () => {
	if (rl) console.log();
	console.log('An error ocurred');
  if (rl) rl.close();
});
async function sendMessage() {
  rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout,
  });

  rl.question('Type anything to send a packet (-1 to exit): ', async msg => {
    rl.close();
    if (msg !== '-1') {
      await sendPacket(PACKET);
      sendMessage();
    } else {
    	client.end();
      server.close();
    }
  });
}

/* Helper for sendMessage */
async function sendPacket(packet) {
  const e = Packet.verify(packet);
  if (e) throw Error(e);

  const message = Packet.create(packet);
  const buffer = Packet.encodeDelimited(message).finish();
  client.write(buffer);
}

function receiveMessage(message) {
  const packet = receivePacket(message);
  process.stdout.write('\nMessage Received: ');
  console.log(packet);
  process.stdout.write('Type anything to send a packet (-1 to exit): ');
}

/* Helper for receiveMessage */
function receivePacket(buffer) {
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