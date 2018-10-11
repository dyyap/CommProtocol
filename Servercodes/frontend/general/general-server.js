/*
 * JS <- C++, Frontend <- Backend
 * To run, do the following scripts:
 *
 * node general-server.js
 */

const net = require('net');

const HOST = '127.0.0.1';
const PORT = 59000;

const server = net.createServer();
server.listen(PORT, HOST, console.log('Receiving incoming messages...'));

server.on('connection', socket => {
  socket.on('data', printData);
});

function printData(data) {
  console.log(`Received message: ${data}`);
}
