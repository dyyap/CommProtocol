/*
 * JS -> C++, Frontend -> Backend
 * To run, do the following scripts:
 *
 * node general-client.js
 */

const net = require('net');
const readline = require('readline');

const HOST = '127.0.0.1';
const PORT = 59000;

const client = net.connect(PORT, HOST);

client.on('connect', () => {
  sendMessage();
});

client.on('close', () => console.log('Client disconnected'));

client.on('error', () => console.log('No server detected'));

function sendMessage() {
  const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout,
  });

  rl.question('Message to send (-1 to exit): ', msg => {
    rl.close();
    if (msg !== '-1') {
      client.write(msg);
      sendMessage();
    } else {
      client.end();
    }
  });
}
