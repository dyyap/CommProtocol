/*
 * JS -> C++, Frontend -> Backend
 * To run, do the following scripts:
 *
 * node general-client.js
 */

const net = require('net');

const HOST = '127.0.0.1';
const PORT = 6969;
const MESSAGE = 'Hello world';

const client = net.connect(PORT, HOST);

client.on('connect', () => {
  console.log('Client connected, sending message...');
  sendMessage(MESSAGE);
});

client.on('close', () => console.log('Client disconnected'));

client.on('error', () => console.log('No server detected'));

function sendMessage(message) {
  client.write(message);
}